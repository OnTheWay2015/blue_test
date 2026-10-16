# Agent 技能实现原理及 using\-git\-worktrees 详解

# 一、核心概念区分：Tool（原子工具） vs Skill（业务技能）

## 1\. Tool（底层原子工具）

是最小可执行单元，为单一能力载体，比如执行shell命令、读写文件、git操作、http请求等，具备固定入参Schema，由Agent运行时直接调用。

常见示例：`shell_exec`、`git_add`、`mkdir` 等。

## 2\. Skill（业务技能）

**using\-git\-worktrees 属于典型Skill**，其核心本质是：一套封装完备的业务流程SOP，并非单一函数。由 **LLM可读规则文档 \+ 多Tool编排逻辑 \+ 前置校验/后置清理/异常兜底机制** 组成，核心作用是约束Agent按照固定标准化流程完成特定场景任务，避免LLM自由发挥导致流程混乱。

Claude Superpowers 标准Skill目录结构：

```Plain Text
using-git-worktrees/
├─ SKILL.md       # 核心：LLM决策依据、触发条件、完整执行步骤、约束规则
├─ scripts/       # 辅助bash/python脚本，封装git worktree底层操作
└─ references/    # 参考文档、安全校验规则
```

# 二、Agent Skill 通用加载与执行全链路

## 阶段1：技能发现与注册（启动阶段）

1\. Agent 扫描全局/项目 `skills/` 目录，仅解析每个Skill下 `SKILL.md` 的头部元数据（名称、描述、适用场景），不加载全文，节省运行内存。

2\. 将所有Skill元数据注入系统Prompt，让LLM感知自身可使用的技能及对应适用场景。

3\. 将Skill封装为标准化Function Call工具，注册至Tool注册表，提供统一调用入口。

## 阶段2：LLM 自动匹配、激活技能（推理阶段）

当Agent接收开发任务（新增功能、修复bug、代码重构等）时，LLM遍历所有Skill描述，匹配对应触发条件。

**using\-git\-worktrees 核心触发规则**（定义于 SKILL\.md）：

需要新增功能、紧急修复、并行开发，需隔离运行环境、避免污染主工作区时，强制启用本技能；若当前已处于worktree内部，直接跳过，禁止嵌套创建多层worktree。

匹配成功后，Agent加载 `SKILL.md` 全文至上下文，LLM严格按照文档步骤编排调用底层工具。

## 阶段3：Skill 流程编排执行（核心运行阶段）

LLM放弃自由推理，完全遵循SKILL\.md预设流程，分步调用 `shell`、`file`、`git` 等原子Tool，每一步均内置安全校验、失败回滚逻辑，保障执行稳定性。

## 阶段4：执行结果回写上下文

命令输出、目录路径、测试结果等所有执行信息，均作为观测结果返回LLM，后续所有代码操作、开发调试自动切换至新建的worktree隔离目录。任务结束后，自动提供分支合并、环境清理等收尾流程。

# 三、using\-git\-worktrees 技能完整实现细节

## 1\. 底层核心依赖：Git Worktree 原生能力

Git 原生提供 `git worktree add` 能力，实现**多目录共享同一仓库对象库、独立工作区**的隔离效果，核心特性如下：

- 主仓库保留完整 `.git` 仓库配置目录；

- 新建worktree目录内的 `.git` 仅为软链接/指针文件，指向主仓库，极致节省磁盘空间；

- 各个worktree拥有独立HEAD、文件修改区、暂存区，相互完全隔离、互不污染；

- 核心限制：同一分支无法同时被两个worktree检出，避免代码冲突。

该Skill的本质，是将零散的原生git命令，封装为**带安全防护、自动路径决策、环境初始化、基线校验**的标准化自动化开发流程。

## 2\. SKILL\.md 定义的完整执行流程

### 步骤0：前置拦截校验（安全第一道门）

