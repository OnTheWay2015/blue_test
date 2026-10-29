# Cocos2d‑x v4 Material 类设计解析与多Pass渲染优化

> 源码参考：`CCMaterial.h` / `CCMaterial.cpp`；设计思想借鉴 **OGRE3D、GamePlay3D** 的材质架构。
> 整体层级：**Material（材质） → Technique（渲染方案） → Pass（渲染通道） → ProgramState(Shader)+RenderState(管线状态)**。

---

## 第一部分：Material 类源码分析

### 一、整体设计思路

Material 代表物体"怎么画"，不直接写死 shader；把渲染行为分层解耦：

1. **Material**：顶层资源对象，一个材质可以拥有**多套渲染方案 Technique**，运行时可快速切换；
2. **Technique**：一套完整渲染方案，可包含**多个 Pass**，支持多通道渲染；
3. **Pass**：真正执行一次绘制单元，持有 `backend::ProgramState`（着色器+uniform+纹理采样器）和 `RenderState::StateBlock`（深度、混合、面裁剪等GPU固定管线状态）；
4. **RenderState**：GPU管线状态块，支持继承覆盖：Material全局状态 → Technique状态 → Pass局部状态，越下层优先级越高；
5. 支持两种来源：`.material`配置文件解析、代码直接传入`ProgramState`（兼容旧2D逻辑）。

> **v4重大变化**：后端抽象层`backend`，不再硬编码OpenGL，统一封装OpenGL/Metal；`backend::ProgramState`替代旧`GLProgramState`，隔离图形API差异。

### 二、类关键要素拆解（成员变量 + 关键接口）

#### 1. 继承关系

```cpp
class Material : public Ref
```

继承`Ref`，使用引用计数内存管理，`autorelease()`，符合cocos内存模型。

#### 2. 成员变量（头文件）

```cpp
std::string _name;                                 // 材质名字
RenderState _renderState;                          // Material层级全局渲染状态
Vector<Technique*> _techniques;                    // 一套材质可以多个Technique
Technique* _currentTechnique = nullptr;            // 当前生效的Technique（弱指针）
Node* _target = nullptr;                           // 绑定的渲染节点，弱引用
std::unordered_map<std::string, int> _textureSlots; // 纹理名→纹理单元槽位映射
int _textureSlotIndex = 0;                         // 分配纹理槽自增计数器
```

| 成员 | 作用 | 设计原因 |
|---|---|---|
| `_techniques` | 多个渲染方案集合 | 同一材质，可准备高/低画质、阴影、正常渲染多套technique，运行时`setTechnique()`一键切换，不用重新加载shader资源；比如远景物体切简化shader，近景用复杂光照shader。 |
| `_currentTechnique` | 当前激活方案 | **弱指针**，对象生命周期由`_techniques`管理，不增加引用计数；性能：频繁切换只改指针，无内存操作。 |
| `_renderState` | 材质全局RenderState | 层级覆盖：Material的renderState会被Technique覆盖，Technique又会被Pass覆盖，越下层优先级越高；允许在材质顶层统一设置混合、深度测试，pass可以局部重写。 |
| `_textureSlots` | 纹理名绑定GPU纹理单元 | 解析`.material`中`sampler`，把不同纹理分配到不同GPU texture unit，避免重复占用槽位；同一个uniform纹理名字复用同一个slot索引。 |
| `_target` | 弱引用Node | 知道自己属于哪个渲染节点，不持有node防止循环引用。 |

#### 3. 静态创建接口（三种创建途径）

```cpp
// 从.material配置文件加载
static Material* createWithFilename(const std::string& path);
// 从Properties内存对象创建（内存解析，不读磁盘）
static Material* createWithProperties(Properties* materialProperties);
// 直接用ProgramState创建（兼容老代码，自动生成1个Technique+1个Pass）
static Material* createWithProgramState(backend::ProgramState* programState);
```

**设计意图：多入口兼容不同场景**

