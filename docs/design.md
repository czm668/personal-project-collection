# 设备巡检系统 — 实现计划

## Context（背景）

项目目录为空，需要根据《设备巡检系统项目需求说明书》从零实现一套基于 C 语言的设备巡检管理系统。系统采用控制台交互，使用链表、链队列与文件持久化，支持管理员与工程师两类角色，覆盖工程师管理、设备管理、异常设备生成、巡检维护、多端登录在线状态判定等功能。

## 技术选型

- **语言**：C99，使用标准库，兼容 Windows（MinGW）与 Linux
- **编译**：提供 Makefile（Linux）与 compile.bat（Windows）
- **文件格式**：管道符 `|` 分隔的文本文件，一行一条记录
- **安全写入**：先写 `.tmp` 临时文件，再 rename 替换目标文件

## 文件结构

```
设备巡检项目/
├── src/
│   ├── common.h          — 公共定义：数据结构、宏、枚举、工具函数声明
│   ├── common.c          — 工具函数：时间格式化、安全文件写入、输入读取、终端ID生成
│   ├── user_manager.h    — 工程师管理接口声明
│   ├── user_manager.c    — 工程师增删改查、登录校验
│   ├── device_manager.h  — 设备管理接口声明
│   ├── device_manager.c  — 设备增删改查、异常设备生成
│   ├── maintenance.h     — 巡检维护与队列接口声明
│   ├── maintenance.c     — 待维护队列操作、巡检流程、维护流程、维护记录管理
│   ├── session.h         — 会话管理接口声明
│   ├── session.c         — 会话记录、在线状态判定、过期清理
│   ├── persistence.h     — 持久化接口声明
│   ├── persistence.c    — 所有数据文件的加载/保存
│   └── main.c            — 入口：主菜单、管理员菜单、工程师菜单、登录流程
├── data/                 — 数据文件目录（运行时自动创建）
├── Makefile              — Linux 编译脚本
├── compile.bat           — Windows 编译脚本
└── README.md             — 编译与运行说明
```

## 数据结构设计

### 1. 工程师（单链表）
```c
typedef struct EngineerNode {
    char id[20];          // 工号（主键）
    char name[50];        // 姓名
    char password[50];    // 密码
    struct EngineerNode* next;
} EngineerNode;
```
在线状态不存于此结构，由会话记录聚合得出。

### 2. 设备（单链表）
```c
typedef struct DeviceNode {
    char id[20];              // 设备编号（主键）
    char name[50];            // 设备名称
    char type[30];            // 设备类型
    char location[50];       // 所在位置
    int status;               // 0=正常, 1=异常
    char lastMaintTime[30];   // 最近维护时间
    char lastMaintEngId[20];  // 最近维护人工号
    char lastMaintEngName[50]; // 最近维护人姓名
    int maintCount;           // 维护次数
    struct DeviceNode* next;
} DeviceNode;
```

### 3. 待维护设备（链队列）
```c
typedef struct QueueNode {
    char deviceId[20];       // 设备编号（索引到设备链表）
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} MaintenanceQueue;
```

### 4. 维护记录（单链表）
```c
typedef struct MaintRecordNode {
    char deviceId[20];       // 设备编号
    char engId[20];          // 维护人工号
    char engName[50];        // 维护人姓名
    char maintTime[30];      // 维护时间
    char description[200];   // 维护说明
    int beforeStatus;        // 维护前状态
    int afterStatus;         // 维护后状态
    struct MaintRecordNode* next;
} MaintRecordNode;
```

### 5. 会话记录（单链表）
```c
typedef struct SessionNode {
    char engId[20];          // 工号
    char terminalId[50];     // 终端标识
    char loginTime[30];      // 登录时间（可读格式）
    time_t loginTimestamp;   // 登录时间戳
    time_t lastActive;      // 最后活跃时间戳
    int ended;               // 0=活跃, 1=已结束
    struct SessionNode* next;
} SessionNode;
```

## 关键设计决策

### 管理员账号
- 硬编码默认管理员账号：`admin` / `admin123`
- 管理员信息不存入工程师文件，仅在程序中校验
- 管理员不支持多端登录（单端即可）

### 多端登录与会话有效期
- 每次工程师登录成功 → 追加一条会话条目（含终端ID、登录时间、时间戳）
- 终端ID = `进程PID + 时间戳`，保证多终端唯一
- 注销或程序正常退出 → 标记对应会话为已结束
- 在线判定：扫描会话记录，存在至少一条未结束且 lastActive 在 30 分钟内的会话 → 在线
- 工程师执行任意操作 → 刷新 lastActive 时间
- 程序退出时（atexit 注册）→ 将当前终端的所有活跃会话标记为已结束

