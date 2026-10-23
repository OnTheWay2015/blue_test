# AIAgent 宿主程序与大模型交互流程解析

> 示例场景：在 `d:/work_ai/test.lua` 中添加一个 `helloWorld()` 方法，打印 `helloworld`。
>
> 主体：**AIAgent 宿主程序（本地 Agent 主程序） ↔ 远端大模型（LLM）**
>
> 核心三块：完整交互流程、请求/响应消息结构、`tool_use` 返回后代码插入修改的定位机制。

---

## 术语约定

| 术语 | 说明 |
|---|---|
| 宿主 / Agent Host | 本地程序，负责文件读写、命令执行、工具调度、维护会话上下文、解析 LLM 输出、做沙箱/权限校验。LLM 本身**不能直接碰本地磁盘**，所有本地操作都必须通过宿主暴露的工具。 |
| tool_use | LLM 在思考后输出工具调用指令（不是自然语言），告诉宿主：我要调用哪个工具、参数是什么；宿主执行工具、拿到结果，再把工具执行结果塞回对话上下文，继续交给 LLM。 |
| file_edit | 代码修改工具的常见命名（也叫 `write_file` / `patch_file` / `replace_in_file`），下文统一用 `file_edit`。 |

---

## 一、完整端到端交互时序

整个流程是**多轮对话 + 工具环（Agent Loop）**，不是单次一问一答。

```
1. 用户输入 Prompt（宿主接收）
   用户：在 d:/work_ai/test.lua 中添加一个打印 helloworld 的 helloWorld 方法

2. 宿主组装系统提示词 + 历史上下文 + 用户消息 → 发给 LLM（第一轮请求）
   ✅ System Prompt 里面固定包含：
   - 工具清单（tools schema：file_read / file_edit / write_file / ls 等）
   - 输出规范：想操作本地文件必须输出 tool_use，不能直接幻想改文件
   - 文件编辑规则：行号定位、搜索替换、追加、全量覆盖、diff 格式约定
   - 权限约束：只能操作允许目录 d:/work_ai/

3. LLM 思考：要新增函数，我需要先知道 test.lua 当前内容，避免覆盖已有代码
   → LLM 输出 tool_use：调用 file_read，读取 d:/work_ai/test.lua

4. 宿主收到 LLM 的 tool_use 响应，校验工具合法性
   - 校验路径是否在白名单、路径规范化、防路径穿越
   - 执行 file_read：磁盘读取 test.lua 的原始文本 + 记录总行数
   - 构造 tool_result（工具返回报文），把文件原文塞进去

5. 宿主把【tool_result】追加到对话上下文，再次发请求给 LLM（第二轮）
   > 现在 LLM 已经拿到 test.lua 当前源码，知道文件末尾/合适位置在哪

6. LLM 再次思考：确定插入位置，生成编辑指令 → 输出 tool_use: file_edit
   意图：在文件末尾追加 helloWorld 函数

7. 宿主解析 file_edit 参数，执行本地文件修改（定位机制在这里生效）
   - 根据定位参数（搜索块 / 行号 / 追加标记）找到目标位置
   - 做冲突检查：搜索文本是否匹配当前磁盘真实文件内容（防 race 条件）
   - 写入变更，保存 d:/work_ai/test.lua
   - 生成 tool_result：成功、diff 变更片段、新文件摘要

8. 宿主把修改结果 tool_result 继续送入上下文，发给 LLM（第三轮）

9. LLM 收到工具执行结果，确认修改成功，不再调用工具，输出自然语言总结给用户
   > "已经在 d:/work_ai/test.lua 添加 helloWorld() 方法..."
```

**终止条件**：LLM 不再输出 tool_use，输出普通 content；
**循环条件**：LLM 持续输出 tool_use，Agent 继续执行工具、回填结果。

```
用户 → 宿主 → LLM(思考读文件) → tool_use(file_read) → 宿主读文件 → tool_result
→ 宿主 → LLM(思考编辑) → tool_use(file_edit) → 宿主修改lua文件 → tool_result
→ 宿主 → LLM(确认完成) → 自然语言回复用户
```

---

## 二、请求/响应消息结构

现在 Agent 框架两大范式：

1. **OpenAI 系**：`tools` + `tool_calls`
2. **Anthropic 系**：`tool_use` / `tool_result`（很多自研 AIAgent 宿主直接抄这套，**tool_use 这个名字就是 Anthropic 原生字段**）

### 2.1 宿主发给 LLM 的请求体（Anthropic 风格）

第一轮，用户刚发需求：

