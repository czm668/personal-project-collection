#ifndef RESERVATIONMANAGEMENT_H
#define RESERVATIONMANAGEMENT_H

#include <QWidget>
#include "reservationmanager.h"
#include "roommanager.h"
#include "guestmanager.h"
#include "usermanager.h"
#include <QTableWidget>
#include <QTableWidgetItem>

namespace Ui {
class ReservationManagement;
}

/**
 * @brief 预订管理界面类，继承自QWidget（非MainWindow）
 * 负责客房预订信息的增删改查UI交互，关联ReservationManager/RoomManager/GuestManager业务类
 */
class ReservationManagement : public QWidget
{
    Q_OBJECT

public:
    explicit ReservationManagement(QWidget *parent = nullptr);
    ~ReservationManagement();

    /**
     * @brief 刷新预订列表（从数据库重新加载并显示）
     */
    void refreshReservationList();

private slots:
    /**
     * @brief 添加预订按钮点击事件
     */
    void on_pushButton_add_clicked();

    /**
     * @brief 修改预订按钮点击事件
     */
    void on_pushButton_update_clicked();

    /**
     * @brief 删除预订按钮点击事件
     */
    void on_pushButton_delete_clicked();

    /**
     * @brief 清空表单按钮点击事件
     */
    void on_pushButton_clear_clicked();

    /**
     * @brief 表格行点击事件（加载选中行数据到表单）
     * @param item 选中的表格项
     */
    void on_tableWidget_reservations_itemClicked(QTableWidgetItem *item);

private:
    Ui::ReservationManagement *ui;          // UI界面对象
    ReservationManager *m_reserveMgr;       // 预订管理业务类实例
    RoomManager *m_roomMgr;                 // 客房管理业务类实例
    GuestManager *m_guestMgr;               // 客户管理业务类实例
    UserManager *m_userMgr;                 // 用户管理业务类实例
    int m_selectedReserveId;                // 当前选中的预订ID（-1表示未选中）

    /**
     * @brief 初始化表格列配置
     */
    void initTable();

    /**
     * @brief 加载客房下拉框数据
     */
    void loadRoomComboBox();

    /**
     * @brief 加载客户下拉框数据
     */
    void loadGuestComboBox();

    /**
     * @brief 清空表单所有输入项
     */
    void clearForm();

    /**
     * @brief 校验表单输入数据的合法性
     * @return 校验通过返回true，否则返回false
     */
    bool validateForm();

    /**
     * @brief 从下拉框获取选中的客房ID
     * @return 客房ID（-1表示未选中）
     */
    int getSelectedRoomId();

    /**
     * @brief 从下拉框获取选中的客户ID
     * @return 客户ID（-1表示未选中）
     */
    int getSelectedGuestId();
};

#endif // RESERVATIONMANAGEMENT_H
