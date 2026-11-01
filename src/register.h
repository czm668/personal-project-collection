#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include "usermanager.h"

namespace Ui {
class Register;
}

/**
 * @brief 注册界面类，继承自QWidget（非MainWindow）
 * 负责用户注册功能
 */
class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    /**
     * @brief 提交注册按钮点击事件
     */
    void on_pushButton_submit_clicked();

    /**
     * @brief 取消按钮点击事件
     */
    void on_pushButton_cancel_clicked();

private:
    Ui::Register *ui;          // UI界面对象
    UserManager *m_userMgr;    // 用户管理类实例
};

#endif // REGISTER_H