1. `createWithFilename`：美术写`.material`资源文件，游戏直接加载资源；
2. `createWithProperties`：资源已经读到内存，不读磁盘，工具/运行时动态构造；
3. `createWithProgramState`：兼容2D老逻辑（Sprite等），不需要写material配置，直接传入shader状态，快速生成最简Material。

#### 4. 初始化函数

```cpp
bool initWithFile(const std::string& file);
bool initWithProperties(Properties* materialProperties);
bool initWithProgramState(backend::ProgramState* state);
```

核心：`parseProperties()`解析Properties配置树。`.material`文件会被解析成`Properties`命名空间树，类似xml/json的分层结构：

```
material {
  renderState { ... }
  technique {
    pass {
      shader { vertexShader fragShader defines ... }
      renderState { ... }
      sampler { path wrapS ... }
    }
  }
}
```

`parseProperties`遍历命名空间，依次调用`parseTechnique`→`parsePass`→`parseShader`→`parseSampler`/`parseUniform`完成整个材质解析。

#### 5. draw绘制入口

```cpp
void draw(MeshCommand* meshCommand, float globalZOrder, backend::Buffer* vertexBuffer,
backend::Buffer* indexBuffer, CustomCommand::PrimitiveType primitive,
CustomCommand::IndexFormat indexFormat, unsigned int indexCount, const Mat4& modelView);
```

```cpp
// cpp实现
for (const auto& pass: _currentTechnique->_passes)
{
    pass->draw(&meshCommands[i], ...);
    i++;
}
```

- Material本身**不直接调用GPU绘制API**，只是遍历当前Technique的所有Pass，逐个交给Pass执行draw；
- 支持**多Pass渲染**：一个物体多次绘制（描边、卡通描边、体积光等多通道效果）。
- 输出`MeshCommand`渲染命令，推到Renderer渲染队列，**符合cocos命令式渲染架构**：渲染对象只生产Command，Renderer统一批量提交GPU，实现合批、排序。

#### 6. clone() 深拷贝

```cpp
virtual Material* clone() const;
```

做完整深拷贝：复制renderState，clone全部Technique，重建内部`_textureSlots`映射。

> 设计目的：原始Material是共享资源；当某个3D实例需要修改材质参数（改颜色、纹理），不能修改公共原始材质，调用`clone()`生成独立实例，修改只作用于该物体，不污染其他使用同一材质的模型。

> ⚠️ 源码有一处笔误bug：
> ```cpp
> material->_textureSlots = material->_textureSlots; // 自己赋值给自己，正确应该是 material->_textureSlots = this->_textureSlots;
> ```

#### 7. parseXXX系列解析函数

- `parseTechnique`：解析technique命名空间，创建Technique对象；
- `parsePass`：解析pass命名空间，创建Pass对象；
- `parseShader`：读取vert/frag shader源码，拼接`defines`宏，创建`backend::Program`，生成`ProgramState`；
  - `replaceDefines()`：处理material中的defines配置，自动补全`#define`，拼接到shader源码头部；
- `parseUniform`：解析配置里的uniform常量（数字、vec2/3/4、矩阵）设置到ProgramState；支持自动参数绑定；
- `parseSampler`：加载纹理，设置纹理过滤、环绕模式，绑定到ProgramState的uniform采样器；
- `parseRenderState`：解析深度测试、混合模式等，写入StateBlock。

#### 8. Technique管理接口

```cpp
Technique* getTechniqueByName(const std::string& name);
Technique* getTechniqueByIndex(ssize_t index);
void setTechnique(const std::string& techniqueName);
```

运行时切换渲染方案。

### 三、分层架构为什么要这样设计（核心设计考量）

#### 1. 关注点分离（解耦）

| 模块 | 职责 |
|---|---|
| Material | 资源外壳，管理多套渲染方案，顶层render state |
| Technique | 一套完整渲染方案，多个pass，适配不同硬件/画质 |
| Pass | 单次绘制单元，shader程序 + 本pass管线状态 |
| ProgramState | shader源码、uniform变量、纹理采样器（逻辑层） |
| RenderState::StateBlock | GPU固定管线状态：depth/stencil/blend/cull |

> 如果把所有shader、渲染状态全部塞到Material一个类，会非常臃肿，多pass、多technique逻辑无法实现。

