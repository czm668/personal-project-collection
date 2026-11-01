/**
 * @file session.h
 * @brief 会话管理模块：会话记录、在线状态判定、过期清理
 */
#ifndef SESSION_H
#define SESSION_H

#include "common.h"

/* 创建新会话（工程师登录成功时调用） */
int createSession(const char *engId, const char *terminalId);

/* 结束指定终端的会话（注销/退出时调用） */
int endSession(const char *engId, const char *terminalId);

/* 刷新指定终端的会话活跃时间（工程师执行操作时调用） */
int refreshSession(const char *engId, const char *terminalId);

/* 判定某工程师是否在线（存在至少一条未结束且未过期的会话） */
int isEngineerOnline(const char *engId);

/* 获取某工程师的活跃会话数（登录端数） */
int getActiveSessionCount(const char *engId);

/* 获取在线工程师总数 */
int getOnlineCount(void);

/* 清理已过期或已结束的会话（可选，用于维护数据整洁） */
void cleanupSessions(void);

/* 结束当前终端所有活跃会话（程序退出时调用） */
void endAllSessionsForTerminal(const char *terminalId);

/* 结束指定工程师所有活跃会话（删除工程师时调用） */
void endAllSessionsForEngineer(const char *engId);

#endif /* SESSION_H */
