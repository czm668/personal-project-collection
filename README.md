# 设备巡检系统（C）

基于 **C99** 的设备巡检管理工具：**管理员端与工程师端双程序**，采用单链表 / 链队列 + 文件持久化，支持同一账号多终端登录的在线状态判定与双端数据自动同步。

## 功能

- **用户管理**：管理员对工程师账号增删改查，查看在线状态
- **设备管理**：管理员与工程师均可对设备信息增删改查
- **异常设备生成**：每日随机产生 **15%** 的异常设备，自动加入待维护队列
- **巡检维护**：工程师逐一巡检设备，对异常设备执行维护，状态回写为正常
- **维护记录追溯**：记录每台设备的维护历史（维护人、时间、说明）
- **多端登录**：同一工程师账号可在多终端同时登录，会话聚合判定在线状态

## 编译与运行

系统分为**管理员端**与**工程师端**两个独立程序，可同时运行（双开），共享 `data/` 目录数据并自动同步。

### Windows 一键编译（推荐）

**双击 `compile.bat`**（或在仓库根目录执行），自动编译出 `admin.exe` 与 `engineer.exe`：

```bat
compile.bat
```

> ⚠️ 仓库不包含编译产物（`*.exe` 已被 `.gitignore` 排除），**必须先编译再运行**。
> 脚本以 GBK 编码保存、命令写成单行（不使用 `^` 续行符），兼容中文 Windows（代码页 936）。

或手动编译（单行命令）：

```bat
gcc -Wall -Wextra -std=c99 -g -DAPP_ADMIN -o admin.exe src\main.c src\common.c src\persistence.c src\session.c src\user_manager.c src\device_manager.c src\maintenance.c
gcc -Wall -Wextra -std=c99 -g -DAPP_ENGINEER -o engineer.exe src\main.c src\common.c src\persistence.c src\session.c src\user_manager.c src\device_manager.c src\maintenance.c
```

### Linux / 已安装 make

```bash
make
```

> Windows 上若未安装 `make`（本机默认未安装），用上面的 `compile.bat` 或 gcc 命令即可。

### 运行

```bat
run.bat                     :: Windows：自动双开管理员端与工程师端
```

```bash
./admin && ./engineer       # Linux：同样双开
```

> 请在仓库根目录下运行（程序会在当前目录创建 `data/`）；`run.bat` 会自动切到脚本所在目录。

> 已用 MinGW-W64 gcc 16.1 以 `-Wall -Wextra` 验证：admin 与 engineer 两个目标均**零警告编译通过**。

## 设计要点

- **双端数据同步**：每次菜单循环前从磁盘重新加载（reload），所有写操作即时落盘；程序退出时先重载磁盘数据，仅更新本终端会话，不覆盖其他端的修改
- **多终端在线判定**：终端 ID = 进程 PID + 时间戳（保证多终端唯一）；会话记录聚合判定在线（30 分钟有效期 + 操作刷新活跃时间）；账号被管理员删除时工程师端自动强制注销
- **原子写**：先写 `.tmp` 临时文件再 `rename` 替换目标文件，读取时清理残留临时文件
- **异常设备生成**：以自然日为周期幂等触发，目标数量 = 设备总数 × 15% 向上取整，生成日期写入状态文件，重启后不重复生成
- **跨平台**：C99 + 标准库，Windows（MinGW）与 Linux 双平台编译脚本

## 数据结构

| 实体 | 实现 |
|---|---|
| 工程师信息 | 单链表 + 文件 |
| 设备信息 | 单链表 + 文件 |
| 待维护设备 | 链队列（FIFO）+ 文件 |
| 维护记录 | 单链表 + 文件（追加式） |
| 会话记录 | 单链表 + 文件（聚合判定在线状态） |

## 目录结构

```
src/
  common.h/c            公共定义与工具函数
  persistence.h/c       文件持久化（加载 / 保存）
  session.h/c           会话管理与在线判定
  user_manager.h/c      工程师增删改查与登录
  device_manager.h/c    设备管理与异常设备生成
  maintenance.h/c       巡检维护与队列操作
  main.c                程序入口与菜单
docs/design.md          设计说明（数据结构、关键决策、验收方案）
Makefile                Linux / 已安装 make 时使用
compile.bat             Windows 一键编译（管理员端 + 工程师端）
run.bat                 Windows 一键运行（双开两端）
```

运行时在 `data/` 目录自动创建：`engineers.dat`、`devices.dat`、`queue.dat`、`records.dat`、`sessions.dat`、`system_state.dat`（管道符分隔的文本，一行一条记录）

## 默认账号

| 角色 | 账号 | 密码 |
|---|---|---|
| 管理员 | admin | admin123 |

工程师账号由管理员创建。

## 验收要点

管理员新增工程师与设备 → 重启程序数据仍在；同一工程师双终端登录 → 管理员看到在线且端数 = 2；一端注销仍在线、两端注销才离线；生成异常设备数量 ≈ 15% 且出现在待维护队列；工程师巡检维护后状态回写为正常；维护记录可按设备 / 工程师 / 时间查询；程序重启后待维护队列中未处理设备仍保留。