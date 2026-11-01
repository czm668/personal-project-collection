/**
 * @file persistence.c
 * @brief 持久化模块实现：所有数据文件的加载与保存
 *
 * 文件格式：管道符 '|' 分隔的文本，一行一条记录
 * 写入策略：先写临时文件 .tmp，再 rename 替换目标文件
 */
#include "persistence.h"

/* ======================== 内部工具函数 ======================== */

/* 按分隔符分割字符串，返回字段数 */
static int splitFields(const char *line, char delim, char fields[][MAX_FIELD_LEN], int maxFields)
{
    int count = 0;
    int pos = 0;
    int i = 0;

    while (line[i] != '\0' && count < maxFields) {
        if (line[i] == delim) {
            fields[count][pos] = '\0';
            count++;
            pos = 0;
        } else {
            if (pos < MAX_FIELD_LEN - 1) {
                fields[count][pos++] = line[i];
            }
        }
        i++;
    }
    if (count < maxFields) {
        fields[count][pos] = '\0';
        count++;
    }
    return count;
}

/* ======================== 初始化 ======================== */

int persistenceInit(void)
{
    if (!ensureDataDir()) {
        printf("警告：无法创建数据目录 %s\n", DATA_DIR);
        return 0;
    }
    cleanTmpFiles();
    return 1;
}

/* ======================== 工程师数据 ======================== */

