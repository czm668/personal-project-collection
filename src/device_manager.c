/**
 * @file device_manager.c
 * @brief 设备管理模块实现：设备增删改查、异常设备生成
 */
#include "device_manager.h"
#include "maintenance.h"  /* removeDeviceFromQueue */
#include "persistence.h"

/* ======================== 内部工具 ======================== */

/* 状态转字符串 */
static const char *statusStr(int status)
{
    return status == STATUS_ABNORMAL ? "异常" : "正常";
}

/* ======================== 链表操作 ======================== */

DeviceNode *findDeviceById(const char *id)
{
    DeviceNode *cur = g_devices;
    while (cur != NULL) {
        if (strcmp(cur->id, id) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

int isDeviceIdExist(const char *id)
{
    return findDeviceById(id) != NULL;
}

int addDevice(const char *id, const char *name, const char *type,
              const char *location)
{
    if (isDeviceIdExist(id)) {
        printf("错误：设备编号 '%s' 已存在。\n", id);
        return 0;
    }
    if (strlen(id) == 0 || strlen(name) == 0) {
        printf("错误：设备编号和名称不能为空。\n");
        return 0;
    }

    DeviceNode *node = (DeviceNode *)malloc(sizeof(DeviceNode));
    if (node == NULL) {
        printf("错误：内存分配失败。\n");
        return 0;
    }
    memset(node, 0, sizeof(DeviceNode));
    copyField(node->id, MAX_ID_LEN, id);
    copyField(node->name, MAX_NAME_LEN, name);
    copyField(node->type, MAX_TYPE_LEN, type);
    copyField(node->location, MAX_LOC_LEN, location);
    node->status = STATUS_NORMAL;
    node->lastMaintTime[0] = '\0';
    node->lastMaintEngId[0] = '\0';
    node->lastMaintEngName[0] = '\0';
    node->maintCount = 0;
    node->next = NULL;

    /* 追加到链表尾部 */
    if (g_devices == NULL) {
        g_devices = node;
    } else {
        DeviceNode *cur = g_devices;
        while (cur->next != NULL) cur = cur->next;
        cur->next = node;
    }
    saveDevices();
    return 1;
}

int deleteDevice(const char *id)
{
    DeviceNode *prev = NULL;
    DeviceNode *cur = g_devices;

    while (cur != NULL) {
        if (strcmp(cur->id, id) == 0) {
            if (prev == NULL) {
                g_devices = cur->next;
            } else {
                prev->next = cur->next;
            }
            /* 从待维护队列中移除（若存在） */
            removeDeviceFromQueue(id);
            free(cur);
            saveDevices();
            saveQueue();
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    printf("错误：未找到设备编号 '%s'。\n", id);
    return 0;
}

int modifyDevice(const char *id, const char *newName, const char *newType,
                 const char *newLocation)
{
    DeviceNode *dev = findDeviceById(id);
    if (dev == NULL) {
        printf("错误：未找到设备编号 '%s'。\n", id);
        return 0;
    }
    if (newName != NULL && strlen(newName) > 0) {
        copyField(dev->name, MAX_NAME_LEN, newName);
    }
    if (newType != NULL && strlen(newType) > 0) {
        copyField(dev->type, MAX_TYPE_LEN, newType);
    }
    if (newLocation != NULL && strlen(newLocation) > 0) {
        copyField(dev->location, MAX_LOC_LEN, newLocation);
    }
    saveDevices();
    return 1;
}

/* ======================== 查询 ======================== */

void printDeviceDetail(const DeviceNode *dev)
{
    printf("  设备编号:     %s\n", dev->id);
    printf("  设备名称:     %s\n", dev->name);
    printf("  设备类型:     %s\n", dev->type[0] ? dev->type : "未分类");
    printf("  所在位置:     %s\n", dev->location[0] ? dev->location : "未知");
    printf("  当前状态:     %s\n", statusStr(dev->status));
    printf("  最近维护时间: %s\n", dev->lastMaintTime[0] ? dev->lastMaintTime : "无");
    printf("  最近维护人:   %s\n", dev->lastMaintEngName[0] ? dev->lastMaintEngName : "无");
    printf("  维护次数:     %d\n", dev->maintCount);
}

void queryDeviceById(const char *id)
{
    DeviceNode *dev = findDeviceById(id);
    printHeader("设备信息");
    if (dev == NULL) {
        printf("  未找到设备编号 '%s'。\n", id);
    } else {
        printDeviceDetail(dev);
    }
    printf("\n");
}

void queryDeviceByName(const char *nameKeyword)
{
    printHeader("设备查询结果（按名称模糊匹配）");
    int found = 0;
    DeviceNode *cur = g_devices;
    printf("  %-15s %-20s %-10s %-10s\n", "编号", "名称", "状态", "位置");
    printf("  --------------------------------------------------\n");
    while (cur != NULL) {
        if (strstr(cur->name, nameKeyword) != NULL) {
            printf("  %-15s %-20s %-10s %-10s\n",
                   cur->id, cur->name, statusStr(cur->status),
                   cur->location[0] ? cur->location : "-");
            found++;
        }
        cur = cur->next;
    }
    if (found == 0) {
        printf("  未找到包含 '%s' 的设备。\n", nameKeyword);
    } else {
        printf("\n  共找到 %d 台设备。\n", found);
    }
    printf("\n");
}

void queryDeviceByStatus(int status)
{
    printHeader(status == STATUS_ABNORMAL ? "异常设备列表" : "正常设备列表");
    int found = 0;
    DeviceNode *cur = g_devices;
    printf("  %-15s %-20s %-10s %-10s\n", "编号", "名称", "状态", "位置");
    printf("  --------------------------------------------------\n");
    while (cur != NULL) {
        if (cur->status == status) {
            /* 异常设备编号前加标记 */
            if (status == STATUS_ABNORMAL) {
                printf("  [异常] %-10s %-20s %-10s %-10s\n",
                       cur->id, cur->name, statusStr(cur->status),
                       cur->location[0] ? cur->location : "-");
            } else {
                printf("  %-15s %-20s %-10s %-10s\n",
                       cur->id, cur->name, statusStr(cur->status),
                       cur->location[0] ? cur->location : "-");
            }
            found++;
        }
        cur = cur->next;
    }
    if (found == 0) {
        printf("  暂无%s设备。\n", statusStr(status));
    } else {
        printf("\n  共 %d 台。\n", found);
    }
    printf("\n");
}

void listAllDevices(void)
{
    printHeader("全部设备列表");
    int count = 0;
    int abnormal = 0;
    DeviceNode *cur = g_devices;
    printf("  %-15s %-20s %-10s %-15s\n", "编号", "名称", "状态", "位置");
    printf("  --------------------------------------------------------\n");
    while (cur != NULL) {
        if (cur->status == STATUS_ABNORMAL) {
            printf("  *%-14s %-20s %-10s %-15s\n",
                   cur->id, cur->name, statusStr(cur->status),
                   cur->location[0] ? cur->location : "-");
            abnormal++;
        } else {
            printf("  %-15s %-20s %-10s %-15s\n",
                   cur->id, cur->name, statusStr(cur->status),
                   cur->location[0] ? cur->location : "-");
        }
        count++;
        cur = cur->next;
    }
    if (count == 0) {
        printf("  暂无设备数据。\n");
    } else {
        printf("\n  共 %d 台设备（其中异常 %d 台）。\n", count, abnormal);
    }
    printf("\n");
}

/* ======================== 异常设备生成 ======================== */

int countDevices(void)
{
    int count = 0;
    DeviceNode *cur = g_devices;
    while (cur != NULL) { count++; cur = cur->next; }
    return count;
}

int countDevicesByStatus(int status)
{
    int count = 0;
    DeviceNode *cur = g_devices;
    while (cur != NULL) {
        if (cur->status == status) count++;
        cur = cur->next;
    }
    return count;
}

int isAbnormalGeneratedToday(void)
{
    char today[MAX_TIME_LEN];
    getCurrentDate(today, sizeof(today));
    return (strcmp(g_sysState.lastAbnormalDate, today) == 0);
}

int generateAbnormalDevices(void)
{
    /* 检查当日是否已生成 */
    if (isAbnormalGeneratedToday()) {
        printf("今日已生成过异常设备（共 %d 台），不重复生成。\n",
               g_sysState.lastAbnormalCount);
        return 0;
    }

    int total = countDevices();
    if (total == 0) {
        printf("系统中暂无设备，无法生成异常设备。\n");
        return 0;
    }

    /* 目标数量 = 设备总数 × 15%，向上取整 */
    int target = (total * ABNORMAL_RATIO + 99) / 100; /* 向上取整 */
    int normalCount = countDevicesByStatus(STATUS_NORMAL);

    printf("设备总数: %d\n", total);
    printf("正常设备: %d\n", normalCount);
    printf("目标异常数量 (15%%): %d\n", target);

    if (normalCount == 0) {
        printf("所有设备已处于异常状态，无可生成的异常设备。\n");
        /* 更新系统状态防止重复触发 */
        getCurrentDate(g_sysState.lastAbnormalDate, MAX_TIME_LEN);
        g_sysState.lastAbnormalCount = 0;
        saveSystemState();
        return 0;
    }

    /* 收集所有正常设备编号 */
    char (*normalIds)[MAX_ID_LEN] = (char(*)[MAX_ID_LEN])malloc(normalCount * MAX_ID_LEN);
    if (normalIds == NULL) {
        printf("内存不足，生成失败。\n");
        return 0;
    }

    int idx = 0;
    DeviceNode *cur = g_devices;
    while (cur != NULL) {
        if (cur->status == STATUS_NORMAL) {
            strcpy(normalIds[idx], cur->id);
            idx++;
        }
        cur = cur->next;
    }

    /* 如果正常设备不足，全部置异常 */
    int actualTarget = (normalCount < target) ? normalCount : target;
    if (normalCount < target) {
        printf("正常设备数量 (%d) 不足目标 (%d)，将全部 %d 台正常设备置为异常。\n",
               normalCount, target, normalCount);
    }

    /* 随机抽取 actualTarget 台设备 */
    /* Fisher-Yates 洗牌 */
    srand((unsigned int)time(NULL));
    for (int i = normalCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        /* 交换 */
        char tmp[MAX_ID_LEN];
        strcpy(tmp, normalIds[i]);
        strcpy(normalIds[i], normalIds[j]);
        strcpy(normalIds[j], tmp);
    }

    /* 取前 actualTarget 台置为异常并加入队列 */
    int generated = 0;
    for (int i = 0; i < actualTarget; i++) {
        DeviceNode *dev = findDeviceById(normalIds[i]);
        if (dev != NULL) {
            dev->status = STATUS_ABNORMAL;
            enqueueDevice(dev->id);
            generated++;
        }
    }

    free(normalIds);

    /* 更新系统状态 */
    getCurrentDate(g_sysState.lastAbnormalDate, MAX_TIME_LEN);
    g_sysState.lastAbnormalCount = generated;

    /* 保存 */
    saveDevices();
    saveQueue();
    saveSystemState();

    printf("成功生成 %d 台异常设备，已加入待维护队列。\n", generated);
    return generated;
}

void markDeviceStatus(const char *deviceId, int status,
                      const char *engId, const char *engName)
{
    DeviceNode *dev = findDeviceById(deviceId);
    if (dev == NULL) return;
    dev->status = status;
    if (status == STATUS_NORMAL) {
        /* 维护完成，更新维护信息 */
        getCurrentTime(dev->lastMaintTime, MAX_TIME_LEN);
        copyField(dev->lastMaintEngId, MAX_ID_LEN, engId);
        copyField(dev->lastMaintEngName, MAX_NAME_LEN, engName);
        dev->maintCount++;
    }
}

/* ======================== 设备管理交互菜单 ======================== */

void deviceManageMenu(void)
{
    while (1) {
        /* 数据同步：每次循环前重载磁盘最新数据 */
        reloadAllData();

        printHeader("设备信息管理");
        printf("  1. 新增设备\n");
        printf("  2. 删除设备\n");
        printf("  3. 修改设备信息\n");
        printf("  4. 按编号精确查询\n");
        printf("  5. 按名称模糊查询\n");
        printf("  6. 按状态筛选\n");
        printf("  7. 列出全部设备\n");
        printf("  0. 返回上一级\n");
        printf("\n请选择: ");

        int choice = readInt("");

        switch (choice) {
        case 1: { /* 新增 */
            char id[MAX_ID_LEN], name[MAX_NAME_LEN];
            char type[MAX_TYPE_LEN], loc[MAX_LOC_LEN];
            printf("请输入设备编号: "); readLine(id, sizeof(id));
            printf("请输入设备名称: "); readLine(name, sizeof(name));
            printf("请输入设备类型: "); readLine(type, sizeof(type));
            printf("请输入所在位置: "); readLine(loc, sizeof(loc));
            if (addDevice(id, name, type, loc)) {
                printf("设备添加成功！\n");
            }
            pauseScreen();
            break;
        }
        case 2: { /* 删除 */
            char id[MAX_ID_LEN];
            printf("请输入要删除的设备编号: "); readLine(id, sizeof(id));
            DeviceNode *dev = findDeviceById(id);
            if (dev == NULL) {
                printf("错误：未找到该设备。\n");
            } else {
                printf("确认删除设备 %s (%s)？(y/n): ", dev->name, dev->id);
                char confirm[4];
                readLine(confirm, sizeof(confirm));
                if (confirm[0] == 'y' || confirm[0] == 'Y') {
                    if (deleteDevice(id)) {
                        printf("删除成功！\n");
                    }
                } else {
                    printf("已取消删除。\n");
                }
            }
            pauseScreen();
            break;
        }
        case 3: { /* 修改 */
            char id[MAX_ID_LEN];
            printf("请输入要修改的设备编号: "); readLine(id, sizeof(id));
            DeviceNode *dev = findDeviceById(id);
            if (dev == NULL) {
                printf("错误：未找到该设备。\n");
            } else {
                printf("当前信息 - 名称: %s, 类型: %s, 位置: %s\n",
                       dev->name, dev->type, dev->location);
                printf("请输入新名称（直接回车保持不变）: ");
                char name[MAX_NAME_LEN]; name[0] = '\0';
                readLine(name, sizeof(name));
                printf("请输入新类型（直接回车保持不变）: ");
                char type[MAX_TYPE_LEN]; type[0] = '\0';
                readLine(type, sizeof(type));
                printf("请输入新位置（直接回车保持不变）: ");
                char loc[MAX_LOC_LEN]; loc[0] = '\0';
                readLine(loc, sizeof(loc));
                const char *pName = (strlen(name) > 0) ? name : NULL;
                const char *pType = (strlen(type) > 0) ? type : NULL;
                const char *pLoc = (strlen(loc) > 0) ? loc : NULL;
                if (modifyDevice(id, pName, pType, pLoc)) {
                    printf("修改成功！\n");
                }
            }
            pauseScreen();
            break;
        }
        case 4: { /* 按编号查询 */
            char id[MAX_ID_LEN];
            printf("请输入设备编号: "); readLine(id, sizeof(id));
            queryDeviceById(id);
            pauseScreen();
            break;
        }
        case 5: { /* 按名称模糊查询 */
            char name[MAX_NAME_LEN];
            printf("请输入名称关键字: "); readLine(name, sizeof(name));
            queryDeviceByName(name);
            pauseScreen();
            break;
        }
        case 6: { /* 按状态筛选 */
            printf("  1. 正常设备\n");
            printf("  2. 异常设备\n");
            printf("请选择: ");
            int s = readInt("");
            if (s == 1) queryDeviceByStatus(STATUS_NORMAL);
            else if (s == 2) queryDeviceByStatus(STATUS_ABNORMAL);
            else printf("无效选择。\n");
            pauseScreen();
            break;
        }
        case 7: /* 列出全部 */
            listAllDevices();
            pauseScreen();
            break;
        case 0:
            return;
        default:
            printf("无效选项，请重新选择。\n");
            break;
        }
    }
}
