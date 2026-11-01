/**
 * @file user_manager.h
 * @brief 用户管理模块：工程师增删改查、登录校验、在线状态查询
 */
#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "common.h"

/* 工程师链表操作 */
int addEngineer(const char *id, const char *name, const char *password);
int deleteEngineer(const char *id);
int modifyEngineer(const char *id, const char *newName, const char *newPassword,
                   const char *newId);
EngineerNode *findEngineerById(const char *id);
int isEngineerIdExist(const char *id);

/* 查询 */
void queryEngineerById(const char *id);
void queryEngineerByName(const char *nameKeyword);
void listAllEngineers(void);
void showOnlineStatus(void);

/* 登录 */
int adminLogin(const char *password);
int engineerLogin(const char *id, const char *password);
void engineerLogout(const char *id);

/* 辅助 */
const char *getEngineerName(const char *id);

/* 工程师管理交互菜单 */
void engineerManageMenu(void);

#endif /* USER_MANAGER_H */