#### 2. 支持多Technique（渲染方案切换）

> 典型场景：
> - 高画质technique：PBR光照；低画质technique：简化漫反射；根据设备性能切换；
> - 正常渲染technique；阴影投射technique；运行时切换。
> - 不需要重新加载编译shader，只修改`_currentTechnique`指针，开销极小。

#### 3. 支持多Pass（多通道渲染）

一个Technique多个Pass，物体被绘制多次，实现描边、轮廓、叠加光照等效果。Material::draw循环遍历全部pass，依次提交绘制命令。

#### 4. RenderState层级覆盖机制

`Material renderState` → `Technique renderState` → `Pass renderState`。下层会覆盖上层配置。

- 比如Material设置`blend=true`，某一个pass想关闭混合，可以在pass的renderState重写；
- 美术写material配置文件时，可以公共状态写上层，特殊状态写下层，减少重复配置。

#### 5. 资源文件驱动 + 代码动态创建双模式

- 美术编辑`.material`文本配置，不用改C++代码；
- 程序也可以完全不靠文件，直接`createWithProgramState`动态生成材质，适配程序动态生成效果。

#### 6. 适配后端抽象层 backend（v4核心）

Material**不直接调用gl函数**，全部操作通过`backend::ProgramState`，底层由backend翻译成OpenGL / Metal指令。同一套Material上层逻辑，跨平台不用修改。

#### 7. clone实例机制：共享资源 + 实例修改分离

原始Material作为共享资源，多个模型共用；如果某个模型需要修改材质参数，调用clone生成独立副本。避免修改公共材质，导致所有模型一起被改动。

#### 8. 命令式渲染，不直接调用GPU

Material::draw不会直接glDraw，而是输出MeshCommand，交给Renderer队列。引擎可以统一做zOrder排序、状态排序、合批，提高渲染性能。

### 四、源码里的小缺陷（阅读源码可以看到）

1. clone函数内部bug：`material->_textureSlots = material->_textureSlots;` 自赋值，没有拷贝源对象的textureSlots；
2. `parseSampler`中minFilter解析逻辑bug：多个mipmap过滤枚举，内部全部赋值为NEAREST / LINEAR，没有正确区分mipmap的filter；
3. Properties是非Ref对象，`initWithFile`中必须手动CC_SAFE_DELETE(properties)，忘记就内存泄漏。

### 五、完整调用链路示例（从文件到绘制）

```
createWithFilename("xxx.material")
    → initWithFile()
        → Properties::createNonRefCounted()读取解析文本
        → parseProperties()
            → parseTechnique()
                → parsePass()
                    → parseShader() 加载shader源码，拼接defines，创建ProgramState
                        → parseUniform() 设置uniform常量
                        → parseSampler() 加载纹理，分配纹理单元
                    → parseRenderState()解析pass的管线状态
draw()
    → _currentTechnique 遍历所有pass
        → pass->draw() 填充MeshCommand
Renderer遍历MeshCommand，backend执行真正GPU绘制
```

---

## 第二部分：多Pass渲染的优化思路

> 回顾概念：**1个Pass = 向GPU提交1次绘制（一次DrawCall）**。
> Technique下多个Pass，就意味着**同一个Mesh要执行多次DrawCall**。
> 例如2个Pass，同一个模型绘制2遍：第一遍画主体，第二遍画外描边。
> 多Pass天生开销：多次DrawCall、重复的顶点数据提交、多次修改GPU状态。
> Cocos Material/Pass架构本身只是描述"要画几遍"，**不自带优化，需要业务层做优化**。

### 1、先认清多Pass带来的性能开销

每一次Pass::draw，都会产生：

1. 新的`MeshCommand`，一次DrawCall；
2. 可能切换ProgramState（shader切换）；
3. 可能切换RenderState（混合、深度、裁剪）；
4. 顶点/索引缓冲区重复绑定（同一个mesh重复上传）。

> 举例：一个物体3个Pass，就产生3个DrawCall；场景100个这样物体就是300 DrawCall，CPU压力上涨明显。