```json
{
  "system": "你是代码Agent，可以操作本地文件。修改文件必须调用工具。工具列表：file_read、file_edit。file_edit支持按搜索块替换或者追加。编辑前尽量先读文件。路径限制 d:/work_ai/...",
  "messages": [
    {
      "role": "user",
      "content": "在 d:/work_ai/test.lua 中添加一个打印 helloworld 的 helloWorld 方法"
    }
  ],
  "tools": [
    {
      "name": "file_read",
      "description": "读取本地文本文件",
      "input_schema": {
        "type": "object",
        "properties": {
          "path": {"type": "string"}
        },
        "required": ["path"]
      }
    },
    {
      "name": "file_edit",
      "description": "修改文件，支持 search/replace 或者 append",
      "input_schema": {
        "type": "object",
        "properties": {
          "path": {"type": "string"},
          "action": {"type": "string", "enum": ["append", "replace", "insert"]},
          "search": {"type": "string", "description": "原文件匹配文本块"},
          "replace": {"type": "string", "description": "替换后的内容"},
          "insert_after": {"type": "string", "description": "在匹配文本之后插入"},
          "insert_line": {"type": "integer", "description": "行号插入模式"}
        },
        "required": ["path", "action"]
      }
    }
  ]
}
```

### 2.2 LLM 返回：tool_use 报文（第一轮返回，决定读文件）

```json
{
  "content": [],
  "stop_reason": "tool_use",
  "tool_use": [
    {
      "id": "tu_001",
      "name": "file_read",
      "input": {
        "path": "d:/work_ai/test.lua"
      }
    }
  ]
}
```

> **重点**：`tool_use.id` 是**关联ID**！后面宿主返回 tool_result 的时候必须带上同一个 id，LLM 才能知道：这个返回结果对应哪一次工具调用。这是会话绑定关键。

### 2.3 宿主执行完 file_read，回填 tool_result（宿主发给 LLM 的新一轮消息）

```json
{
  "role": "user",
  "content": [
    {
      "type": "tool_result",
      "tool_use_id": "tu_001",
      "content": "-- test.lua original content\nlocal x = 10\nfunction foo()\n    print(x)\nend\n"
    }
  ]
}
```

> 这条消息是**宿主伪造、插入到对话流里**的，不是用户输入。LLM 看到它，就相当于看到了文件读取结果。

### 2.4 LLM 第二轮输出 tool_use：file_edit（真正改 lua）

```json
{
  "content": [],
  "stop_reason": "tool_use",
  "tool_use": [
    {
      "id": "tu_002",
      "name": "file_edit",
      "input": {
        "path": "d:/work_ai/test.lua",
        "action": "append",
        "content": "\nfunction helloWorld()\n    print(\"helloworld\")\nend\n"
      }
    }
  ]
}
```

也可以是 `search + replace` 模式（如果不是追加，是插入到指定位置）：

```json
"input": {
  "path": "d:/work_ai/test.lua",
  "action": "insert_after",
  "search": "function foo()\n    print(x)\nend",
  "content": "\nfunction helloWorld()\n    print(\"helloworld\")\nend\n"
}
```

### 2.5 宿主执行 file_edit 之后，回填 tool_result

```json
{
  "role": "user",
  "content": [
    {
      "type": "tool_result",
      "tool_use_id": "tu_002",
      "content": "SUCCESS\nDiff:\n--- original\n+++ modified\n@@ -3,4 +3,8 @@\n function foo()\n     print(x)\n end\n+\n+function helloWorld()\n+    print(\"helloworld\")\n+end\n\nFile saved to d:/work_ai/test.lua"
    }
  ]
}
```

### 2.6 LLM 收到 tool_result，确认成功，不再调用工具，返回普通文本

