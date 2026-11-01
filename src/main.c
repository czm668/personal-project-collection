/**
 * @file main.c
 * @brief 设备巡检系统主程序入口
 *
 * 通过宏区分编译目标：
 *   APP_ADMIN    → 管理员端 (admin.exe)
 *   APP_ENGINEER → 工程师端 (engineer.exe)
 *
 * 多实例数据同步设计：
 *   - 每次菜单循环前调用 reloadAllData() 从磁盘重载最新数据
 *   - 所有写操作即时落盘（原子写：tmp + rename）
 *   - 退出时重载磁盘数据后仅更新本终端会话，不覆盖其他实例的修改
 */
#include "common.h"
#include "persistence.h"
#include "session.h"
#include "user_manager.h"
#include "device_manager.h"
#include "maintenance.h"

#ifdef _WIN32
#include <windows.h>
#endif

#if !defined(APP_ADMIN) && !defined(APP_ENGINEER)
#error "必须定义 APP_ADMIN 或 APP_ENGINEER 之一来指定编译目标"
#endif

#if defined(APP_ADMIN) && defined(APP_ENGINEER)
#error "APP_ADMIN 与 APP_ENGINEER 只能定义其一"
#endif

#ifdef APP_ADMIN
#define APP_TITLE "设备巡检系统 - 管理员端"
#else
#define APP_TITLE "设备巡检系统 - 工程师端"
#endif

/* ======================== 全局状态 ======================== */
static Role g_currentRole = ROLE_NONE;
static char g_currentEngId[MAX_ID_LEN] = "";
static char g_currentEngName[MAX_NAME_LEN] = "";

/* 前置声明 */
void adminMenu(void);
void engineerMenu(void);

/* ======================== 退出清理 ======================== */

static void cleanupOnExit(void)
{
    /*
     * 重载磁盘最新数据后仅更新本终端的会话状态，
     * 避免用本实例的旧内存数据覆盖其他实例的修改。
     */
    reloadAllData();
    if (strlen(g_terminalId) > 0) {
        endAllSessionsForTerminal(g_terminalId); /* 内部已 saveSessions */
    }
    /* 释放内存 */
    freeEngineers();
    freeDevices();
    freeQueue();
    freeRecords();
    freeSessions();
}

/* ======================== 登录 ======================== */

#ifdef APP_ADMIN

static void adminLoginFlow(void)
{
    printHeader("管理员登录");
    char id[MAX_ID_LEN], pwd[MAX_PWD_LEN];
    printf("请输入账号: "); readLine(id, sizeof(id));

    int maxAttempts = 3;
    int attempts = 0;
    int loggedIn = 0;

    while (attempts < maxAttempts && !loggedIn) {
        printf("请输入密码 (第 %d/%d 次): ", attempts + 1, maxAttempts);
        readPassword(pwd, sizeof(pwd));

        if (strcmp(id, ADMIN_ID) == 0 && adminLogin(pwd)) {
            g_currentRole = ROLE_ADMIN;
            strcpy(g_currentEngId, ADMIN_ID);
            strcpy(g_currentEngName, "管理员");
            printf("\n管理员登录成功！\n");
            loggedIn = 1;

            /* 管理员登录时检查当日异常设备是否已生成 */
            if (!isAbnormalGeneratedToday()) {
                printf("\n检测到今日尚未生成异常设备，是否立即生成？(y/n): ");
                char confirm[4];
                readLine(confirm, sizeof(confirm));
                if (confirm[0] == 'y' || confirm[0] == 'Y') {
                    generateAbnormalDevices();
                }
            }
            adminMenu();
            g_currentRole = ROLE_NONE;
        } else {
            printf("管理员账号或密码错误。");
            attempts++;
            if (attempts < maxAttempts) printf(" 请重试。\n");
        }
    }
    if (!loggedIn) {
        printf("\n连续 %d 次登录失败，返回主菜单。\n", maxAttempts);
    }
}

#else /* APP_ENGINEER */

static void engineerLoginFlow(void)
{
    printHeader("工程师登录");
    char id[MAX_ID_LEN], pwd[MAX_PWD_LEN];
    printf("请输入工号: "); readLine(id, sizeof(id));

    int maxAttempts = 3;
    int attempts = 0;
    int loggedIn = 0;

    while (attempts < maxAttempts && !loggedIn) {
        printf("请输入密码 (第 %d/%d 次): ", attempts + 1, maxAttempts);
        readPassword(pwd, sizeof(pwd));

        if (engineerLogin(id, pwd)) {
            g_currentRole = ROLE_ENGINEER;
            strcpy(g_currentEngId, id);
            strcpy(g_currentEngName, getEngineerName(id));
            engineerMenu();
            /* 注销会话 */
            engineerLogout(g_currentEngId);
            g_currentRole = ROLE_NONE;
            loggedIn = 1;
        } else {
            attempts++;
            if (attempts < maxAttempts) printf(" 请重试。\n");
        }
    }
    if (!loggedIn) {
        printf("\n连续 %d 次登录失败，返回主菜单。\n", maxAttempts);
    }
}

#endif

/* ======================== 管理员菜单 ======================== */

