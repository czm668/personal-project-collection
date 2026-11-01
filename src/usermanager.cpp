#include "usermanager.h"
#include "dbmanager.h"
#include "logger.h"
#include <QSqlQuery>
#include <QSqlError>

UserManager* UserManager::m_instance = nullptr;

UserManager::UserManager()
{
    m_db = DbManager::getInstance()->getDatabase();
}

UserManager* UserManager::getInstance()
{
    if (m_instance == nullptr) {
        m_instance = new UserManager();
    }
    return m_instance;
}

bool UserManager::login(const QString& username, const QString& password)
{
    // 检查用户是否已锁定
    if (isUserLocked(username)) {
        Logger::getInstance()->logError("用户 " + username + " 已被锁定");
        return false;
    }

    // 查询用户信息
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        Logger::getInstance()->logError("登录查询失败: " + query.lastError().text());
        return false;
    }

    if (query.next()) {
        // 登录成功：重置失败次数 + 记录当前用户
        resetLoginAttempts(username);
        m_currentUser["id"] = query.value("id").toInt();
        m_currentUser["username"] = query.value("username").toString();
        m_currentUser["name"] = query.value("name").toString();
        m_currentUser["is_admin"] = query.value("is_admin").toBool();
        Logger::getInstance()->log("用户 " + username + " 登录成功");
        return true;
    } else {
        // 登录失败：增加失败次数（满3次则锁定）
        incrementLoginAttempts(username);
        Logger::getInstance()->logError("用户 " + username + " 登录失败：账号/密码错误");
        return false;
    }
}

bool UserManager::registerUser(const QString& username, const QString& password,
                               const QString& name, bool isAdmin)
{
    // 检查用户名是否已存在
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (query.exec() && query.next()) {
        Logger::getInstance()->logError("注册失败：用户名 " + username + " 已存在");
        return false;
    }

    // 插入新用户
    query.prepare("INSERT INTO users (username, password, name, is_admin) "
                  "VALUES (:username, :password, :name, :is_admin)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":name", name);
    query.bindValue(":is_admin", isAdmin);

    if (!query.exec()) {
        Logger::getInstance()->logError("注册失败: " + query.lastError().text());
        return false;
    }

    Logger::getInstance()->log("用户 " + username + " 注册成功");
    return true;
}

bool UserManager::isUserLocked(const QString& username)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT is_locked FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        return false; // 用户不存在则视为未锁定
    }
    return query.value("is_locked").toBool();
}

QVariantMap UserManager::getCurrentUser()
{
    return m_currentUser;
}

void UserManager::resetCurrentUser()
{
    m_currentUser.clear();
    Logger::getInstance()->log("用户已退出登录");
}

bool UserManager::incrementLoginAttempts(const QString& username)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT login_attempts FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if (!query.exec() || !query.next()) return false;

    int attempts = query.value("login_attempts").toInt() + 1;
    query.prepare("UPDATE users SET login_attempts = :attempts WHERE username = :username");
    query.bindValue(":attempts", attempts);
    query.bindValue(":username", username);
    if (!query.exec()) return false;

    // 满3次失败则锁定
    if (attempts >= 3) {
        lockUser(username);
    }
    return true;
}

bool UserManager::resetLoginAttempts(const QString& username)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE users SET login_attempts = 0 WHERE username = :username");
    query.bindValue(":username", username);
    return query.exec();
}

bool UserManager::lockUser(const QString& username)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE users SET is_locked = TRUE WHERE username = :username");
    query.bindValue(":username", username);
    bool result = query.exec();
    if (result) {
        Logger::getInstance()->log("用户 " + username + " 已被锁定（连续3次登录失败）");
    }
    return result;
}