```json
{
  "content": [
    {
      "type": "text",
      "text": "已在 d:/work_ai/test.lua 文件末尾添加 helloWorld 函数，调用后会打印 helloworld。\n```lua\nfunction helloWorld()\n    print(\"helloworld\")\nend\n```"
    }
  ],
  "stop_reason": "end_turn"
}
```

> OpenAI function calling 版本只是字段名换成 `tool_calls` / `tool` role，逻辑环完全一致，只是 schema 名字不一样。

---

## 三、重点：tool_use → file_edit 的修改定位机制

LLM **本身没有文件、没有行号视图**；定位信息全部编码在 tool_use.input 参数里，由**宿主程序负责解析、定位、校验、落地**。

行业里主流 4 套定位策略（很多 Agent 同时支持多种，LLM 根据场景选）：

### 方式1：Search + Replace 块匹配（最稳、最推荐，Cursor / Claude Code 默认）

**参数**：`search`（原始文本块）、`replace`（替换后） / `insert_after`

**逻辑**：

1. 宿主打开当前磁盘文件，把文件读成完整字符串；
2. 在全文中**精确查找 search 文本片段**；
3. ✅ 匹配唯一：在该块位置做替换 / 在块后面插入；
4. ❌ 匹配 0 处：报错 tool_result 返回 `search text not found`（文件已经被外部改动，上下文失效）；
5. ❌ 匹配多处：拒绝修改，返回歧义错误，防止误改。

> **优点**：**不依赖行号**，不怕文件前面增删代码导致行号漂移；
> **缺点**：LLM 必须准确记住它上一轮读到的文本，不能写错 search 字符串。
>
> 这个是 Agent 代码编辑最核心的冲突防护机制。
>
> 在我们 lua 例子里，如果选择 `insert_after: "function foo() ... end"`，宿主就找到 foo 函数那块，后面塞 helloWorld。

### 方式2：行号定位模式 `insert_line / start_line, end_line`

**参数**：`insert_line: 5`，代表在第 5 行之后插入

**逻辑**：

1. 宿主加载文件，按 `\n` split 成行数组；
2. 校验行号在 1 ~ total_lines；
3. 在指定下标位置 splice 插入代码；

> **坑点**：**行号很容易失效**。LLM 拿到的是上一轮 file_read 的快照；如果文件在两次工具调用之间被外部编辑器改动，行号全部错位 → 插错地方。
>
> 所以成熟 Agent 都会优先 search 块匹配，行号只作为辅助。

### 方式3：Append / Prepend 标记（追加到文件末尾 / 文件开头）

就是我们这个例子最简单的方案：`action: append`

宿主不需要搜索、不需要行号，直接把 content 拼到文件末尾。

适合新增函数、新增测试代码；缺点：无法插入到文件中间。

### 方式4：全量覆盖 write_file

tool 是独立工具 `write_file`，参数 path + content，直接覆盖整个文件。

适合新建文件；已有文件慎用，容易丢内容。

### 定位校验与安全锁（宿主侧必须做，很多自研 Agent 漏掉）

1. **快照一致性校验**：每次 file_read 后宿主可以记录文件 hash；file_edit 前重新读磁盘比对 hash；hash 变化 → 说明外部修改，拒绝编辑，返回冲突；
2. **搜索块白空格容错（可选）**：部分宿主支持忽略末尾换行/空格匹配，但严格模式下完全精确匹配，避免模糊匹配改错；
3. **差分预览**：宿主计算 diff，tool_result 把 diff 返回给 LLM，LLM 能自检改的对不对；
4. **回滚机制**：编辑前备份原文件，失败时恢复。

### 定位机制一句话总结

> LLM 在 tool_use 输入里描述【在哪里改】（search 文本/行号/append 标记），**LLM 不负责定位实现**；宿主拿到这个定位描述，加载磁盘真实文件，执行字符串查找/行拆分，定位目标区间，执行变更；变更结果（成功/失败/diff）打包进 tool_result，带着同一个 tool_use_id 塞回对话上下文，让 LLM 感知变更结果。

---

## 四、容易混淆的边界澄清（很多人踩坑）

1. ❌ LLM 直接输出 lua 代码字符串，宿主自动识别并改文件：这是弱方案，没有标准工具协议，没有冲突检测；正规 Agent 走显式 tool_use；
2. ❌ tool_use 是 LLM 调用宿主函数，不是宿主调用 LLM 函数；方向不要搞反；
3. ❌ tool_result 不是发给用户的消息；它是**给 LLM 看的返回值**，属于内部上下文；用户看不到 tool_result 原始内容，宿主可以选择把 diff 摘要展示给用户；
4. 整个上下文是累积的：user msg → tool_use → tool_result → user msg（继续），形成一个环；LLM 的视野就是这个对话数组。

---

## 五、套回 test.lua helloWorld 的极简数据流摘要

1. 用户：加 helloWorld 打印 helloworld
2. LLM：我要读文件 → `tool_use(file_read)`
3. 宿主：读 `d:/work_ai/test.lua` 返回原文 → `tool_result`
4. LLM：文件末尾是空位置，append 函数 → `tool_use(file_edit, action=append)`
5. 宿主：打开文件，追加 lua 代码，保存，生成 diff → `tool_result`
6. LLM：看到 success diff，结束工具循环，回复用户