### 异常设备生成（15%）
- 以自然日为周期，当日只触发一次
- 目标数量 = 设备总数 × 15%，向上取整
- 从"正常"设备中随机抽取，置为"异常"
- 正常设备不足时全部置为异常并记录日志
- 生成日期写入系统状态文件，程序重启后保持一致
- 触发时机：管理员登录时检查 + 管理员菜单显式入口

### 文件安全写入
- 写入时先写 `filename.tmp`
- 写完后 rename 替换目标文件
- 读取时若检测到 `.tmp` 残留则删除（上次写入未完成）

### 数据文件（data/ 目录）
| 文件 | 内容 |
|------|------|
| engineers.dat | 全部工程师账号 |
| devices.dat | 全部设备信息 |
| queue.dat | 待维护设备编号及顺序 |
| records.dat | 历史维护明细 |
| sessions.dat | 登录会话记录 |
| system_state.dat | 当日异常生成日期 |

## 菜单结构

```
主菜单
├── 1. 登录
│   ├── 管理员登录 → 管理员菜单
│   └── 工程师登录 → 工程师菜单
└── 0. 退出系统

管理员菜单
├── 1. 工程师信息管理（增/删/改/查/列出全部）
├── 2. 查看工程师在线状态
├── 3. 设备信息管理（增/删/改/查/列出全部）
├── 4. 查看待维护队列
├── 5. 查看维护记录（按设备/按工程师/按时间）
├── 6. 生成当日异常设备
├── 7. 查看所有设备列表
└── 0. 注销登录

工程师菜单
├── 1. 设备巡检
├── 2. 设备信息管理（增/删/改/查/列出全部）
├── 3. 设备维护（从队列取出并维护）
├── 4. 查看待维护队列
├── 5. 查看维护记录（按设备/按工程师/按时间）
├── 6. 查看所有设备列表
└── 0. 注销登录
```

## 实现步骤

### Step 1: 公共定义与工具函数
- `common.h`：所有数据结构定义、宏（MAX_*、文件路径、会话有效期）、枚举（设备状态、角色）
- `common.c`：getCurrentTime、formatTime、safeWriteFile、readLine、clearInputBuffer、generateTerminalId、createDirectory

### Step 2: 持久化模块
- `persistence.h/c`：loadEngineers、saveEngineers、loadDevices、saveDevices、loadQueue、saveQueue、loadRecords、saveRecords、loadSessions、saveSessions、loadSystemState、saveSystemState
- 每个函数：打开文件 → 逐行解析 → 重建链表/队列；或遍历链表 → 写文件

### Step 3: 用户管理模块
- `user_manager.h/c`：工程师链表的增删改查、登录校验（含会话创建）、注销（标记会话结束）
- 管理员账号校验（硬编码）

### Step 4: 会话管理模块
- `session.h/c`：createSession、endSession、refreshSession、isEngineerOnline、getOnlineCount、getActiveSessionCount、cleanupExpiredSessions

### Step 5: 设备管理模块
- `device_manager.h/c`：设备链表增删改查、generateAbnormalDevices（15%随机抽取）、markDeviceStatus、查询按编号/名称/状态/全量

### Step 6: 巡检维护模块
- `maintenance.h/c`：队列初始化、入队、出队、遍历队列、巡检流程（逐台检查）、维护流程（出队→回写状态→追加维护记录）、维护记录查询（按设备/工程师/时间范围）

### Step 7: 主程序
- `main.c`：全局数据指针初始化、数据加载、菜单循环、登录流程、各菜单分支调用对应模块、退出时保存数据并结束会话
- atexit 注册清理函数

### Step 8: 编译脚本与说明
- `Makefile`（Linux: gcc）
- `compile.bat`（Windows: gcc/MinGW）
- `README.md`：编译与运行说明

## 验收测试方案

按需求文档第八节验收要点，逐一验证：
1. 管理员新增工程师与设备 → 重启程序 → 数据仍在
2. 同一工程师在两个终端登录 → 管理员看到在线且端数=2
3. 一端注销 → 仍在线；两端注销 → 离线
4. 生成当日异常设备 → 数量≈15% → 出现在待维护队列
5. 工程师巡检 → 维护异常设备 → 状态恢复正常
6. 查询维护记录 → 显示维护人、时间、说明
7. 程序重启 → 待维护队列中未处理设备仍保留
