#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>
#include <QVariantMap>
#include <QList>
#include <QSqlDatabase>

/**
 * @brief 用户管理类（单例模式）
 * 负责用户登录、注册、信息管理等业务逻辑
 */
class UserManager
{
public:
    /** 获取单例实例 */
    static UserManager* getInstance();

    /** 用户登录：返回是否成功 */
    bool login(const QString& username, const QString& password);

    /** 用户注册：返回是否成功 */
    bool registerUser(const QString& username, const QString& password,
                      const QString& name, bool isAdmin = false);

    /** 检查用户是否被锁定 */
    bool isUserLocked(const QString& username);

    /** 获取当前登录用户信息 */
    QVariantMap getCurrentUser();

    /** 重置当前登录用户（退出登录） */
    void resetCurrentUser();

private:
    QSqlDatabase m_db;          // 数据库连接
    static UserManager* m_instance; // 单例实例
    QVariantMap m_currentUser;  // 当前登录用户信息

    /** 私有构造函数（单例限制） */
    UserManager();

    /** 增加登录失败次数 */
    bool incrementLoginAttempts(const QString& username);

    /** 重置登录失败次数 */
    bool resetLoginAttempts(const QString& username);

    /** 锁定用户（连续3次登录失败） */
    bool lockUser(const QString& username);
};

#endif // USERMANAGER_H
