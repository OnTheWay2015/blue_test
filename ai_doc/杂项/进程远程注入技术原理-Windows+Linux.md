# 进程远程注入技术原理（Windows + Linux）

> ⚠️ **重要前置声明**
>
> 以下内容仅为操作系统底层原理学习科普。**未经授权对他人程序、服务器进程进行注入属于违法违规行为；开发、传播、使用游戏外挂、恶意程序均触犯法律，会承担民事赔偿、账号封禁甚至刑事责任。严禁将下文技术用于非法用途。**

---

## 目录

- [Windows 远程注入](#windows-远程注入)
  - [1. 经典 CreateRemoteThread DLL 注入](#1-经典-createremotethread-dll-注入最广为人知)
  - [2. 进阶：不使用 CreateRemoteThread 的注入](#2-进阶不使用-createremotethread-的注入规避检测外挂常用变种)
  - [3. 配套关键 API（内存操作通用）](#3-配套关键-api内存操作通用)
  - [4. Windows 底层原理总结](#4-windows-底层原理总结)
  - [5. Windows 现代防护手段](#5-windows-现代防护手段)
- [Linux 远程注入](#linux-远程注入)
  - [1. 经典 ptrace 注入（最主流）](#1-经典-linux-注入ptrace-注入最主流)
  - [2. 不使用 ptrace 的注入方案](#2-不使用-ptrace-的注入方案规避ptrace检测root权限)
  - [3. 配套重要系统调用 / 函数](#3-配套重要系统调用--函数)
  - [4. Linux 注入核心原理总结](#4-linux-注入核心原理总结)
  - [5. Linux 现代防护手段](#5-linux-现代防护手段)
- [Windows 与 Linux 注入技术对照表](#windows-与-linux-注入技术对照表)

---

## Windows 远程注入

远程注入本质：**把一段自定义代码（DLL / Shellcode），强制写入另一个独立进程的地址空间，并让目标进程执行这段代码**。Windows 下经典远程注入方案主要分为 DLL 注入、原生 Shellcode 注入。

### 1. 经典 CreateRemoteThread DLL 注入（最广为人知）

#### 用到的核心 Win32 API

| API | 作用 |
|---|---|
| `OpenProcess` | 打开目标进程，获取进程句柄；需要 `PROCESS_VM_OPERATION`、`PROCESS_VM_WRITE`、`PROCESS_CREATE_THREAD` 权限 |
| `VirtualAllocEx` | 在**远程进程的虚拟地址空间**分配一块内存，用来存放 DLL 路径字符串 |
| `WriteProcessMemory` | 将 DLL 文件路径字符串，写入刚才远程分配的内存 |
| `GetProcAddress` | 获取 `LoadLibraryA/W` 函数地址（`LoadLibrary` 在 kernel32.dll，所有进程共享，地址全局一致） |
| `CreateRemoteThread` | 在远程进程内部创建一个线程，线程入口直接指向 `LoadLibrary`，参数就是远程内存里存放的 DLL 路径 |
| `WaitForSingleObject` | 等待远程线程执行完成（可选） |
| `CloseHandle` | 关闭进程句柄、线程句柄，清理资源 |

#### 原理

1. 每个 Windows 进程拥有独立虚拟地址空间，默认互相隔离，不能直接读写别的进程内存、执行代码。
2. `kernel32.dll` 是系统模块，**所有进程加载的 kernel32 在同一系统版本下，函数基址相同**。
3. 不需要把 `LoadLibrary` 注入进去，只需要让远程进程调用它：
   - 远程线程启动 → 调用 `LoadLibrary("xxx.dll")`
   - `LoadLibrary` 在目标进程上下文执行，**目标进程就会加载你的 DLL 到自身地址空间**
4. DLL 一旦被目标进程加载，`DllMain` 入口函数就会执行，自定义代码就在目标进程里运行。

> 缺点：现代反作弊非常容易检测 `CreateRemoteThread`，这个 API 特征明显。

### 2. 进阶：不使用 CreateRemoteThread 的注入（规避检测，外挂常用变种）

#### 1）QueueUserAPC 注入（APC 注入）

核心 API 调用链：

```text
OpenProcess → VirtualAllocEx → WriteProcessMemory（写入 shellcode）
→ OpenThread（获取目标进程内的线程句柄）→ QueueUserAPC
```

原理：APC（异步过程调用），Windows 线程有 APC 队列。当线程进入**可警告等待状态**时，系统会自动执行 APC 回调函数。把 shellcode 地址作为 APC 回调，排队到目标线程，等线程休眠 / 等待时自动执行 shellcode。**不新建线程，规避 CreateRemoteThread 特征**。

#### 2）RtlCreateUserThread 注入

`CreateRemoteThread` 底层内部调用 `RtlCreateUserThread`。部分旧反作弊只 hook `CreateRemoteThread` 但不钩底层，会直接调用这个未公开的 Native API。

> Nt 系列 API（Native API：`NtOpenProcess`、`NtWriteVirtualMemory`、`NtCreateThreadEx`）位于 `ntdll.dll`，很多注入会绕开 Win32 封装直接调用 Native API，进一步规避 Hook。

#### 3）Shellcode 直接注入（不依赖 DLL）

不再注入 DLL，直接把机器码（shellcode）写入远程进程内存，然后触发执行。

API 同上，区别：`WriteProcessMemory` 写入的不是 DLL 路径，而是编译好的二进制 shellcode，直接执行，不需要 `LoadLibrary`。

### 3. 配套关键 API（内存操作通用）

| API | 作用 |
|---|---|
| `VirtualProtectEx` | 修改远程内存页属性（`PAGE_READWRITE` 写入，写入后改成 `PAGE_EXECUTE_READ`，标记为可执行，执行 shellcode 必须） |
| `ReadProcessMemory` | 读取远程进程内存（外挂用来读游戏内存数据） |

### 4. Windows 底层原理总结

1. **进程地址空间隔离**：用户态进程默认无法访问其他进程内存，必须通过系统提供的跨进程内存 API，这些 API 会经过内核态校验权限。
2. **模块基址共享**：系统 DLL（kernel32、ntdll）在同版本 Windows 下，所有进程加载基址一致，所以可以直接拿到 `LoadLibrary` 这类函数地址给远程进程调用。
3. **代码执行触发方式**：注入分为两步：
   - ① 代码 / 数据写入目标进程内存；
   - ② 触发目标进程 CPU 跳转到这段代码执行（新建线程、APC、劫持已有线程等）。
4. **DllMain 入口**：DLL 被 `LoadLibrary` 加载时，系统自动调用 `DllMain`，这是注入代码的入口点。

### 5. Windows 现代防护手段

1. Hook `CreateRemoteThread`、`NtCreateThreadEx`、`QueueUserAPC` 等 API，监控调用。
2. 监控 `LoadLibrary` 加载未知 DLL，检查 DLL 签名、导入表。
3. 内核驱动监控跨进程内存写入、远程内存分配行为。
4. 进程内存扫描，检测可执行页内的可疑 shellcode。

---

## Linux 远程注入

Linux 没有 Windows 那种原生"远程线程"API，注入思路和 Windows 差异很大。Linux 进程拥有独立虚拟地址空间，传统用户态不能直接读写其他进程内存。Linux 最核心用于跨进程内存读写的系统调用：`ptrace`（进程跟踪，调试器底层接口，gdb / strace 都基于它）。Linux 没有 `LoadLibrary`；Linux 动态库是 `.so`，加载函数为 `dlopen()`。

### 1. 经典 Linux 注入：ptrace 注入（最主流）

目标：向目标进程注入 `.so` 动态库，或者直接注入 shellcode。

#### 用到的关键系统调用（syscall）

| 系统调用 | 作用 |
|---|---|
| `ptrace(PTRACE_ATTACH, pid)` | 附加到目标进程，暂停目标进程；获取调试控制权 |
| `waitpid(pid, &status, 0)` | 等待目标进程被暂停，attach 之后必须 waitpid |
| `ptrace(PTRACE_GETREGS, pid, NULL, &regs)` | 读取目标进程当前 CPU 寄存器（RIP / RSP 等），保存现场 |
| `ptrace(PTRACE_SETREGS, pid, NULL, &regs)` | 修改目标进程寄存器，修改指令指针 RIP，劫持执行流 |
| `ptrace(PTRACE_PEEKDATA, pid, addr, 0)` | 读取远程进程内存 |
| `ptrace(PTRACE_POKEDATA, pid, addr, data)` | 写入远程进程内存（写入 shellcode / dlopen 字符串） |
| `ptrace(PTRACE_CONT, pid, NULL, NULL)` | 恢复进程继续执行 |
| `ptrace(PTRACE_DETACH, pid, NULL, NULL)` | 解除调试附加，交还进程控制权 |

> **注入 so 的核心思路：**
>
> 1. attach 目标进程，暂停它
> 2. 在目标进程栈 / 内存写入 so 路径字符串
> 3. 劫持指令指针，让目标进程调用 `dlopen()`，加载目标 `.so`
> 4. `dlopen` 在目标进程上下文执行，so 被加载，执行 `_init` / `constructor` 函数（Linux 下没有 DllMain，用构造函数 `__attribute__((constructor))`）
> 5. 恢复原始寄存器，detach，目标进程继续运行，so 常驻进程内

关键点：`dlopen` 是 libc 函数，**同一 libc 版本下，进程内 libc 地址在 ASLR 关闭时固定；开启 ASLR 后需要先读取目标进程 libc 基址**。

### 2. 不使用 ptrace 的注入方案（规避 ptrace 检测，root 权限）

ptrace 特征极强，现代程序、沙箱会监控 `PTRACE_ATTACH`。下面是 Linux 下常见替代方案：

#### 1）/proc/pid/mem 直接读写（Linux 3.2+）

不需要 ptrace attach，直接读写进程内存文件 `/proc/<pid>/mem`。

用到：

- `open("/proc/%d/mem", O_RDWR)`
- `pread64()` / `pwrite64()`：读写指定虚拟地址内存

限制：

- 只能读写，**不能直接修改执行流**，一般要配合其他方式劫持指令指针；
- 权限严格，很多发行版加固后限制访问 mem。

#### 2）LD_PRELOAD 预加载注入（最简单，但有使用条件）

不属于"远程注入"，是**启动时注入**。

原理：Linux 动态链接器 `ld.so`，在程序启动前优先加载 `LD_PRELOAD` 指定的 so，so 的 constructor 函数会在主程序执行前运行。

```bash
LD_PRELOAD="./hack.so" ./target_program
```

- ✅ 优点：最简单，不需要 ptrace；
- ❌ 缺点：**只能在进程启动前生效，不能注入已经运行的进程**；子进程会继承这个环境变量。

#### 3）内核模块注入（root 权限）

编写内核模块，在内核层修改进程页表，写入代码，修改 `task_struct`，劫持执行流。

> 现代内核（5.x+）开启 lockdown、eBPF 限制，内核模块签名，很难加载未签名 ko。

#### 4）eBPF 注入（现代 Linux）

eBPF 可以挂载 kprobe 跟踪进程，修改内存；但高版本内核安全策略限制很大，多用于观测，不适合传统注入。

#### 5）信号劫持注入

利用进程信号处理机制，发送信号（SIGUSR 等），进程收到信号进入信号处理上下文，临时修改栈与寄存器，执行 shellcode。比较少见。

### 3. 配套重要系统调用 / 函数

| 接口 | 作用 |
|---|---|
| `dlopen()` | Linux 动态库加载函数，对应 Windows LoadLibrary |
| `dlsym()` | 获取 so 内函数地址，对应 GetProcAddress |
| `dlclose()` | 卸载动态库 |
| `mmap()` | 远程分配内存（需要 ptrace + pokedata 调用 mmap syscall），对应 Windows VirtualAllocEx |
| `mprotect()` | 修改内存页权限 `PROT_READ \| PROT_WRITE \| PROT_EXEC`，对应 VirtualProtectEx |

> Linux 没有单独"分配远程内存"的系统调用。想要在目标进程分配内存，一般通过 ptrace 劫持，让目标进程自己调用 `mmap`。

### 4. Linux 注入核心原理总结

1. **地址空间隔离**：和 Windows 类似，每个进程拥有独立虚拟地址空间。用户态默认不能访问其他进程内存。
2. **ptrace 本质**：内核提供调试接口，attach 后内核接管进程，可以读写内存、修改寄存器，暂停 / 恢复进程。
3. **ASLR（地址空间布局随机化）**：现代 Linux 默认开启 ASLR，libc、堆、栈基址每次启动随机。注入时必须先泄露 libc 基址，才能拿到 `dlopen` 真实地址。
4. **SO 入口点**：Linux 没有 DllMain。`.so` 加载时自动运行标记 `__attribute__((constructor))` 的函数，作为注入入口。
5. **执行流劫持**：Linux 没有远程线程。注入代码不是新建线程，而是**暂停现有线程，修改寄存器 RIP，临时让它执行我们的代码，执行完再恢复现场**。

### 5. Linux 现代防护手段

1. 检测 ptrace attach 行为，监控 `/proc` 访问。
2. YAMA ptrace 限制（`/proc/sys/kernel/yama/ptrace_scope`），默认不允许非父进程 ptrace 附加。
3. ASLR、栈保护、RELRO，防止地址泄露。
4. 内核 lockdown，禁止加载未签名内核模块。
5. AppArmor / SELinux 限制进程内存访问。

---

## Windows 与 Linux 注入技术对照表

| Windows | Linux |
|---|---|
| `CreateRemoteThread` DLL 注入 | `ptrace` + `dlopen` so 注入 |
| `LoadLibraryA/W` | `dlopen()` |
| `GetProcAddress` | `dlsym()` |
| `VirtualAllocEx` | ptrace 劫持调用 `mmap` syscall |
| `WriteProcessMemory` | `ptrace(PTRACE_POKEDATA)` / `pwrite64(/proc/pid/mem)` |
| `VirtualProtectEx` | `mprotect()`（远程调用） |
| `ReadProcessMemory` | `ptrace(PTRACE_PEEKDATA)` / `pread64(/proc/pid/mem)` |
| `DllMain` | `__attribute__((constructor))` |
| `QueueUserAPC` | 信号劫持 / kprobe / eBPF |

---

> **再次提醒**：上述技术**仅操作系统原理学习用途**。任何将其用于游戏外挂、恶意程序的行为都属于违法行为。游戏反作弊会对这类注入行为进行检测，一旦使用会导致账号封禁，情节严重将承担法律责任。
