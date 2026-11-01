/**
 * @file maintenance.h
 * @brief 巡检维护模块：待维护队列操作、巡检流程、维护流程、维护记录管理
 */
#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include "common.h"

/* ======================== 待维护队列操作 ======================== */
void initQueue(void);
int enqueueDevice(const char *deviceId);
int dequeueDevice(char *outDeviceId, int outSize);
int isQueueEmpty(void);
int getQueueSize(void);
void traverseQueue(void);
int removeDeviceFromQueue(const char *deviceId);

/* ======================== 维护记录操作 ======================== */
int addMaintenanceRecord(const char *deviceId, const char *engId,
                         const char *engName, const char *description,
                         int beforeStatus, int afterStatus);
void queryRecordsByDevice(const char *deviceId);
void queryRecordsByEngineer(const char *engId);
void queryRecordsByTimeRange(const char *startDate, const char *endDate);

/* ======================== 巡检与维护流程 ======================== */

/* 工程师巡检设备 */
void inspectDevices(const char *engId, const char *engName);

/* 工程师执行维护（从队列出队并维护） */
void performMaintenance(const char *engId, const char *engName);

/* 查看待维护队列 */
void viewQueue(void);

/* 查看维护记录菜单 */
void viewRecordsMenu(void);

#endif /* MAINTENANCE_H */
