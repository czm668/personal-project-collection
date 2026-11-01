/**
 * @file common.c
 * @brief 工具函数实现：时间格式化、安全文件写入、输入读取、终端ID生成
 */
#include "common.h"

#include <errno.h>

#ifdef _WIN32
#include <direct.h>
#include <process.h>
#include <io.h>
#include <conio.h>
#define MKDIR(path) _mkdir(path)
#define GETPID() _getpid()
#define ISATTY(fd) _isatty(fd)
#else
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#define MKDIR(path) mkdir(path, 0755)
#define GETPID() getpid()
#define ISATTY(fd) isatty(fd)
#endif

/* 全局变量定义 */
EngineerNode *g_engineers = NULL;
DeviceNode *g_devices = NULL;
MaintenanceQueue g_queue = {NULL, NULL, 0};
MaintRecordNode *g_records = NULL;
SessionNode *g_sessions = NULL;
SystemState g_sysState = {"", 0};
char g_terminalId[MAX_TERM_LEN] = "";

/* ======================== 时间相关 ======================== */

void getCurrentTime(char *buf, int bufSize)
{
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    strftime(buf, bufSize, "%Y-%m-%d %H:%M:%S", lt);
}

void getCurrentDate(char *buf, int bufSize)
{
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    strftime(buf, bufSize, "%Y-%m-%d", lt);
}

time_t getCurrentTimestamp(void)
{
    return time(NULL);
}

void formatTimestamp(time_t ts, char *buf, int bufSize)
{
    struct tm *lt = localtime(&ts);
    strftime(buf, bufSize, "%Y-%m-%d %H:%M:%S", lt);
}

/* ======================== 输入相关 ======================== */

void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int readLine(char *buf, int bufSize)
{
    if (fgets(buf, bufSize, stdin) == NULL) {
        buf[0] = '\0';
        return 0;
    }
    /* 检测输入是否被截断（未读到换行符说明输入过长） */
    int len = strlen(buf);
    if (len > 0 && buf[len - 1] != '\n') {
        /* 输入超出缓冲区，丢弃剩余部分 */
        clearInputBuffer();
        printf("  提示：输入过长已截断，仅保留前 %d 字符。\n", bufSize - 2);
    }
    trimNewline(buf);
    trim(buf);
    /* 过滤管道符 |，防止破坏文件格式 */
    int filtered = 0;
    for (int i = 0; buf[i] != '\0'; i++) {
        if (buf[i] == DELIMITER) {
            buf[i] = '_';
            filtered = 1;
        }
    }
    if (filtered) {
        printf("  提示：输入包含分隔符 '|'，已替换为 '_'。\n");
    }
    return strlen(buf) > 0;
}

int readInt(const char *prompt)
{
    char buf[MAX_LINE_LEN];
    int val;
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            /* EOF（输入耗尽），返回 0 避免无限循环 */
            printf("\n输入结束，返回 0。\n");
            return 0;
        }
        trimNewline(buf);
        trim(buf);
        if (strlen(buf) == 0) {
            printf("输入不能为空，请重新输入。\n");
            continue;
        }
        /* 检查是否全是数字 */
        int valid = 1;
        for (int i = 0; buf[i] != '\0'; i++) {
            if (i == 0 && buf[0] == '-') continue;
            if (buf[i] < '0' || buf[i] > '9') {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("输入无效，请输入一个整数。\n");
            continue;
        }
        val = atoi(buf);
        return val;
    }
}

void trimNewline(char *str)
{
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}

void trim(char *str)
{
    /* 去除尾部空格 */
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t')) {
        str[--len] = '\0';
    }
    /* 去除头部空格 */
    char *start = str;
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

/* ======================== 密码掩码输入 ======================== */

