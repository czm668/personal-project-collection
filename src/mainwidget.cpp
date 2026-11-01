#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "login.h"
#include "logger.h"
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    m_userMgr(UserManager::getInstance()),
    m_roomWidget(new RoomManagement()),
    m_guestWidget(new GuestManagement()),
    m_reserveWidget(new ReservationManagement()),
    m_queryWidget(new QueryManagement())
{
    ui->setupUi(this);
    initUI();

    // 将子界面添加到栈式窗口
    ui->stackedWidget_content->addWidget(m_roomWidget);
    ui->stackedWidget_content->addWidget(m_guestWidget);
    ui->stackedWidget_content->addWidget(m_reserveWidget);
    ui->stackedWidget_content->addWidget(m_queryWidget);

    // 窗口最大化
    showMaximized();
}

MainWidget::~MainWidget()
{
    delete ui;
    delete m_roomWidget;
    delete m_guestWidget;
    delete m_reserveWidget;
    delete m_queryWidget;
}

void MainWidget::initUI()
{
    // 显示当前登录用户
    QVariantMap user = m_userMgr->getCurrentUser();
    QString userInfo = QString("当前用户：%1（%2）")
                           .arg(user["name"].toString())
                           .arg(user["username"].toString());
    ui->label_userInfo->setText(userInfo);
    Logger::getInstance()->log("用户 " + user["username"].toString() + " 进入主界面");
}

void MainWidget::on_pushButton_room_clicked()
{
    // 切换到客房管理界面
    ui->stackedWidget_content->setCurrentWidget(m_roomWidget);
    m_roomWidget->refreshRoomList(); // 刷新数据
    Logger::getInstance()->log("切换到客房管理界面");
}

void MainWidget::on_pushButton_guest_clicked()
{
    // 切换到客户管理界面
    ui->stackedWidget_content->setCurrentWidget(m_guestWidget);
    m_guestWidget->refreshGuestList(); // 刷新数据
    Logger::getInstance()->log("切换到客户管理界面");
}

void MainWidget::on_pushButton_reservation_clicked()
{
    // 切换到预订管理界面
    ui->stackedWidget_content->setCurrentWidget(m_reserveWidget);
    m_reserveWidget->refreshReservationList(); // 刷新数据
    Logger::getInstance()->log("切换到预订管理界面");
}

// 添加查询按钮点击事件
void MainWidget::on_pushButton_query_clicked()
{
    // 切换到查询管理界面
    ui->stackedWidget_content->setCurrentWidget(m_queryWidget);
    Logger::getInstance()->log("切换到查询管理界面");
}

void MainWidget::on_pushButton_logout_clicked()
{
    // 确认退出
    int ret = QMessageBox::question(this, "确认", "是否退出登录？");
    if (ret == QMessageBox::Yes) {
        m_userMgr->resetCurrentUser();
        Logger::getInstance()->log("用户退出登录");
        // 返回登录界面
        Login *login = new Login();
        login->show();
        this->close();
    }
}
