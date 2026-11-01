#ifndef MENU_H
#define MENU_H

// 包含公共定义（结构体、宏等）
#include "common.h"

// 函数声明：管理员菜单展示
void show_admin_menu(void);

// 函数声明：普通用户菜单展示
void show_user_menu(void);

// 函数声明：处理管理员操作（参数：选择的操作编号）
void handle_admin_oper(int choice);

// 函数声明：处理普通用户操作（参数：选择的操作编号）
void handle_user_oper(int choice);

#endif // MENU_H
