#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "usermanager.h"
#include "roommanagement.h"
#include "guestmanagement.h"
#include "reservationmanagement.h"
#include "querymanagement.h"

namespace Ui {
class MainWidget;
}

/**
 * @brief 系统主界面类，继承自QWidget（非MainWindow）
 * 负责功能导航、显示当前用户、退出登录
 */
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    /**
     * @brief 客房管理按钮点击事件
     */
    void on_pushButton_room_clicked();

    /**
     * @brief 客户管理按钮点击事件
     */
    void on_pushButton_guest_clicked();

    /**
     * @brief 预订管理按钮点击事件
     */
    void on_pushButton_reservation_clicked();

    // 声明查询按钮的槽函数
    void on_pushButton_query_clicked();

    /**
     * @brief 退出登录按钮点击事件
     */
    void on_pushButton_logout_clicked();



private:
    Ui::MainWidget *ui;                  // UI界面对象
    UserManager *m_userMgr;              // 用户管理类实例
    RoomManagement *m_roomWidget;        // 客房管理子界面
    GuestManagement *m_guestWidget;      // 客户管理子界面
    ReservationManagement *m_reserveWidget; // 预订管理子界面
    QueryManagement *m_queryWidget;        // 查询管理子界面

    /**
     * @brief 初始化界面（显示当前用户、加载子界面）
     */
    void initUI();
};

#endif // MAINWIDGET_H