1\. 检测当前运行环境是否已处于linked worktree：

```Plain Text
git rev-parse --git-dir
```

若返回路径非主仓库 `.git` 目录，直接终止技能执行，杜绝worktree嵌套问题。

2\. 自动获取仓库根目录、项目名称、当前主分支等基础环境信息。

### 步骤1：自动决策worktree存放目录（三层优先级链）

目录优先级规则：`已有目录 > 项目配置CLAUDE.md > 询问用户`

1\. 优先检测项目本地隐藏目录 `.worktrees/`，其次匹配 `worktrees/`，两者共存时优先选用`.worktrees`；

2\. 自动校验目录是否已写入`.gitignore`，未忽略则自动追加配置并提交，防止临时沙盒文件被提交至仓库；

3\. 无本地目录时，读取项目 `CLAUDE.md` 预设的worktree路径配置；

4\. 无预设配置时，提供路径选择：项目本地目录 / 全局用户目录 `~/.local/spellbook/worktrees`；

5\. 确认路径后自动写入 `CLAUDE.md` 固化配置，下次执行无需重复询问。

### 步骤2：创建独立worktree开发沙盒

1\. 根据当前任务、功能名称自动生成标准化分支名（如 `feature/xxx`、`hotfix/bug-id`）；

2\. 拼接完整worktree隔离路径：`{worktree_root}/{branch_name}`；

3\. 调用原生git命令创建隔离工作区，默认基于主分支main/master创建，支持自定义基线分支：

```Plain Text
git worktree add -b $BRANCH $FULL_PATH $BASE_REF
```

4\. 将新建的worktree与当前任务ID绑定，存入 `.tasks/` 索引文件，方便后续统一管理、切换、清理。

### 步骤3：自动初始化隔离开发环境

自动进入worktree隔离目录，识别项目技术栈，执行对应依赖安装，初始化可运行环境：

```Plain Text
# Node 项目
[ -f package.json ] && npm install
# Python 项目
[ -f pyproject.toml ] && poetry install
[ -f requirements.txt ] && pip install -r requirements.txt
# Rust/Go 等其他技术栈同理适配
```

### 步骤4：基线校验（质量门禁）

环境初始化完成后，自动执行项目预设测试命令，校验沙盒基线环境干净无异常：

- 测试通过：确认沙盒就绪，Agent后续所有文件修改、编译、调试、提交操作，全部强制路由至该隔离目录；

- 测试失败：主动提示用户，提供「继续开发」「清理重建worktree」两种兜底选择。

### 步骤5：后置生命周期管理（任务收尾）

SKILL\.md预设两种标准化收尾流程，适配不同开发场景：

1\. 合并模式：开发完成后，自动切换回主工作区，合并worktree分支代码，执行 `git worktree remove` 清理沙盒环境，释放磁盘空间；

2\. 保留模式：针对长期并行开发场景，保留worktree目录及分支，记录路径信息，支持后续复用；

3\. 异常兜底：任务意外中断时，自动触发环境回收逻辑，避免磁盘残留无效worktree文件。

## 3\. Skill 内置强制安全约束（LLM不可绕过）

- 项目本地worktree目录必须加入 `.gitignore`，严格禁止沙盒临时文件提交至代码仓库；

- 禁止在主工作区直接修改业务代码，所有开发操作强制路由至隔离worktree目录；

- 限制单一分支仅能绑定一个worktree，杜绝分支冲突；

- 全局目录文件不纳入仓库追踪，无需执行ignore校验；

- 所有shell操作配置路径白名单，仅允许读写worktree内部文件，防止篡改主仓库代码。

## 4\. 脚本层封装能力

在 `scripts/` 目录封装通用bash工具脚本，抽取重复逻辑，LLM仅调用标准化封装脚本，无需手写原生git命令，降低出错概率：

- `detect-worktree-root.sh`：实现三层路径优先级判断、目录合法性校验；