void readPassword(char *buf, int bufSize)
{
    /* 非终端环境（管道/重定向）回退为普通读取 */
    int isTTY = ISATTY(fileno(stdin));
    if (!isTTY) {
        readLine(buf, bufSize);
        return;
    }

#ifdef _WIN32
    /* Windows: 用 _getch() 逐字符读取，不回显 */
    int i = 0;
    int c;
    while (i < bufSize - 1) {
        c = _getch();
        if (c == '\r' || c == '\n') {
            break;
        }
        if (c == 8 || c == 127) {
            /* 退格 */
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (c == 3 || c == 26) {
            /* Ctrl+C / Ctrl+Z */
            printf("\n");
            exit(1);
        } else if (c >= 32 && c <= 126) {
            buf[i++] = (char)c;
            printf("*");
        }
    }
    buf[i] = '\0';
    printf("\n");
#else
    /* Linux: 用 termios 关闭回显 */
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    if (fgets(buf, bufSize, stdin) == NULL) {
        buf[0] = '\0';
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    trimNewline(buf);
    trim(buf);
    printf("\n");
#endif
}

/* ======================== 密码哈希 ======================== */

void hashPassword(const char *input, char *output, int outSize)
{
    /* 简单 XOR + hex 编码，非加密级安全，仅防止明文直接可见 */
    int i;
    for (i = 0; input[i] != '\0' && (i + 1) * 2 < outSize; i++) {
        unsigned char c = (unsigned char)input[i] ^ 0x5A;
        sprintf(output + i * 2, "%02x", c);
    }
    output[i * 2] = '\0';
}

int verifyPassword(const char *input, const char *hashed)
{
    char buf[MAX_PWD_LEN * 2];
    hashPassword(input, buf, sizeof(buf));
    return strcmp(buf, hashed) == 0;
}

/* ======================== 文件 I/O ======================== */

int safeWriteFile(const char *filepath, const char *content)
{
    char tmpPath[MAX_LINE_LEN + 8];
    FILE *fp;

    snprintf(tmpPath, sizeof(tmpPath), "%s.tmp", filepath);

    fp = fopen(tmpPath, "w");
    if (fp == NULL) {
        return 0;
    }
    fputs(content, fp);
    fclose(fp);

    /* 删除原文件，重命名临时文件 */
    remove(filepath);
    if (rename(tmpPath, filepath) != 0) {
        remove(tmpPath);
        return 0;
    }
    return 1;
}

int ensureDataDir(void)
{
    if (MKDIR(DATA_DIR) == 0) return 1;
    /* 目录已存在 (EEXIST) 也视为成功 */
    if (errno == EEXIST) return 1;
    return 0;
}

void cleanTmpFiles(void)
{
    /* 清理可能残留的临时文件 */
    char paths[][MAX_LINE_LEN] = {
        FILE_ENGINEERS, FILE_DEVICES, FILE_QUEUE,
        FILE_RECORDS, FILE_SESSIONS, FILE_SYS_STATE
    };
    char tmpPath[MAX_LINE_LEN + 8];
    for (int i = 0; i < 6; i++) {
        snprintf(tmpPath, sizeof(tmpPath), "%s.tmp", paths[i]);
        remove(tmpPath);
    }
}

/* ======================== 字段处理 ======================== */

void copyField(char *dest, int destSize, const char *src)
{
    strncpy(dest, src, destSize - 1);
    dest[destSize - 1] = '\0';
    trim(dest);
}

/* ======================== 终端标识 ======================== */

void generateTerminalId(char *buf, int bufSize)
{
    int pid = GETPID();
    time_t ts = time(NULL);
    snprintf(buf, bufSize, "T%d_%ld", pid, (long)ts);
}

/* ======================== 显示相关 ======================== */

void printHeader(const char *title)
{
    printf("\n");
    printSeparator();
    printf("  %s\n", title);
    printSeparator();
}

void printSeparator(void)
{
    printf("==============================================\n");
}

void pauseScreen(void)
{
    printf("\n按回车键继续...");
    clearInputBuffer();
}

/* ======================== 内存释放 ======================== */

void freeEngineers(void)
{
    EngineerNode *cur = g_engineers;
    while (cur != NULL) {
        EngineerNode *next = cur->next;
        free(cur);
        cur = next;
    }
    g_engineers = NULL;
}

void freeDevices(void)
{
    DeviceNode *cur = g_devices;
    while (cur != NULL) {
        DeviceNode *next = cur->next;
        free(cur);
        cur = next;
    }
    g_devices = NULL;
}

void freeQueue(void)
{
    QueueNode *cur = g_queue.front;
    while (cur != NULL) {
        QueueNode *next = cur->next;
        free(cur);
        cur = next;
    }
    g_queue.front = NULL;
    g_queue.rear = NULL;
    g_queue.size = 0;
}

void freeRecords(void)
{
    MaintRecordNode *cur = g_records;
    while (cur != NULL) {
        MaintRecordNode *next = cur->next;
        free(cur);
        cur = next;
    }
    g_records = NULL;
}

void freeSessions(void)
{
    SessionNode *cur = g_sessions;
    while (cur != NULL) {
        SessionNode *next = cur->next;
        free(cur);
        cur = next;
    }
    g_sessions = NULL;
}