int loadEngineers(void)
{
    FILE *fp = fopen(FILE_ENGINEERS, "r");
    if (fp == NULL) return 1; /* 文件不存在视为正常（首次运行） */

    char line[MAX_LINE_LEN];
    int lineNum = 0;
    EngineerNode *tail = NULL;

    while (fgets(line, sizeof(line), fp) != NULL) {
        lineNum++;
        trimNewline(line);
        trim(line);
        if (strlen(line) == 0) continue;

        char fields[3][MAX_FIELD_LEN];
        int nf = splitFields(line, DELIMITER, fields, 3);
        if (nf < 3) {
            printf("警告：工程师文件第 %d 行格式错误，已跳过。\n", lineNum);
            continue;
        }

        EngineerNode *node = (EngineerNode *)malloc(sizeof(EngineerNode));
        if (node == NULL) {
            fclose(fp);
            return 0;
        }
        memset(node, 0, sizeof(EngineerNode));
        copyField(node->id, MAX_ID_LEN, fields[0]);
        copyField(node->name, MAX_NAME_LEN, fields[1]);
        copyField(node->password, MAX_PWD_LEN, fields[2]);
        node->next = NULL;

        if (g_engineers == NULL) {
            g_engineers = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    fclose(fp);
    return 1;
}

int saveEngineers(void)
{
    char buf[MAX_LINE_LEN * 4];
    buf[0] = '\0';

    EngineerNode *cur = g_engineers;
    while (cur != NULL) {
        char line[MAX_LINE_LEN];
        snprintf(line, sizeof(line), "%s|%s|%s\n",
                 cur->id, cur->name, cur->password);
        strcat(buf, line);
        cur = cur->next;
    }
    return safeWriteFile(FILE_ENGINEERS, buf);
}

/* ======================== 设备数据 ======================== */

int loadDevices(void)
{
    FILE *fp = fopen(FILE_DEVICES, "r");
    if (fp == NULL) return 1;

    char line[MAX_LINE_LEN];
    int lineNum = 0;
    DeviceNode *tail = NULL;

    while (fgets(line, sizeof(line), fp) != NULL) {
        lineNum++;
        trimNewline(line);
        trim(line);
        if (strlen(line) == 0) continue;

        char fields[9][MAX_FIELD_LEN];
        int nf = splitFields(line, DELIMITER, fields, 9);
        if (nf < 9) {
            printf("警告：设备文件第 %d 行格式错误（仅 %d 个字段），已跳过。\n", lineNum, nf);
            continue;
        }

        DeviceNode *node = (DeviceNode *)malloc(sizeof(DeviceNode));
        if (node == NULL) {
            fclose(fp);
            return 0;
        }
        memset(node, 0, sizeof(DeviceNode));
        copyField(node->id, MAX_ID_LEN, fields[0]);
        copyField(node->name, MAX_NAME_LEN, fields[1]);
        copyField(node->type, MAX_TYPE_LEN, fields[2]);
        copyField(node->location, MAX_LOC_LEN, fields[3]);
        /* 数据校验：状态值必须为 0(正常) 或 1(异常) */
        int st = atoi(fields[4]);
        node->status = (st == STATUS_ABNORMAL) ? STATUS_ABNORMAL : STATUS_NORMAL;
        copyField(node->lastMaintTime, MAX_TIME_LEN, fields[5]);
        copyField(node->lastMaintEngId, MAX_ID_LEN, fields[6]);
        copyField(node->lastMaintEngName, MAX_NAME_LEN, fields[7]);
        node->maintCount = atoi(fields[8]);
        if (node->maintCount < 0) node->maintCount = 0;
        node->next = NULL;

        if (g_devices == NULL) {
            g_devices = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    fclose(fp);
    return 1;
}

int saveDevices(void)
{
    /* 设备数量可能较多，动态分配缓冲区 */
    int count = 0;
    DeviceNode *cur = g_devices;
    while (cur != NULL) { count++; cur = cur->next; }

    if (count == 0) {
        return safeWriteFile(FILE_DEVICES, "");
    }

    int bufSize = count * MAX_LINE_LEN + 1;
    char *buf = (char *)malloc(bufSize);
    if (buf == NULL) return 0;
    buf[0] = '\0';

    cur = g_devices;
    while (cur != NULL) {
        char line[MAX_LINE_LEN];
        snprintf(line, sizeof(line), "%s|%s|%s|%s|%d|%s|%s|%s|%d\n",
                 cur->id, cur->name, cur->type, cur->location,
                 cur->status, cur->lastMaintTime, cur->lastMaintEngId,
                 cur->lastMaintEngName, cur->maintCount);
        strcat(buf, line);
        cur = cur->next;
    }
    int ret = safeWriteFile(FILE_DEVICES, buf);
    free(buf);
    return ret;
}

/* ======================== 待维护队列 ======================== */

int loadQueue(void)
{
    FILE *fp = fopen(FILE_QUEUE, "r");
    if (fp == NULL) return 1;

    char line[MAX_LINE_LEN];
    int lineNum = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        lineNum++;
        trimNewline(line);
        trim(line);
        if (strlen(line) == 0) continue;

        char fields[1][MAX_FIELD_LEN];
        int nf = splitFields(line, DELIMITER, fields, 1);
        if (nf < 1) {
            printf("警告：队列文件第 %d 行格式错误，已跳过。\n", lineNum);
            continue;
        }

        QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
        if (node == NULL) {
            fclose(fp);
            return 0;
        }
        memset(node, 0, sizeof(QueueNode));
        copyField(node->deviceId, MAX_ID_LEN, fields[0]);
        node->next = NULL;

        if (g_queue.rear == NULL) {
            g_queue.front = node;
            g_queue.rear = node;
        } else {
            g_queue.rear->next = node;
            g_queue.rear = node;
        }
        g_queue.size++;
    }
    fclose(fp);
    return 1;
}

int saveQueue(void)
{
    int bufSize = (g_queue.size + 1) * MAX_LINE_LEN;
    char *buf = (char *)malloc(bufSize);
    if (buf == NULL) return 0;
    buf[0] = '\0';

    QueueNode *cur = g_queue.front;
    while (cur != NULL) {
        char line[MAX_LINE_LEN];
        snprintf(line, sizeof(line), "%s\n", cur->deviceId);
        strcat(buf, line);
        cur = cur->next;
    }
    int ret = safeWriteFile(FILE_QUEUE, buf);
    free(buf);
    return ret;
}

/* ======================== 维护记录 ======================== */

int loadRecords(void)
{
    FILE *fp = fopen(FILE_RECORDS, "r");
    if (fp == NULL) return 1;

    char line[MAX_LINE_LEN];
    int lineNum = 0;
    MaintRecordNode *tail = NULL;

    while (fgets(line, sizeof(line), fp) != NULL) {
        lineNum++;
        trimNewline(line);
        trim(line);
        if (strlen(line) == 0) continue;

        char fields[7][MAX_FIELD_LEN];
        int nf = splitFields(line, DELIMITER, fields, 7);
        if (nf < 7) {
            printf("警告：维护记录文件第 %d 行格式错误，已跳过。\n", lineNum);
            continue;
        }

        MaintRecordNode *node = (MaintRecordNode *)malloc(sizeof(MaintRecordNode));
        if (node == NULL) {
            fclose(fp);
            return 0;
        }
        memset(node, 0, sizeof(MaintRecordNode));
        copyField(node->deviceId, MAX_ID_LEN, fields[0]);
        copyField(node->engId, MAX_ID_LEN, fields[1]);
        copyField(node->engName, MAX_NAME_LEN, fields[2]);
        copyField(node->maintTime, MAX_TIME_LEN, fields[3]);
        copyField(node->description, MAX_DESC_LEN, fields[4]);
        node->beforeStatus = atoi(fields[5]);
        node->afterStatus = atoi(fields[6]);
        node->next = NULL;

        if (g_records == NULL) {
            g_records = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    fclose(fp);
    return 1;
}

int saveRecords(void)
{
    int count = 0;
    MaintRecordNode *cur = g_records;
    while (cur != NULL) { count++; cur = cur->next; }

    if (count == 0) {
        return safeWriteFile(FILE_RECORDS, "");
    }

    int bufSize = count * MAX_LINE_LEN * 2 + 1;
    char *buf = (char *)malloc(bufSize);
    if (buf == NULL) return 0;
    buf[0] = '\0';

    cur = g_records;
    while (cur != NULL) {
        char line[MAX_LINE_LEN * 2];
        snprintf(line, sizeof(line), "%s|%s|%s|%s|%s|%d|%d\n",
                 cur->deviceId, cur->engId, cur->engName,
                 cur->maintTime, cur->description,
                 cur->beforeStatus, cur->afterStatus);
        strcat(buf, line);
        cur = cur->next;
    }
    int ret = safeWriteFile(FILE_RECORDS, buf);
    free(buf);
    return ret;
}

/* ======================== 会话记录 ======================== */

int loadSessions(void)
{
    FILE *fp = fopen(FILE_SESSIONS, "r");
    if (fp == NULL) return 1;

    char line[MAX_LINE_LEN];
    int lineNum = 0;
    SessionNode *tail = NULL;

    while (fgets(line, sizeof(line), fp) != NULL) {
        lineNum++;
        trimNewline(line);
        trim(line);
        if (strlen(line) == 0) continue;

        char fields[6][MAX_FIELD_LEN];
        int nf = splitFields(line, DELIMITER, fields, 6);
        if (nf < 6) {
            printf("警告：会话文件第 %d 行格式错误，已跳过。\n", lineNum);
            continue;
        }

        SessionNode *node = (SessionNode *)malloc(sizeof(SessionNode));
        if (node == NULL) {
            fclose(fp);
            return 0;
        }
        memset(node, 0, sizeof(SessionNode));
        copyField(node->engId, MAX_ID_LEN, fields[0]);
        copyField(node->terminalId, MAX_TERM_LEN, fields[1]);
        copyField(node->loginTime, MAX_TIME_LEN, fields[2]);
        node->loginTimestamp = (time_t)atol(fields[3]);
        node->lastActive = (time_t)atol(fields[4]);
        node->ended = atoi(fields[5]);
        node->next = NULL;

        if (g_sessions == NULL) {
            g_sessions = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    fclose(fp);
    return 1;
}

int saveSessions(void)
{
    int count = 0;
    SessionNode *cur = g_sessions;
    while (cur != NULL) { count++; cur = cur->next; }

    if (count == 0) {
        return safeWriteFile(FILE_SESSIONS, "");
    }

    int bufSize = count * MAX_LINE_LEN + 1;
    char *buf = (char *)malloc(bufSize);
    if (buf == NULL) return 0;
    buf[0] = '\0';

    cur = g_sessions;
    while (cur != NULL) {
        char line[MAX_LINE_LEN];
        snprintf(line, sizeof(line), "%s|%s|%s|%ld|%ld|%d\n",
                 cur->engId, cur->terminalId, cur->loginTime,
                 (long)cur->loginTimestamp, (long)cur->lastActive,
                 cur->ended);
        strcat(buf, line);
        cur = cur->next;
    }
    int ret = safeWriteFile(FILE_SESSIONS, buf);
    free(buf);
    return ret;
}

/* ======================== 系统状态 ======================== */

int loadSystemState(void)
{
    FILE *fp = fopen(FILE_SYS_STATE, "r");
    if (fp == NULL) return 1;

    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), fp) != NULL) {
        trimNewline(line);
        trim(line);
        char fields[2][MAX_FIELD_LEN];
        int nf = splitFields(line, DELIMITER, fields, 2);
        if (nf >= 1) {
            copyField(g_sysState.lastAbnormalDate, MAX_TIME_LEN, fields[0]);
            if (nf >= 2) {
                g_sysState.lastAbnormalCount = atoi(fields[1]);
            }
        }
    }
    fclose(fp);
    return 1;
}

int saveSystemState(void)
{
    char line[MAX_LINE_LEN];
    snprintf(line, sizeof(line), "%s|%d\n",
             g_sysState.lastAbnormalDate, g_sysState.lastAbnormalCount);
    return safeWriteFile(FILE_SYS_STATE, line);
}

/* ======================== 全部加载/保存 ======================== */

int loadAllData(void)
{
    loadEngineers();
    loadDevices();
    loadQueue();
    loadRecords();
    loadSessions();
    loadSystemState();
    return 1;
}

int saveAllData(void)
{
    int ok = 1;
    if (!saveEngineers())  ok = 0;
    if (!saveDevices())    ok = 0;
    if (!saveQueue())      ok = 0;
    if (!saveRecords())    ok = 0;
    if (!saveSessions())   ok = 0;
    if (!saveSystemState()) ok = 0;
    return ok;
}

void reloadAllData(void)
{
    freeEngineers();
    freeDevices();
    freeQueue();
    freeRecords();
    freeSessions();
    loadEngineers();
    loadDevices();
    loadQueue();
    loadRecords();
    loadSessions();
    loadSystemState();
}
