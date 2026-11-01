/**
 * @file user_manager.c
 * @brief 用户管理模块实现：工程师增删改查、登录校验、在线状态查询
 */
#include "user_manager.h"
#include "session.h"
#include "persistence.h"

/* ======================== 链表操作 ======================== */

EngineerNode *findEngineerById(const char *id)
{
    EngineerNode *cur = g_engineers;
    while (cur != NULL) {
        if (strcmp(cur->id, id) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

int isEngineerIdExist(const char *id)
{
    return findEngineerById(id) != NULL;
}

int addEngineer(const char *id, const char *name, const char *password)
{
    /* 校验账号唯一性 */
    if (isEngineerIdExist(id)) {
        printf("错误：工号 '%s' 已存在，无法重复添加。\n", id);
        return 0;
    }
    if (strlen(id) == 0 || strlen(name) == 0 || strlen(password) == 0) {
        printf("错误：工号、姓名、密码不能为空。\n");
        return 0;
    }

    EngineerNode *node = (EngineerNode *)malloc(sizeof(EngineerNode));
    if (node == NULL) {
        printf("错误：内存分配失败。\n");
        return 0;
    }
    memset(node, 0, sizeof(EngineerNode));
    copyField(node->id, MAX_ID_LEN, id);
    copyField(node->name, MAX_NAME_LEN, name);
    /* 密码哈希存储，不明文保存 */
    hashPassword(password, node->password, MAX_PWD_LEN);
    node->next = NULL;

    /* 追加到链表尾部 */
    if (g_engineers == NULL) {
        g_engineers = node;
    } else {
        EngineerNode *cur = g_engineers;
        while (cur->next != NULL) cur = cur->next;
        cur->next = node;
    }
    saveEngineers();
    return 1;
}

int deleteEngineer(const char *id)
{
    EngineerNode *prev = NULL;
    EngineerNode *cur = g_engineers;

    while (cur != NULL) {
        if (strcmp(cur->id, id) == 0) {
            /* 结束该工程师所有活跃会话 */
            endAllSessionsForEngineer(id);
            /* 二次确认在交互层处理 */
            if (prev == NULL) {
                g_engineers = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            saveEngineers();
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    printf("错误：未找到工号为 '%s' 的工程师。\n", id);
    return 0;
}

int modifyEngineer(const char *id, const char *newName, const char *newPassword,
                   const char *newId)
{
    EngineerNode *eng = findEngineerById(id);
    if (eng == NULL) {
        printf("错误：未找到工号为 '%s' 的工程师。\n", id);
        return 0;
    }

    /* 修改姓名 */
    if (newName != NULL && strlen(newName) > 0) {
        copyField(eng->name, MAX_NAME_LEN, newName);
    }

    /* 修改密码（哈希存储） */
    if (newPassword != NULL && strlen(newPassword) > 0) {
        hashPassword(newPassword, eng->password, MAX_PWD_LEN);
    }

    /* 修改工号（需校验唯一性） */
    if (newId != NULL && strlen(newId) > 0 && strcmp(newId, id) != 0) {
        if (isEngineerIdExist(newId)) {
            printf("错误：新工号 '%s' 已被占用，修改失败。\n", newId);
            saveEngineers(); /* 保存已修改的其他字段 */
            return 0;
        }
        copyField(eng->id, MAX_ID_LEN, newId);
    }

    saveEngineers();
    return 1;
}

/* ======================== 查询 ======================== */

void queryEngineerById(const char *id)
{
    EngineerNode *eng = findEngineerById(id);
    printHeader("工程师信息");
    if (eng == NULL) {
        printf("  未找到工号为 '%s' 的工程师。\n", id);
    } else {
        int online = isEngineerOnline(eng->id);
        int endpoints = getActiveSessionCount(eng->id);
        printf("  工号:     %s\n", eng->id);
        printf("  姓名:     %s\n", eng->name);
        printf("  在线状态: %s\n", online ? "在线" : "离线");
        if (online) {
            printf("  登录端数: %d\n", endpoints);
        }
    }
    printf("\n");
}

void queryEngineerByName(const char *nameKeyword)
{
    printHeader("工程师查询结果（按姓名模糊匹配）");
    int found = 0;
    EngineerNode *cur = g_engineers;
    printf("  %-20s %-20s %-10s %s\n", "工号", "姓名", "状态", "端数");
    printf("  ----------------------------------------------------\n");
    while (cur != NULL) {
        if (strstr(cur->name, nameKeyword) != NULL) {
            int online = isEngineerOnline(cur->id);
            int endpoints = getActiveSessionCount(cur->id);
            printf("  %-20s %-20s %-10s %d\n",
                   cur->id, cur->name, online ? "在线" : "离线",
                   online ? endpoints : 0);
            found++;
        }
        cur = cur->next;
    }
    if (found == 0) {
        printf("  未找到包含 '%s' 的工程师。\n", nameKeyword);
    } else {
        printf("\n  共找到 %d 位工程师。\n", found);
    }
    printf("\n");
}

void listAllEngineers(void)
{
    printHeader("全部工程师列表");
    int count = 0;
    EngineerNode *cur = g_engineers;
    printf("  %-20s %-20s %-10s %s\n", "工号", "姓名", "状态", "端数");
    printf("  ----------------------------------------------------\n");
    while (cur != NULL) {
        int online = isEngineerOnline(cur->id);
        int endpoints = getActiveSessionCount(cur->id);
        printf("  %-20s %-20s %-10s %d\n",
               cur->id, cur->name,
               online ? "在线" : "离线",
               online ? endpoints : 0);
        count++;
        cur = cur->next;
    }
    if (count == 0) {
        printf("  暂无工程师数据。\n");
    } else {
        printf("\n  共 %d 位工程师。\n", count);
    }
    printf("\n");
}

void showOnlineStatus(void)
{
    printHeader("工程师在线状态");
    int onlineCount = 0;
    int total = 0;
    EngineerNode *cur = g_engineers;
    printf("  %-20s %-20s %-10s %s\n", "工号", "姓名", "状态", "端数");
    printf("  ----------------------------------------------------\n");
    while (cur != NULL) {
        int online = isEngineerOnline(cur->id);
        int endpoints = getActiveSessionCount(cur->id);
        printf("  %-20s %-20s %-10s %d\n",
               cur->id, cur->name,
               online ? "在线" : "离线",
               online ? endpoints : 0);
        if (online) onlineCount++;
        total++;
        cur = cur->next;
    }
    printf("\n  在线: %d / %d\n", onlineCount, total);
    printf("\n");
}

/* ======================== 登录 ======================== */

int adminLogin(const char *password)
{
    return (strcmp(password, ADMIN_PWD) == 0);
}

int engineerLogin(const char *id, const char *password)
{
    EngineerNode *eng = findEngineerById(id);
    if (eng == NULL) {
        printf("错误：工号 '%s' 不存在。\n", id);
        return 0;
    }
    /* 密码校验（哈希比对） */
    if (!verifyPassword(password, eng->password)) {
        printf("错误：密码不正确。\n");
        return 0;
    }
    /* 登录成功，创建会话 */
    createSession(id, g_terminalId);
    printf("登录成功！欢迎，%s 工程师。\n", eng->name);
    return 1;
}

void engineerLogout(const char *id)
{
    endSession(id, g_terminalId);
    printf("已注销登录。\n");
}

/* ======================== 辅助 ======================== */

const char *getEngineerName(const char *id)
{
    EngineerNode *eng = findEngineerById(id);
    return eng ? eng->name : "未知";
}

/* ======================== 工程师管理交互菜单 ======================== */

void engineerManageMenu(void)
{
    while (1) {
        /* 数据同步：每次循环前重载磁盘最新数据 */
        reloadAllData();

        printHeader("工程师信息管理");
        printf("  1. 新增工程师\n");
        printf("  2. 删除工程师\n");
        printf("  3. 修改工程师信息\n");
        printf("  4. 按工号精确查询\n");
        printf("  5. 按姓名模糊查询\n");
        printf("  6. 列出全部工程师\n");
        printf("  0. 返回上一级\n");
        printf("\n请选择: ");

        int choice = readInt("");

        switch (choice) {
        case 1: { /* 新增 */
            char id[MAX_ID_LEN], name[MAX_NAME_LEN], pwd[MAX_PWD_LEN];
            printf("请输入工号: "); readLine(id, sizeof(id));
            printf("请输入姓名: "); readLine(name, sizeof(name));
            printf("请输入密码: "); readPassword(pwd, sizeof(pwd));
            if (strlen(pwd) == 0) {
                printf("密码不能为空。\n");
            } else if (addEngineer(id, name, pwd)) {
                printf("工程师添加成功！\n");
            }
            pauseScreen();
            break;
        }
        case 2: { /* 删除 */
            char id[MAX_ID_LEN];
            printf("请输入要删除的工号: "); readLine(id, sizeof(id));
            EngineerNode *eng = findEngineerById(id);
            if (eng == NULL) {
                printf("错误：未找到该工程师。\n");
            } else {
                printf("确认删除工程师 %s (%s)？(y/n): ", eng->name, eng->id);
                char confirm[4];
                readLine(confirm, sizeof(confirm));
                if (confirm[0] == 'y' || confirm[0] == 'Y') {
                    if (deleteEngineer(id)) {
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
            printf("请输入要修改的工号: "); readLine(id, sizeof(id));
            EngineerNode *eng = findEngineerById(id);
            if (eng == NULL) {
                printf("错误：未找到该工程师。\n");
            } else {
                printf("当前信息 - 姓名: %s\n", eng->name);
                printf("请输入新姓名（直接回车保持不变）: ");
                char name[MAX_NAME_LEN]; name[0] = '\0';
                readLine(name, sizeof(name));
                printf("请输入新密码（直接回车保持不变）: ");
                char pwd[MAX_PWD_LEN]; pwd[0] = '\0';
                readPassword(pwd, sizeof(pwd));
                printf("请输入新工号（直接回车保持不变）: ");
                char newId[MAX_ID_LEN]; newId[0] = '\0';
                readLine(newId, sizeof(newId));
                const char *pName = (strlen(name) > 0) ? name : NULL;
                const char *pPwd = (strlen(pwd) > 0) ? pwd : NULL;
                const char *pId = (strlen(newId) > 0) ? newId : NULL;
                if (modifyEngineer(id, pName, pPwd, pId)) {
                    printf("修改成功！\n");
                }
            }
            pauseScreen();
            break;
        }
        case 4: { /* 按工号查询 */
            char id[MAX_ID_LEN];
            printf("请输入工号: "); readLine(id, sizeof(id));
            queryEngineerById(id);
            pauseScreen();
            break;
        }
        case 5: { /* 按姓名模糊查询 */
            char name[MAX_NAME_LEN];
            printf("请输入姓名关键字: "); readLine(name, sizeof(name));
            queryEngineerByName(name);
            pauseScreen();
            break;
        }
        case 6: /* 列出全部 */
            listAllEngineers();
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
