#include "dbmanager.h"
#include "logger.h"
#include <QDebug>

DbManager* DbManager::m_instance = nullptr;

DbManager::DbManager() {}

DbManager* DbManager::getInstance()
{
    if (m_instance == nullptr) {
        m_instance = new DbManager();
    }
    return m_instance;
}

bool DbManager::openDatabase()
{
    const QString host = "127.0.0.1";   // 数据库IP
    const int     port = 3306;          // 端口
    const QString dbName = "hotel_db";  // 数据库名（不存在时自动创建）
    const QString user = "root";        // 数据库用户名
    const QString pass = "";            // 数据库密码（本机 MySQL 的 root 当前为空密码）

    // 复用已存在的连接（避免重复创建）
    if (QSqlDatabase::contains("hotel_connection")) {
        m_db = QSqlDatabase::database("hotel_connection");
    } else {
        m_db = QSqlDatabase::addDatabase("QMYSQL", "hotel_connection");
        m_db.setHostName(host);
        m_db.setPort(port);
        m_db.setUserName(user);
        m_db.setPassword(pass);
        // 先不指定库名连接，用于自动建库
        m_db.setDatabaseName("");
    }

    // 尝试打开连接（此时未指定具体数据库）
    if (!m_db.open()) {
        Logger::getInstance()->logError("数据库连接失败: " + m_db.lastError().text());
        return false;
    }

    // 确保目标数据库存在，不存在则创建
    {
        QSqlQuery q(m_db);
        if (!q.exec("CREATE DATABASE IF NOT EXISTS `" + dbName + "`")) {
            Logger::getInstance()->logError("创建数据库失败: " + q.lastError().text());
            m_db.close();
            return false;
        }
    }

    // 切换到目标数据库：重新以指定库名打开连接
    m_db.setDatabaseName(dbName);
    m_db.close();
    if (!m_db.open()) {
        Logger::getInstance()->logError("打开数据库 " + dbName + " 失败: " + m_db.lastError().text());
        return false;
    }

    Logger::getInstance()->log("数据库连接成功");
    return initTables(); // 初始化表结构
}

QSqlDatabase DbManager::getDatabase()
{
    return m_db;
}

void DbManager::closeDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
        Logger::getInstance()->log("数据库连接已关闭");
    }
}

bool DbManager::initTables()
{
    QSqlQuery query(m_db);
    QString sql;

    // 1. 创建用户表
    sql = "CREATE TABLE IF NOT EXISTS users ("
          "id INT PRIMARY KEY AUTO_INCREMENT,"
          "username VARCHAR(50) UNIQUE NOT NULL,"
          "password VARCHAR(50) NOT NULL,"
          "name VARCHAR(100) NOT NULL,"
          "is_admin BOOLEAN DEFAULT FALSE,"
          "is_locked BOOLEAN DEFAULT FALSE,"
          "login_attempts INT DEFAULT 0,"
          "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)";
    if (!query.exec(sql)) {
        Logger::getInstance()->logError("创建用户表失败: " + query.lastError().text());
        return false;
    }

    // 2. 创建客房表
    sql = "CREATE TABLE IF NOT EXISTS rooms ("
          "id INT PRIMARY KEY AUTO_INCREMENT,"
          "room_number VARCHAR(20) UNIQUE NOT NULL,"
          "type VARCHAR(50) NOT NULL,"
          "price DECIMAL(10,2) NOT NULL,"
          "status VARCHAR(20) NOT NULL DEFAULT '空闲',"
          "description TEXT,"
          "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)";
    if (!query.exec(sql)) {
        Logger::getInstance()->logError("创建客房表失败: " + query.lastError().text());
        return false;
    }

    // 3. 创建客户表
    sql = "CREATE TABLE IF NOT EXISTS guests ("
          "id INT PRIMARY KEY AUTO_INCREMENT,"
          "name VARCHAR(100) NOT NULL,"
          "id_card VARCHAR(20) UNIQUE NOT NULL,"
          "phone VARCHAR(20) NOT NULL,"
          "address TEXT,"
          "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)";
    if (!query.exec(sql)) {
        Logger::getInstance()->logError("创建客户表失败: " + query.lastError().text());
        return false;
    }

    // 4. 创建预订表
    sql = "CREATE TABLE IF NOT EXISTS reservations ("
          "id INT PRIMARY KEY AUTO_INCREMENT,"
          "room_id INT NOT NULL,"
          "guest_id INT NOT NULL,"
          "checkin_date DATETIME NOT NULL,"
          "checkout_date DATETIME NOT NULL,"
          "status VARCHAR(20) NOT NULL DEFAULT '预订中',"
          "created_by INT NOT NULL,"
          "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
          "FOREIGN KEY (room_id) REFERENCES rooms(id),"
          "FOREIGN KEY (guest_id) REFERENCES guests(id),"
          "FOREIGN KEY (created_by) REFERENCES users(id))";
    if (!query.exec(sql)) {
        Logger::getInstance()->logError("创建预订表失败: " + query.lastError().text());
        return false;
    }

    // 初始化管理员账号（默认账号：admin / 密码：admin123）
    sql = "INSERT IGNORE INTO users (username, password, name, is_admin) "
          "VALUES ('admin', 'admin123', '系统管理员', TRUE)";
    query.exec(sql);

    Logger::getInstance()->log("数据库表初始化完成");
    return true;
}