- `create-isolation.sh`：统一封装worktree创建、环境校验、分支初始化逻辑；

- `baseline-test.sh`：自动识别项目技术栈，执行基线测试校验；

- `clean-worktree.sh`：安全回收worktree环境、清理无效分支，避免残留垃圾文件。

# 四、主流 Skill 实现架构对比

## 方案A：Claude Superpowers 声明式Skill（using\-git\-worktrees 采用）

**核心逻辑**：纯文档驱动，`SKILL.md` 同时承担「LLM指令手册 \+ 执行流程规范」双重作用。

**特点**：代码量极少，执行流程由LLM读取文档后自行编排Tool调用；上手门槛低、可灵活修改、支持版本管理，普通开发者可自定义扩展Skill。

**短板**：复杂分支、异常兜底逻辑依赖LLM对文档的理解，极端场景下流程稳定性略弱。

## 方案B：代码式Skill（LangChain/LlamaIndex 主流方案）

**核心逻辑**：Skill为Python/JS实体类，硬编码完整执行流程，串行调用底层Tool，LLM仅负责场景判断、触发技能，不参与流程编排。

**特点**：执行流程高度稳定、异常处理精准、可控性极强，适合核心关键场景。

**短板**：修改技能逻辑需要改动代码，自定义扩展门槛较高。

# 五、代码式 Skill 极简伪代码示例

```Plain Text
class UsingGitWorktreesSkill(BaseSkill):
    name = "using-git-worktrees"
    description = "创建隔离git worktree，实现并行开发，不污染主工作区"

    def run(self, task_name: str, base_branch: str = "main") -> str:
        # 1. 前置拦截：检测是否已处于worktree环境
        git_dir = self.shell.exec("git rev-parse --git-dir").strip()
        repo_root = self.shell.exec("git rev-parse --show-toplevel").strip()
        if git_dir != f"{repo_root}/.git":
            return f"当前已在worktree {git_dir}，无需新建隔离环境"

        # 2. 解析worktree存储根目录
        wt_root = self.resolve_worktree_root()
        branch_name = f"feature/{task_name}"
        wt_path = Path(wt_root) / branch_name

        # 3. 创建全新worktree隔离环境
        self.shell.exec(f"git worktree add -b {branch_name} {wt_path} {base_branch}")

        # 4. 自动初始化项目依赖环境
        self.init_project_env(wt_path)

        # 5. 执行基线测试校验环境合法性
        test_result = self.run_baseline_test(wt_path)
        if not test_result.success:
            raise Exception(f"沙盒基线测试失败：{test_result.log}")

        # 切换Agent全局工作目录至隔离沙盒，后续操作全部隔离执行
        self.agent.context.cwd = wt_path
        return f"隔离worktree开发环境就绪：{wt_path}"

    def resolve_worktree_root(self):
        # 实现三层目录优先级判断逻辑
        pass

    def init_project_env(self, wt_path):
        # 自动识别技术栈、安装项目依赖
        pass

    def run_baseline_test(self, wt_path):
        # 执行项目基线测试、返回校验结果
        pass
```

# 六、核心总结

1. **Skill 本质**：标准化业务流程包，由规则文档、辅助脚本、底层原子Tool组合而成，用于约束Agent行为，规范场景化执行流程；

2. **using\-git\-worktrees 核心价值**：基于Git原生worktree能力，封装「自动路径选择\+安全环境隔离\+项目初始化\+基线校验\+全生命周期清理」的完整开发沙盒流程；

3. **整体运行链路**：LLM场景匹配触发技能 → 加载标准化SOP文档 → 分步调用底层工具执行 → 全局操作路由至隔离目录 → 任务收尾环境回收；

4. **核心作用**：彻底解决Agent开发过程中污染本地主分支、多任务并行相互干扰的问题，提供安全、独立的代码开发沙盒环境。

> （注：部分内容可能由 AI 生成）
