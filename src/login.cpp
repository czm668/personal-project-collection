#include "login.h"
#include "ui_login.h"
#include "register.h"
#include "mainwidget.h"
#include "logger.h"
#include <QMessageBox>
#include <QGuiApplication>
#include <QScreen>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login),
    m_userMgr(UserManager::getInstance())
{
    ui->setupUi(this); // 加载UI，基类为QWidget
    // 设置窗口居中
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            // 获取主屏幕的可用区域（替代旧desktop接口）
            QGuiApplication::primaryScreen()->availableGeometry()
            )
        );
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();

    // 空值校验
    if (username.isEmpty() || password.isEmpty()) {
        ui->label_error->setText("用户名和密码不能为空！");
        Logger::getInstance()->logError("登录失败：用户名或密码为空");
        return;
    }

    // 检查用户是否锁定
    if (m_userMgr->isUserLocked(username)) {
        ui->label_error->setText("该账号已被锁定（连续3次登录失败）！");
        Logger::getInstance()->logError("登录失败：用户 " + username + " 已被锁定");
        return;
    }

    // 登录验证
    if (m_userMgr->login(username, password)) {
        ui->label_error->clear();
        // 跳转到主界面
        MainWidget *mainWidget = new MainWidget();
        mainWidget->show();
        this->close();
    } else {
        ui->label_error->setText("用户名或密码错误！");
    }
}

void Login::on_pushButton_register_clicked()
{
    // 打开注册界面
    Register *reg = new Register(this);
    // 设置窗口为应用级模态（阻塞整个应用）
    reg->setWindowModality(Qt::ApplicationModal);
    // 可选：设置窗口为对话框样式（显示标题栏等）
    reg->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    reg->show();
}