> Material源码中draw实现：
> ```cpp
> // Material::draw
> for (const auto& pass: _currentTechnique->_passes)
> {
>     pass->draw(&meshCommands[i], ...);
>     i++;
> }
> ```
> 每个Pass独立生成MeshCommand，全部送入渲染队列。**引擎不会自动合并Pass**。

---

### 方案1：能不用多Pass就尽量不用（最高性价比）

很多传统多Pass效果，优先改成**单Pass多渲染目标(MRT)或者单Shader内部逻辑分支**，把多Pass改成单Pass，消除额外DrawCall。

**适用场景**

- 简单描边、边缘光、双面效果、简单叠加效果。
  - ❌ 旧方案：Pass1绘制物体本身；Pass2放大背面模型绘制描边（双Pass）
  - ✅ 优化方案：**单Pass在fragment shader内部直接计算描边，1次Draw完成**。

- 条件分支用`#define`材质宏(defines)开关，而不是新增Pass。
  - `.material`的shader支持`defines`字段，编译期开启关闭shader功能，只保留一个Pass。

> 注意：shader内部if分支不要过度，复杂光照不要堆大量动态if，会导致GPU性能下降。

### 方案2：必须使用多Pass的场景（确实无法合并）

典型必须多Pass：

1. **阴影投射**：物体一套Pass渲染到颜色缓冲区，另一套Pass渲染到阴影贴图（不同渲染目标FrameBuffer）
2. 后处理类物体：物体本身渲染 + 生成法线贴图、深度贴图；
3. 复杂卡通渲染：轮廓、偏移膨胀、多次混合叠加；
4. 一些烘焙、离屏渲染逻辑。

> 这种场景就必须保留多Pass，做下面这些优化。

---

### 多Pass场景下的运行时优化手段（Cocos4.x）

#### 1. 控制Pass执行顺序，减少GPU状态切换开销

Render队列按照`globalZOrder`排序，Material::draw传递同一个globalZOrder给所有Pass。

- Pass之间如果需要修改混合模式、cull face、深度读写，尽量**把状态相似的Pass放在相邻位置**。
- 同一个Technique内部Pass，尽量：先全部写深度，再做混合叠加；减少Blend、CullFace、DepthWrite来回切换。

> 引擎`Renderer`会根据Command的RenderState做状态对比，**状态没变就不会向backend发指令**。
> 如果相邻Pass RenderState完全一致，backend层可以省下大量状态设置开销。

#### 2. 复用Vertex/Index Buffer，减少资源绑定开销

看源码，Material::draw把vertexBuffer/indexBuffer统一传入所有Pass：

```cpp
void Material::draw(MeshCommand* meshCommands, float globalZOrder, backend::Buffer* vertexBuffer,
backend::Buffer* indexBuffer, ...)
{
    int i = 0;
    for (const auto& pass: _currentTechnique->_passes)
    {
        pass->draw(&meshCommands[i], globalZOrder, vertexBuffer, indexBuffer,...);
        i++;
    }
}
```

✅ **缓冲区对象只传一次，多个Pass复用同一个buffer句柄，不需要重复创建Buffer对象**。

> 但：虽然Buffer对象复用，每一次Pass的MeshCommand提交，驱动层依然会做一次draw。只能减少CPU侧资源创建，不能消除DrawCall。

#### 3. 材质clone的合理使用，避免Pass重复创建

```cpp
Material* clone() const;
```

- 共享原始`.material`资源，物体实例使用`clone()`得到实例材质；**不要每个物体都从文件createWithFilename**，避免每个实例重复解析生成Pass/ProgramState。

> 注意源码clone有bug `_textureSlots = _textureSlots`，使用时需要手动修复拷贝`_textureSlots`，否则纹理槽会错乱。

#### 4. 按设备性能选择Technique，减少移动端Pass数量

利用Material多Technique设计：

- `technique HighQuality`：3个Pass完整效果；
- `technique LowQuality`：1个Pass简化效果；

运行时根据设备性能：

```cpp
material->setTechnique("LowQuality");
```

低端设备直接切换到Pass数量少的渲染方案，直接减少DrawCall。

