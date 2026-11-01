#ifndef QUERYMANAGEMENT_H
#define QUERYMANAGEMENT_H

#include <QWidget>
#include "roommanager.h"
#include "guestmanager.h"
#include "reservationmanager.h"
#include <QTableWidgetItem>

namespace Ui {
class QueryManagement;
}

/**
 * @brief 查询管理界面类，用于整合查询客房、客户和预订信息
 */
class QueryManagement : public QWidget
{
    Q_OBJECT

public:
    explicit QueryManagement(QWidget *parent = nullptr);
    ~QueryManagement();

    /**
     * @brief 刷新查询结果列表
     */
    void refreshQueryResults();

private slots:
    /**
     * @brief 查询按钮点击事件
     */
    void on_pushButton_query_clicked();

    /**
     * @brief 重置按钮点击事件
     */
    void on_pushButton_reset_clicked();

    /**
     * @brief 查询类型切换事件
     * @param index 选中的索引
     */
    void on_comboBox_queryType_currentIndexChanged(int index);

private:
    Ui::QueryManagement *ui;                  // UI界面对象
    RoomManager *m_roomMgr;                   // 客房管理业务类实例
    GuestManager *m_guestMgr;                 // 客户管理业务类实例
    ReservationManager *m_reserveMgr;         // 预订管理业务类实例

    /**
     * @brief 初始化表格列配置
     * @param type 查询类型：0-客房，1-客户，2-预订
     */
    void initTable(int type);

    /**
     * @brief 清空查询条件
     */
    void clearQueryConditions();

    /**
     * @brief 执行客房查询
     */
    void queryRooms();

    /**
     * @brief 执行客户查询
     */
    void queryGuests();

    /**
     * @brief 执行预订查询
     */
    void queryReservations();
};

#endif // QUERYMANAGEMENT_H
