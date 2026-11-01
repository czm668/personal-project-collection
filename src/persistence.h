/**
 * @file persistence.h
 * @brief 持久化模块：所有数据文件的加载与保存
 */
#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "common.h"

/* 初始化：确保数据目录存在，清理临时文件 */
int persistenceInit(void);

/* 加载所有数据 */
int loadAllData(void);

/* 保存所有数据 */
int saveAllData(void);

/* 重新加载所有数据（多实例数据同步：释放内存链表后从磁盘重读） */
void reloadAllData(void);

/* 工程师数据 */
int loadEngineers(void);
int saveEngineers(void);

/* 设备数据 */
int loadDevices(void);
int saveDevices(void);

/* 待维护队列 */
int loadQueue(void);
int saveQueue(void);

/* 维护记录 */
int loadRecords(void);
int saveRecords(void);

/* 会话记录 */
int loadSessions(void);
int saveSessions(void);

/* 系统状态 */
int loadSystemState(void);
int saveSystemState(void);

#endif /* PERSISTENCE_H */
