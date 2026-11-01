/**
 * @file maintenance.c
 * @brief 巡检维护模块实现：待维护队列操作、巡检流程、维护流程、维护记录管理
 */
#include "maintenance.h"
#include "device_manager.h"
#include "persistence.h"

/* ======================== 队列操作 ======================== */

void initQueue(void)
{
    freeQueue();
}

int enqueueDevice(const char *deviceId)
{
    /* 去重：检查设备是否已在队列中 */
    QueueNode *check = g_queue.front;
    while (check != NULL) {
        if (strcmp(check->deviceId, deviceId) == 0) {
            return 0; /* 已在队列中，不重复入队 */
        }
        check = check->next;
    }

    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    if (node == NULL) {
        printf("错误：内存分配失败，入队失败。\n");
        return 0;
    }
    memset(node, 0, sizeof(QueueNode));
    copyField(node->deviceId, MAX_ID_LEN, deviceId);
    node->next = NULL;

    if (g_queue.rear == NULL) {
        g_queue.front = node;
        g_queue.rear = node;
    } else {
        g_queue.rear->next = node;
        g_queue.rear = node;
    }
    g_queue.size++;
    return 1;
}

int dequeueDevice(char *outDeviceId, int outSize)
{
    if (g_queue.front == NULL) return 0;
    QueueNode *node = g_queue.front;
    copyField(outDeviceId, outSize, node->deviceId);

    g_queue.front = node->next;
    if (g_queue.front == NULL) {
        g_queue.rear = NULL;
    }
    g_queue.size--;
    free(node);
    return 1;
}

int isQueueEmpty(void)
{
    return g_queue.front == NULL;
}

int getQueueSize(void)
{
    return g_queue.size;
}

void traverseQueue(void)
{
    printHeader("待维护设备队列");
    if (isQueueEmpty()) {
        printf("  队列为空，当前无待维护设备。\n");
        printf("\n");
        return;
    }
    int idx = 1;
    QueueNode *cur = g_queue.front;
    printf("  队列中共 %d 台待维护设备（先进先出）：\n", g_queue.size);
    printf("  %-5s %-15s %-20s %-10s\n", "序号", "设备编号", "设备名称", "状态");
    printf("  --------------------------------------------------\n");
    while (cur != NULL) {
        DeviceNode *dev = findDeviceById(cur->deviceId);
        if (dev != NULL) {
            printf("  %-5d %-15s %-20s %-10s\n",
                   idx, dev->id, dev->name, dev->status == STATUS_ABNORMAL ? "异常" : "正常");
        } else {
            printf("  %-5d %-15s (设备已删除) -\n", idx, cur->deviceId);
        }
        idx++;
        cur = cur->next;
    }
    printf("\n");
}

