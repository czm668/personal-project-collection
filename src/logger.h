#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>

/**
 * @brief 日志管理类（单例模式）
 * 负责系统操作日志、错误日志的记录（同时输出到控制台和文件）
 */
class Logger
{
public:
    /** 获取单例实例 */
    static Logger* getInstance();

    /** 初始化日志文件 */
    void init(const QString& filename);

    /** 记录普通日志 */
    void log(const QString& message);

    /** 记录错误日志 */
    void logError(const QString& message);

private:
    QFile m_logFile;            // 日志文件
    static Logger* m_instance;  // 单例实例

    /** 私有构造函数（单例限制） */
    Logger();

    /** 写入日志到文件+控制台 */
    void writeLog(const QString& type, const QString& message);
};

#endif // LOGGER_H
