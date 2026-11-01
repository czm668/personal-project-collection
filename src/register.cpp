#include "register.h"
#include "ui_register.h"
#include "logger.h"
#include <QMessageBox>
#include <QStyle>
#include <QGuiApplication>
#include <QScreen>

// register.cpp 构造函数中添加初始化
Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register),
    m_userMgr(UserManager::getInstance())  // 新增这一行
{
    ui->setupUi(this);
    // 替换desktop接口
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            QGuiApplication::primaryScreen()->availableGeometry()
            )
        );
}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_submit_clicked()
{
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();
    QString confirm = ui->lineEdit_confirm->text().trimmed();
    QString name = ui->lineEdit_name->text().trimmed();

    // 校验输入
    if (username.isEmpty() || password.isEmpty() || name.isEmpty()) {
        ui->label_error->setText("用户名、密码、真实姓名不能为空！");
        Logger::getInstance()->logError("注册失败：必填项为空");
        return;
    }

    if (password != confirm) {
        ui->label_error->setText("两次输入的密码不一致！");
        Logger::getInstance()->logError("注册失败：密码确认不一致");
        return;
    }

    // 提交注册
    if (m_userMgr->registerUser(username, password, name)) {
        QMessageBox::information(this, "成功", "注册成功！请返回登录。");
        Logger::getInstance()->log("用户 " + username + " 注册成功");
        this->close();
    } else {
        ui->label_error->setText("注册失败：用户名已存在或系统错误！");
    }
}

void Register::on_pushButton_cancel_clicked()
{
    this->close();
}