> 这是Cocos Material架构原生支持的能力，非常适合3D材质做设备分级。

#### 5. 多Pass物体，尽量减少实例数量

多Pass开销是**每个实例叠加**：

- 1个物体3Pass → 3DrawCall；100个物体 → 300 DrawCall。

优化思路：

1. 尽量把大量重复模型做**静态合并Mesh（MeshBatch）**，合并成一个大Mesh；
2. 合并后虽然依然多Pass，但每个Pass只产生1个DrawCall，而不是N个。

> 100个小模型合并为1个大Mesh：3Pass，总共3 DrawCall，而不是300。

⚠️ 限制：合并Mesh适合静态物体；如果物体需要独立移动旋转，不能合并。

---

### 针对多Pass的渲染队列与FrameBuffer优化

很多多Pass效果，第二个Pass渲染到**离屏FBO，而不是直接屏幕**。

1. 如果Pass输出到FBO：不要每帧每物体各创建FBO；**FBO池化复用**，避免频繁创建销毁渲染纹理。
2. 如果多个物体都需要做相同的第二Pass（比如描边）：不要每个物体各自执行Pass2；

> 优化思路：全部物体Pass1绘制到一张离屏RT，然后对整个RT做一次后处理shader完成描边。把N物体的N次Pass2变成**全局1次后处理Pass**，DrawCall会大幅下降。

> 这是大型3D项目最常用的优化手段：把"物体级多Pass"提升为"全屏后处理Pass"。

示例对比：

- ❌ 低效：每个物体有2个Pass，物体各自做描边；N物体 → 2*N DrawCall
- ✅ 高效：Pass1全部物体绘制到RT；全屏后处理shader对RT做描边；总DrawCall=N+1。N越大收益越高。

---

### Cocos多Pass的坑（源码层面）

1. **每个Pass生成独立MeshCommand，无法自动合并**
   引擎没有"Pass合并"逻辑，多Pass必然多条command。不要期待引擎自动优化。

2. **Pass之间uniform状态互相干扰**
   同一个Material多个Pass，每个Pass持有自己的ProgramState；但是如果共享纹理槽位`_textureSlots`，要注意解析时纹理单元分配冲突。

3. **多Pass+透明物体zOrder排序**
   多个Pass共用同一个globalZOrder，全部进入同一个z层。
   > 如果第二个Pass是半透明叠加，深度读写设置错误，会出现闪烁；需要仔细设置每个Pass RenderState深度测试、深度写入。

4. **移动端，Pass数量不要超过2‑3个，尽量控制**。Android低端机CPU对大量DrawCall非常敏感。

---

### 不同场景选型总结表

| 场景 | 处理方案 | 收益 |
|---|---|---|
| 简单描边、边缘光 | 删掉第二Pass，shader内部单Pass实现 | 消除额外DrawCall，收益最大 |
| 效果不能单Pass实现 | 使用多Pass；做Mesh静态合并，减少实例数 | 降低DrawCall数量 |
| 大量物体都需要第二Pass效果 | 物体只保留主Pass；把第二Pass改为全屏后处理 | 从 O(N) → O(1) 次额外绘制 |
| 设备性能差异 | 多Technique，低端设备切换少Pass方案 | 低端机自动降级 |
| 离屏渲染Pass | FBO对象池复用，不要每帧新建 | 减少GPU资源分配开销 |

---

### 简单示例伪代码

#### 1. 设备降级切换Technique

```cpp
// 低端设备使用少pass的technique
if (isLowEndDevice) {
    mat->setTechnique("LowQuality");
}
```

#### 2. 静态物体合并，减少多Pass带来的DrawCall膨胀

```cpp
// 把多个静态小mesh合并为一个大mesh
MeshBatch* batch = MeshBatch::create();
batch->addMesh(srcMesh1, transform1);
batch->addMesh(srcMesh2, transform2);
batch->setMaterial(mat); // mat内部是2个Pass
// 绘制batch的时候，2个Pass，仅产生2次DrawCall，而不是 2+2
```
#（注：内容由AI生成）
