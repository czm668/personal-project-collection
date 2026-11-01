#include <QApplication>
#include "splashscreen.h"
#include "login.h"
#include "dbmanager.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. 初始化日志系统
    Logger::getInstance()->init("hotel_system.log");
    Logger::getInstance()->log("程序启动");

    // 2. 显示启动页
    SplashScreen splash;
    splash.show();
    a.processEvents(); // 确保启动页立即显示

    // 3. 初始化数据库连接
    if (!DbManager::getInstance()->openDatabase()) {
        Logger::getInstance()->logError("数据库初始化失败，程序退出");
        return -1;
    }

    // 4. 启动页停留3秒
    splash.delay(3000);

    // 5. 显示登录界面
    Login login;
    login.show();
    splash.close();

    return a.exec();
}
