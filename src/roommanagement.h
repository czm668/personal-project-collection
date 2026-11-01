#ifndef ROOMMANAGEMENT_H
#define ROOMMANAGEMENT_H

#include <QWidget>
#include "roommanager.h"
#include <QTableWidget>
#include <QTableWidgetItem>

namespace Ui {
class RoomManagement;
}

/**
 * @brief 客房管理界面类，继承自QWidget（非MainWindow）
 * 负责客房信息的增删改查UI交互
 */
class RoomManagement : public QWidget
{
    Q_OBJECT

public:
    explicit RoomManagement(QWidget *parent = nullptr);
    ~RoomManagement();

    /**
     * @brief 刷新客房列表
     */
    void refreshRoomList();

private slots:
    /**
     * @brief 添加客房按钮点击事件
     */
    void on_pushButton_add_clicked();

    /**
     * @brief 修改客房按钮点击事件
     */
    void on_pushButton_update_clicked();

    /**
     * @brief 删除客房按钮点击事件
     */
    void on_pushButton_delete_clicked();

    /**
     * @brief 清空表单按钮点击事件
     */
    void on_pushButton_clear_clicked();

    /**
     * @brief 表格行点击事件（加载选中行数据到表单）
     */
    void on_tableWidget_rooms_itemClicked(QTableWidgetItem *item);

private:
    Ui::RoomManagement *ui;          // UI界面对象
    RoomManager *m_roomMgr;          // 客房管理业务类实例
    int m_selectedRoomId;            // 当前选中的客房ID

    /**
     * @brief 初始化表格列
     */
    void initTable();

    /**
     * @brief 清空表单数据
     */
    void clearForm();

    /**
     * @brief 校验表单数据
     * @return 校验通过返回true，否则返回false
     */
    bool validateForm();
};

#endif // ROOMMANAGEMENT_H
