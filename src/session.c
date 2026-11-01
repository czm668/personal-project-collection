/**
 * @file session.c
 * @brief 会话管理模块实现：会话记录、在线状态判定、过期清理
 *
 * 多端登录支持：
 * - 每次登录追加一条会话条目
 * - 注销/退出时标记对应会话为已结束
 * - 在线判定 = 存在至少一条未结束且 lastActive 在 30 分钟内的会话
 * - 工程师执行任意操作即刷新活跃时间
 */
#include "session.h"
#include "persistence.h"

/* ======================== 内部工具 ======================== */

/* 检查单条会话是否有效（未结束且未过期） */
static int isSessionActive(const SessionNode *s)
{
    if (s->ended) return 0;
    time_t now = getCurrentTimestamp();
    if (now - s->lastActive > SESSION_TIMEOUT) return 0;
    return 1;
}

/* ======================== 创建会话 ======================== */

int createSession(const char *engId, const char *terminalId)
{
    SessionNode *node = (SessionNode *)malloc(sizeof(SessionNode));
    if (node == NULL) return 0;
    memset(node, 0, sizeof(SessionNode));

    copyField(node->engId, MAX_ID_LEN, engId);
    copyField(node->terminalId, MAX_TERM_LEN, terminalId);
    getCurrentTime(node->loginTime, MAX_TIME_LEN);
    node->loginTimestamp = getCurrentTimestamp();
    node->lastActive = node->loginTimestamp;
    node->ended = 0;
    node->next = NULL;

    /* 追加到链表尾部 */
    if (g_sessions == NULL) {
        g_sessions = node;
    } else {
        SessionNode *cur = g_sessions;
        while (cur->next != NULL) cur = cur->next;
        cur->next = node;
    }
    saveSessions();
    return 1;
}

/* ======================== 结束会话 ======================== */

int endSession(const char *engId, const char *terminalId)
{
    SessionNode *cur = g_sessions;
    while (cur != NULL) {
        if (strcmp(cur->engId, engId) == 0 &&
            strcmp(cur->terminalId, terminalId) == 0 &&
            cur->ended == 0) {
            cur->ended = 1;
            saveSessions();
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

/* ======================== 刷新会话活跃时间 ======================== */

int refreshSession(const char *engId, const char *terminalId)
{
    SessionNode *cur = g_sessions;
    while (cur != NULL) {
        if (strcmp(cur->engId, engId) == 0 &&
            strcmp(cur->terminalId, terminalId) == 0 &&
            cur->ended == 0) {
            cur->lastActive = getCurrentTimestamp();
            saveSessions(); /* 落盘，保证其他实例 reload 后看到准确的活跃时间 */
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

/* ======================== 在线判定 ======================== */

int isEngineerOnline(const char *engId)
{
    SessionNode *cur = g_sessions;
    while (cur != NULL) {
        if (strcmp(cur->engId, engId) == 0 && isSessionActive(cur)) {
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

int getActiveSessionCount(const char *engId)
{
    int count = 0;
    SessionNode *cur = g_sessions;
    while (cur != NULL) {
        if (strcmp(cur->engId, engId) == 0 && isSessionActive(cur)) {
            count++;
        }
        cur = cur->next;
    }
    return count;
}

int getOnlineCount(void)
{
    int count = 0;
    EngineerNode *cur = g_engineers;
    while (cur != NULL) {
        if (isEngineerOnline(cur->id)) {
            count++;
        }
        cur = cur->next;
    }
    return count;
}

/* ======================== 清理会话 ======================== */

void cleanupSessions(void)
{
    /* 将已过期的未结束会话标记为已结束 */
    SessionNode *cur = g_sessions;
    int changed = 0;
    while (cur != NULL) {
        if (cur->ended == 0 && !isSessionActive(cur)) {
            cur->ended = 1;
            changed = 1;
        }
        cur = cur->next;
    }
    if (changed) {
        saveSessions();
    }
}

void endAllSessionsForTerminal(const char *terminalId)
{
    SessionNode *cur = g_sessions;
    int changed = 0;
    while (cur != NULL) {
        if (strcmp(cur->terminalId, terminalId) == 0 && cur->ended == 0) {
            cur->ended = 1;
            changed = 1;
        }
        cur = cur->next;
    }
    if (changed) {
        saveSessions();
    }
}

void endAllSessionsForEngineer(const char *engId)
{
    SessionNode *cur = g_sessions;
    int changed = 0;
    while (cur != NULL) {
        if (strcmp(cur->engId, engId) == 0 && cur->ended == 0) {
            cur->ended = 1;
            changed = 1;
        }
        cur = cur->next;
    }
    if (changed) {
        saveSessions();
    }
}
