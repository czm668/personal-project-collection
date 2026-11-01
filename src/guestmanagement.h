#ifndef GUESTMANAGEMENT_H
#define GUESTMANAGEMENT_H

#include <QWidget>
#include "guestmanager.h"
#include <QTableWidget>
#include <QTableWidgetItem>

namespace Ui {
class GuestManagement;
}

/**
 * @brief 客户管理界面类，继承自QWidget（非MainWindow）
 * 负责客户信息的增删改查UI交互，关联GuestManager业务类
 */
class GuestManagement : public QWidget
{
    Q_OBJECT

public:
    explicit GuestManagement(QWidget *parent = nullptr);
    ~GuestManagement();

    /**
     * @brief 刷新客户列表（从数据库重新加载并显示）
     */
    void refreshGuestList();

private slots:
    /**
     * @brief 添加客户按钮点击事件
     */
    void on_pushButton_add_clicked();

    /**
     * @brief 修改客户按钮点击事件
     */
    void on_pushButton_update_clicked();

    /**
     * @brief 删除客户按钮点击事件
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
    void on_tableWidget_guests_itemClicked(QTableWidgetItem *item);

private:
    Ui::GuestManagement *ui;          // UI界面对象
    GuestManager *m_guestMgr;         // 客户管理业务类实例
    int m_selectedGuestId;            // 当前选中的客户ID（-1表示未选中）

    /**
     * @brief 初始化表格列配置
     */
    void initTable();

    /**
     * @brief 清空表单所有输入项
     */
    void clearForm();

    /**
     * @brief 校验表单输入数据的合法性
     * @return 校验通过返回true，否则返回false
     */
    bool validateForm();
};

#endif // GUESTMANAGEMENT_H
