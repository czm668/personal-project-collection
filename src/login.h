#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "usermanager.h"

// 引入UI自动生成的类
namespace Ui {
class Login;
}

/**
 * @brief 登录界面类，继承自QWidget（非MainWindow）
 * 负责用户登录验证、跳转到注册界面
 */
class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    /**
     * @brief 登录按钮点击事件
     */
    void on_pushButton_login_clicked();

    /**
     * @brief 注册按钮点击事件
     */
    void on_pushButton_register_clicked();

private:
    Ui::Login *ui;          // UI界面对象
    UserManager *m_userMgr; // 用户管理类实例
};

#endif // LOGIN_H
