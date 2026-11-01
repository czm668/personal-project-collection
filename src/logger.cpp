#include "logger.h"
#include <QDebug>
#include <QDateTime>

Logger* Logger::m_instance = nullptr;

Logger::Logger() {}

Logger* Logger::getInstance()
{
    if (m_instance == nullptr) {
        m_instance = new Logger();
    }
    return m_instance;
}

void Logger::init(const QString& filename)
{
    m_logFile.setFileName(filename);
    if (!m_logFile.open(QIODevice::Append | QIODevice::Text)) {
        qCritical() << "日志文件打开失败: " << m_logFile.errorString();
    }
}

void Logger::log(const QString& message)
{
    writeLog("INFO", message);
}

void Logger::logError(const QString& message)
{
    writeLog("ERROR", message);
}

void Logger::writeLog(const QString& type, const QString& message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString logLine = QString("[%1] [%2] %3\n").arg(timestamp).arg(type).arg(message);

    // 输出到控制台
    if (type == "ERROR") {
        qCritical() << logLine.trimmed();
    } else {
        qDebug() << logLine.trimmed();
    }

    // 写入日志文件
    if (m_logFile.isOpen()) {
        QTextStream out(&m_logFile);
        out << logLine;
        m_logFile.flush();
    }
}