int removeDeviceFromQueue(const char *deviceId)
{
    QueueNode *prev = NULL;
    QueueNode *cur = g_queue.front;
    while (cur != NULL) {
        if (strcmp(cur->deviceId, deviceId) == 0) {
            if (prev == NULL) {
                g_queue.front = cur->next;
            } else {
                prev->next = cur->next;
            }
            if (cur == g_queue.rear) {
                g_queue.rear = prev;
            }
            free(cur);
            g_queue.size--;
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

/* ======================== 维护记录操作 ======================== */

int addMaintenanceRecord(const char *deviceId, const char *engId,
                          const char *engName, const char *description,
                          int beforeStatus, int afterStatus)
{
    MaintRecordNode *node = (MaintRecordNode *)malloc(sizeof(MaintRecordNode));
    if (node == NULL) {
        printf("错误：内存分配失败，记录添加失败。\n");
        return 0;
    }
    memset(node, 0, sizeof(MaintRecordNode));
    copyField(node->deviceId, MAX_ID_LEN, deviceId);
    copyField(node->engId, MAX_ID_LEN, engId);
    copyField(node->engName, MAX_NAME_LEN, engName);
    getCurrentTime(node->maintTime, MAX_TIME_LEN);
    copyField(node->description, MAX_DESC_LEN, description);
    node->beforeStatus = beforeStatus;
    node->afterStatus = afterStatus;
    node->next = NULL;

    /* 追加到链表尾部（保持时间顺序） */
    if (g_records == NULL) {
        g_records = node;
    } else {
        MaintRecordNode *cur = g_records;
        while (cur->next != NULL) cur = cur->next;
        cur->next = node;
    }
    saveRecords();
    return 1;
}

static const char *statusStr(int status)
{
    return status == STATUS_ABNORMAL ? "异常" : "正常";
}

static void printRecordDetail(const MaintRecordNode *r)
{
    printf("  设备编号:   %s\n", r->deviceId);
    printf("  维护人:     %s (%s)\n", r->engName, r->engId);
    printf("  维护时间:   %s\n", r->maintTime);
    printf("  维护说明:   %s\n", r->description);
    printf("  维护前状态: %s\n", statusStr(r->beforeStatus));
    printf("  维护后状态: %s\n", statusStr(r->afterStatus));
    printf("  ----------------------------------------------\n");
}

void queryRecordsByDevice(const char *deviceId)
{
    printHeader("设备维护记录");
    int found = 0;
    MaintRecordNode *cur = g_records;
    while (cur != NULL) {
        if (strcmp(cur->deviceId, deviceId) == 0) {
            found++;
        }
        cur = cur->next;
    }
    if (found == 0) {
        printf("  设备 '%s' 无维护记录。\n", deviceId);
        printf("\n");
        return;
    }
    printf("  设备 '%s' 共 %d 条维护记录：\n\n", deviceId, found);
    cur = g_records;
    while (cur != NULL) {
        if (strcmp(cur->deviceId, deviceId) == 0) {
            printRecordDetail(cur);
        }
        cur = cur->next;
    }
}

void queryRecordsByEngineer(const char *engId)
{
    printHeader("工程师维护记录");
    int found = 0;
    MaintRecordNode *cur = g_records;
    while (cur != NULL) {
        if (strcmp(cur->engId, engId) == 0) {
            found++;
        }
        cur = cur->next;
    }
    if (found == 0) {
        printf("  工程师 '%s' 无维护记录。\n", engId);
        printf("\n");
        return;
    }
    printf("  工程师 '%s' 共 %d 条维护记录：\n\n", engId, found);
    cur = g_records;
    while (cur != NULL) {
        if (strcmp(cur->engId, engId) == 0) {
            printRecordDetail(cur);
        }
        cur = cur->next;
    }
}

void queryRecordsByTimeRange(const char *startDate, const char *endDate)
{
    printHeader("时间段维护记录");
    int found = 0;
    MaintRecordNode *cur = g_records;
    while (cur != NULL) {
        /* 日期格式 YYYY-MM-DD HH:MM:SS，字符串前10位为日期 */
        char recDate[MAX_TIME_LEN];
        strncpy(recDate, cur->maintTime, 10);
        recDate[10] = '\0';
        if (strcmp(recDate, startDate) >= 0 && strcmp(recDate, endDate) <= 0) {
            found++;
        }
        cur = cur->next;
    }
    if (found == 0) {
        printf("  %s 至 %s 时间段内无维护记录。\n", startDate, endDate);
        printf("\n");
        return;
    }
    printf("  %s 至 %s 共 %d 条维护记录：\n\n", startDate, endDate, found);
    cur = g_records;
    while (cur != NULL) {
        char recDate[MAX_TIME_LEN];
        strncpy(recDate, cur->maintTime, 10);
        recDate[10] = '\0';
        if (strcmp(recDate, startDate) >= 0 && strcmp(recDate, endDate) <= 0) {
            printRecordDetail(cur);
        }
        cur = cur->next;
    }
}

/* ======================== 巡检流程 ======================== */

void inspectDevices(const char *engId, const char *engName)
{
    printHeader("设备巡检");
    int total = countDevices();
    if (total == 0) {
        printf("  系统中暂无设备，无法巡检。\n");
        pauseScreen();
        return;
    }

    int checked = 0;
    int abnormalFound = 0;
    int maintained = 0;

    printf("  当前设备总数: %d\n", total);
    printf("  开始逐一巡检...\n\n");

    DeviceNode *cur = g_devices;
    while (cur != NULL) {
        checked++;
        printf("  [%d/%d] 设备编号: %s\n", checked, total, cur->id);
        printf("        设备名称: %s\n", cur->name);
        printf("        设备类型: %s\n", cur->type[0] ? cur->type : "未分类");
        printf("        所在位置: %s\n", cur->location[0] ? cur->location : "未知");
        printf("        当前状态: %s\n", cur->status == STATUS_ABNORMAL ? "*** 异常 ***" : "正常");

        if (cur->status == STATUS_ABNORMAL) {
            abnormalFound++;
            printf("\n  发现异常设备！请执行维护操作。\n");
            printf("  设备编号: %s, 名称: %s\n", cur->id, cur->name);
            printf("  请输入维护说明（故障现象与处理措施）: \n");
            char desc[MAX_DESC_LEN];
            readLine(desc, sizeof(desc));
            if (strlen(desc) == 0) {
                strcpy(desc, "无说明");
            }

            /* 执行维护：状态回写为正常，追加维护记录 */
            int beforeStatus = cur->status;
            markDeviceStatus(cur->id, STATUS_NORMAL, engId, engName);
            addMaintenanceRecord(cur->id, engId, engName, desc,
                                beforeStatus, STATUS_NORMAL);

            /* 从待维护队列中移除该设备 */
            removeDeviceFromQueue(cur->id);
            saveQueue();
            saveDevices();

            maintained++;
            printf("  维护成功！设备状态已恢复为正常。\n");
        } else {
            printf("  设备正常，确认后继续检查下一台。\n");
            printf("  按回车继续...");
            clearInputBuffer();
        }
        printf("\n");
        cur = cur->next;
    }

    printHeader("巡检汇总");
    printf("  巡检设备总数: %d\n", total);
    printf("  发现异常数量: %d\n", abnormalFound);
    printf("  已维护数量:   %d\n", maintained);
    printf("  巡检人:       %s (%s)\n", engName, engId);
    printf("\n");
    pauseScreen();
}

/* ======================== 维护流程 ======================== */

void performMaintenance(const char *engId, const char *engName)
{
    while (1) {
        /* 数据同步：每轮循环前重载磁盘最新数据 */
        reloadAllData();

        printHeader("设备维护");
        if (isQueueEmpty()) {
            printf("  待维护队列为空，当前无待维护设备。\n");
            pauseScreen();
            return;
        }

        printf("  待维护队列中共 %d 台设备。\n", getQueueSize());

        /* 从队首取出待维护设备 */
        char deviceId[MAX_ID_LEN];
        if (!dequeueDevice(deviceId, sizeof(deviceId))) {
            printf("  出队失败。\n");
            pauseScreen();
            return;
        }
        saveQueue();

        DeviceNode *dev = findDeviceById(deviceId);
        if (dev == NULL) {
            printf("  警告：设备 '%s' 已被删除，跳过。\n", deviceId);
            continue;
        }

        printf("\n  待维护设备详情：\n");
        printDeviceDetail(dev);
        printf("\n  请确认是否执行维护？(y/n): ");
        char confirm[4];
        readLine(confirm, sizeof(confirm));

        if (confirm[0] != 'y' && confirm[0] != 'Y') {
            /* 不维护，放回队列队尾 */
            enqueueDevice(deviceId);
            saveQueue();
            printf("  已跳过，设备返回队列末尾。\n");
            printf("  是否继续处理下一台？(y/n): ");
            readLine(confirm, sizeof(confirm));
            if (confirm[0] == 'y' || confirm[0] == 'Y') continue;
            else return;
        }

        /* 执行维护 */
        printf("  请输入维护说明（故障现象、处理措施等）: \n");
        char desc[MAX_DESC_LEN];
        readLine(desc, sizeof(desc));
        if (strlen(desc) == 0) {
            strcpy(desc, "无说明");
        }

        int beforeStatus = dev->status;
        markDeviceStatus(deviceId, STATUS_NORMAL, engId, engName);
        addMaintenanceRecord(deviceId, engId, engName, desc,
                             beforeStatus, STATUS_NORMAL);
        saveDevices();

        printf("\n  维护成功！\n");
        printf("  设备编号: %s\n", dev->id);
        printf("  设备名称: %s\n", dev->name);
        printf("  状态已恢复为: 正常\n");
        printf("  维护次数: %d\n", dev->maintCount);
        printf("  维护人:   %s (%s)\n", engName, engId);
        printf("  维护时间: %s\n", dev->lastMaintTime);

        printf("\n  是否继续处理下一台待维护设备？(y/n): ");
        readLine(confirm, sizeof(confirm));
        if (confirm[0] != 'y' && confirm[0] != 'Y') {
            return;
        }
    }
}

/* ======================== 查看队列与记录菜单 ======================== */

void viewQueue(void)
{
    traverseQueue();
    pauseScreen();
}

void viewRecordsMenu(void)
{
    while (1) {
        /* 数据同步：每次循环前重载磁盘最新数据 */
        reloadAllData();

        printHeader("维护记录查询");
        printf("  1. 按设备编号查询\n");
        printf("  2. 按维护人查询\n");
        printf("  3. 按时间段查询\n");
        printf("  0. 返回上一级\n");
        printf("\n请选择: ");

        int choice = readInt("");

        switch (choice) {
        case 1: {
            char id[MAX_ID_LEN];
            printf("请输入设备编号: "); readLine(id, sizeof(id));
            queryRecordsByDevice(id);
            pauseScreen();
            break;
        }
        case 2: {
            char id[MAX_ID_LEN];
            printf("请输入维护人工号: "); readLine(id, sizeof(id));
            queryRecordsByEngineer(id);
            pauseScreen();
            break;
        }
        case 3: {
            char start[MAX_TIME_LEN], end[MAX_TIME_LEN];
            printf("请输入起始日期 (YYYY-MM-DD): "); readLine(start, sizeof(start));
            printf("请输入结束日期 (YYYY-MM-DD): "); readLine(end, sizeof(end));
            queryRecordsByTimeRange(start, end);
            pauseScreen();
            break;
        }
        case 0:
            return;
        default:
            printf("无效选项。\n");
            break;
        }
    }
}
