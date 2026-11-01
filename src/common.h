/**
 * @file common.h
 * @brief 公共定义：数据结构、宏、枚举、工具函数声明
 */
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ======================== 宏定义 ======================== */
#define MAX_ID_LEN        20
#define MAX_NAME_LEN      50
#define MAX_PWD_LEN       50
#define MAX_TYPE_LEN      30
#define MAX_LOC_LEN       50
#define MAX_DESC_LEN      200
#define MAX_TIME_LEN      30
#define MAX_TERM_LEN      50
#define MAX_LINE_LEN      1024
#define MAX_FIELD_LEN     256

#define SESSION_TIMEOUT   1800          /* 会话有效期 30 分钟（秒） */
#define ABNORMAL_RATIO   15             /* 异常设备占比 15% */
#define DELIMITER        '|'            /* 文件字段分隔符 */

#define DATA_DIR          "data"
#define FILE_ENGINEERS    "data/engineers.dat"
#define FILE_DEVICES      "data/devices.dat"
#define FILE_QUEUE        "data/queue.dat"
#define FILE_RECORDS      "data/records.dat"
#define FILE_SESSIONS     "data/sessions.dat"
#define FILE_SYS_STATE    "data/system_state.dat"

#define ADMIN_ID          "admin"
#define ADMIN_PWD         "admin123"

/* ======================== 枚举 ======================== */
typedef enum {
    ROLE_NONE = 0,
    ROLE_ADMIN,
    ROLE_ENGINEER
} Role;

typedef enum {
    STATUS_NORMAL = 0,
    STATUS_ABNORMAL = 1
} DeviceStatus;

/* ======================== 链表结点定义 ======================== */

/* 工程师结点（单链表） */
typedef struct EngineerNode {
    char id[MAX_ID_LEN];        /* 工号（主键） */
    char name[MAX_NAME_LEN];    /* 姓名 */
    char password[MAX_PWD_LEN]; /* 密码 */
    struct EngineerNode *next;
} EngineerNode;

/* 设备结点（单链表） */
typedef struct DeviceNode {
    char id[MAX_ID_LEN];              /* 设备编号（主键） */
    char name[MAX_NAME_LEN];          /* 设备名称 */
    char type[MAX_TYPE_LEN];         /* 设备类型 */
    char location[MAX_LOC_LEN];      /* 所在位置 */
    int  status;                      /* 0=正常, 1=异常 */
    char lastMaintTime[MAX_TIME_LEN]; /* 最近维护时间 */
    char lastMaintEngId[MAX_ID_LEN];  /* 最近维护人工号 */
    char lastMaintEngName[MAX_NAME_LEN]; /* 最近维护人姓名 */
    int  maintCount;                  /* 维护次数 */
    struct DeviceNode *next;
} DeviceNode;

/* 待维护队列结点（链队列） */
typedef struct QueueNode {
    char deviceId[MAX_ID_LEN];  /* 设备编号（索引到设备链表） */
    struct QueueNode *next;
} QueueNode;

/* 链队列结构 */
typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int size;
} MaintenanceQueue;

/* 维护记录结点（单链表） */
typedef struct MaintRecordNode {
    char deviceId[MAX_ID_LEN];    /* 设备编号 */
    char engId[MAX_ID_LEN];      /* 维护人工号 */
    char engName[MAX_NAME_LEN];  /* 维护人姓名 */
    char maintTime[MAX_TIME_LEN]; /* 维护时间 */
    char description[MAX_DESC_LEN]; /* 维护说明 */
    int  beforeStatus;            /* 维护前状态 */
    int  afterStatus;             /* 维护后状态 */
    struct MaintRecordNode *next;
} MaintRecordNode;

/* 会话记录结点（单链表） */
typedef struct SessionNode {
    char engId[MAX_ID_LEN];       /* 工号 */
    char terminalId[MAX_TERM_LEN]; /* 终端标识 */
    char loginTime[MAX_TIME_LEN]; /* 登录时间（可读格式） */
    time_t loginTimestamp;        /* 登录时间戳 */
    time_t lastActive;           /* 最后活跃时间戳 */
    int  ended;                   /* 0=活跃, 1=已结束 */
    struct SessionNode *next;
} SessionNode;

/* 系统状态 */
typedef struct {
    char lastAbnormalDate[MAX_TIME_LEN]; /* 上次生成异常设备的日期 YYYY-MM-DD */
    int  lastAbnormalCount;              /* 上次生成的异常数量 */
} SystemState;

/* ======================== 全局变量声明 ======================== */
extern EngineerNode *g_engineers;       /* 工程师链表头 */
extern DeviceNode *g_devices;           /* 设备链表头 */
extern MaintenanceQueue g_queue;        /* 待维护队列 */
extern MaintRecordNode *g_records;      /* 维护记录链表头 */
extern SessionNode *g_sessions;         /* 会话记录链表头 */
extern SystemState g_sysState;          /* 系统状态 */
extern char g_terminalId[MAX_TERM_LEN]; /* 当前终端标识 */

/* ======================== 工具函数声明 ======================== */

/* 时间相关 */
void getCurrentTime(char *buf, int bufSize);
void getCurrentDate(char *buf, int bufSize);
time_t getCurrentTimestamp(void);
void formatTimestamp(time_t ts, char *buf, int bufSize);

/* 输入相关 */
void clearInputBuffer(void);
int readLine(char *buf, int bufSize);       /* 过滤 | 字符，返回是否非空 */
int readInt(const char *prompt);
void trimNewline(char *str);
void trim(char *str);

/* 密码掩码输入 */
void readPassword(char *buf, int bufSize);

/* 密码哈希（简单 XOR + hex 编码，非加密级安全） */
void hashPassword(const char *input, char *output, int outSize);
int verifyPassword(const char *input, const char *hashed);

/* 文件相关 */
int safeWriteFile(const char *filepath, const char *content);
int ensureDataDir(void);
void cleanTmpFiles(void);

/* 终端标识 */
void generateTerminalId(char *buf, int bufSize);

/* 字段处理 */
void copyField(char *dest, int destSize, const char *src);

/* 显示相关 */
void printHeader(const char *title);
void printSeparator(void);
void pauseScreen(void);

/* 内存释放 */
void freeEngineers(void);
void freeDevices(void);
void freeQueue(void);
void freeRecords(void);
void freeSessions(void);

#endif /* COMMON_H */