void adminMenu(void)
{
    int menuCounter = 0;
    while (g_currentRole == ROLE_ADMIN) {
        /* 数据同步：每次循环前重载磁盘最新数据 */
        reloadAllData();

        /* 每5次操作清理一次过期会话 */
        if (++menuCounter % 5 == 0) {
            cleanupSessions();
        }
        printHeader("管理员菜单");
        printf("  1. 工程师信息管理\n");
        printf("  2. 查看工程师在线状态\n");
        printf("  3. 设备信息管理\n");
        printf("  4. 查看待维护队列\n");
        printf("  5. 查看维护记录\n");
        printf("  6. 生成当日异常设备\n");
        printf("  7. 查看所有设备列表\n");
        printf("  0. 注销登录\n");
        printf("\n请选择: ");

        int choice = readInt("");

        switch (choice) {
        case 1:
            engineerManageMenu();
            break;
        case 2:
            showOnlineStatus();
            pauseScreen();
            break;
        case 3:
            deviceManageMenu();
            break;
        case 4:
            viewQueue();
            break;
        case 5:
            viewRecordsMenu();
            break;
        case 6:
            generateAbnormalDevices();
            pauseScreen();
            break;
        case 7:
            listAllDevices();
            pauseScreen();
            break;
        case 0:
            printf("正在注销管理员登录...\n");
            g_currentRole = ROLE_NONE;
            break;
        default:
            printf("无效选项，请重新选择。\n");
            break;
        }
    }
}

/* ======================== 工程师菜单 ======================== */

void engineerMenu(void)
{
    int menuCounter = 0;
    while (g_currentRole == ROLE_ENGINEER) {
        /* 数据同步：每次循环前重载磁盘最新数据 */
        reloadAllData();

        /* 账号可能已被管理员在另一实例中删除 */
        if (findEngineerById(g_currentEngId) == NULL) {
            printf("\n您的账号已被删除，强制注销登录。\n");
            endAllSessionsForTerminal(g_terminalId);
            g_currentRole = ROLE_NONE;
            return;
        }

        /* 刷新本终端会话活跃时间（内部已落盘） */
        refreshSession(g_currentEngId, g_terminalId);

        /* 每5次操作清理一次过期会话 */
        if (++menuCounter % 5 == 0) {
            cleanupSessions();
        }

        printHeader("工程师菜单");
        printf("  当前用户: %s (%s)\n", g_currentEngName, g_currentEngId);
        printf("  1. 设备巡检\n");
        printf("  2. 设备信息管理\n");
        printf("  3. 设备维护\n");
        printf("  4. 查看待维护队列\n");
        printf("  5. 查看维护记录\n");
        printf("  6. 查看所有设备列表\n");
        printf("  0. 注销登录\n");
        printf("\n请选择: ");

        int choice = readInt("");

        switch (choice) {
        case 1:
            inspectDevices(g_currentEngId, g_currentEngName);
            break;
        case 2:
            deviceManageMenu();
            break;
        case 3:
            performMaintenance(g_currentEngId, g_currentEngName);
            break;
        case 4:
            viewQueue();
            break;
        case 5:
            viewRecordsMenu();
            break;
        case 6:
            listAllDevices();
            pauseScreen();
            break;
        case 0:
            printf("正在注销工程师登录...\n");
            g_currentRole = ROLE_NONE;
            break;
        default:
            printf("无效选项，请重新选择。\n");
            break;
        }
    }
}

/* ======================== 主函数 ======================== */

int main(void)
{
#ifdef _WIN32
    /* 设置控制台为 UTF-8 编码，解决中文乱码 */
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    /* 生成终端标识 */
    generateTerminalId(g_terminalId, sizeof(g_terminalId));

    /* 注册退出清理函数 */
    atexit(cleanupOnExit);

    /* 初始化持久化模块 */
    if (!persistenceInit()) {
        printf("警告：数据目录初始化失败，部分功能可能不可用。\n");
    }

    /* 加载所有数据 */
    loadAllData();

    /* 清理过期会话 */
    cleanupSessions();

    printf("\n");
    printf("==============================================\n");
    printf("        %s\n", APP_TITLE);
    printf("        基于链表、链队列与文件持久化\n");
    printf("==============================================\n");
    printf("  终端标识: %s\n", g_terminalId);
#ifdef APP_ADMIN
    printf("  管理员账号: %s\n", ADMIN_ID);
#endif

    /* 主菜单循环 */
    while (1) {
        /* 数据同步：每次循环前重载磁盘最新数据 */
        reloadAllData();

        printHeader("主菜单");
#ifdef APP_ADMIN
        printf("  1. 管理员登录\n");
#else
        printf("  1. 工程师登录\n");
#endif
        printf("  0. 退出系统\n");
        printf("\n请选择: ");

        int choice = readInt("");

        switch (choice) {
        case 1:
#ifdef APP_ADMIN
            adminLoginFlow();
#else
            engineerLoginFlow();
#endif
            break;
        case 0:
            printf("感谢使用设备巡检系统，再见！\n");
            return 0;
        default:
            printf("无效选项，请重新选择。\n");
            break;
        }
    }

    return 0;
}
