/**
 * @file device_manager.h
 * @brief 设备管理模块：设备增删改查、异常设备生成
 */
#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "common.h"

/* 设备链表操作 */
int addDevice(const char *id, const char *name, const char *type,
              const char *location);
int deleteDevice(const char *id);
int modifyDevice(const char *id, const char *newName, const char *newType,
                 const char *newLocation);
DeviceNode *findDeviceById(const char *id);
int isDeviceIdExist(const char *id);

/* 查询 */
void printDeviceDetail(const DeviceNode *dev);
void queryDeviceById(const char *id);
void queryDeviceByName(const char *nameKeyword);
void queryDeviceByStatus(int status);
void listAllDevices(void);

/* 异常设备生成 */
int generateAbnormalDevices(void);
int isAbnormalGeneratedToday(void);

/* 设备管理交互菜单（管理员/工程师共用） */
void deviceManageMenu(void);

/* 辅助 */
int countDevices(void);
int countDevicesByStatus(int status);
void markDeviceStatus(const char *deviceId, int status,
                      const char *engId, const char *engName);

#endif /* DEVICE_MANAGER_H */
