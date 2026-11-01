#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

/**
 * @brief 数据库管理类（单例模式）
 * 负责数据库连接、表初始化，确保全局唯一数据库连接
 */
class DbManager
{
public:
    /** 获取单例实例 */
    static DbManager* getInstance();

    /** 打开数据库连接，返回是否成功 */
    bool openDatabase();

    /** 获取数据库连接对象 */
    QSqlDatabase getDatabase();

    /** 关闭数据库连接 */
    void closeDatabase();

private:
    QSqlDatabase m_db;          // 数据库连接对象
    static DbManager* m_instance; // 单例实例

    /** 私有构造函数（单例限制） */
    DbManager();

    /** 初始化数据库表结构，返回是否成功 */
    bool initTables();
};

#endif // DBMANAGER_H
