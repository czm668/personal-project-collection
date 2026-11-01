#include "roommanagement.h"
#include "ui_roommanagement.h"
#include "logger.h"
#include <QMessageBox>
#include <QDoubleValidator>

RoomManagement::RoomManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomManagement),
    m_roomMgr(RoomManager::getInstance()),
    m_selectedRoomId(-1)
{
    ui->setupUi(this);
    initTable();
    refreshRoomList();

    // 设置价格输入框为数字校验
    ui->lineEdit_price->setValidator(new QDoubleValidator(0.0, 99999.0, 2, this));
}

RoomManagement::~RoomManagement()
{
    delete ui;
}

void RoomManagement::initTable()
{
    // 设置表格列
    QStringList headers = {"ID", "房间号", "类型", "价格", "状态", "描述"};
    ui->tableWidget_rooms->setColumnCount(headers.size());
    ui->tableWidget_rooms->setHorizontalHeaderLabels(headers);
    // 表格属性设置
    ui->tableWidget_rooms->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidget_rooms->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget_rooms->horizontalHeader()->setStretchLastSection(true);
}

void RoomManagement::refreshRoomList()
{
    // 清空表格
    ui->tableWidget_rooms->setRowCount(0);

    // 获取所有客房
    QList<QVariantMap> rooms = m_roomMgr->getAllRooms();

    // 填充表格
    for (int i = 0; i < rooms.size(); i++) {
        QVariantMap room = rooms[i];
        ui->tableWidget_rooms->insertRow(i);
        ui->tableWidget_rooms->setItem(i, 0, new QTableWidgetItem(QString::number(room["id"].toInt())));
        ui->tableWidget_rooms->setItem(i, 1, new QTableWidgetItem(room["room_number"].toString()));
        ui->tableWidget_rooms->setItem(i, 2, new QTableWidgetItem(room["type"].toString()));
        ui->tableWidget_rooms->setItem(i, 3, new QTableWidgetItem(QString::number(room["price"].toDouble(), 'f', 2)));
        ui->tableWidget_rooms->setItem(i, 4, new QTableWidgetItem(room["status"].toString()));
        ui->tableWidget_rooms->setItem(i, 5, new QTableWidgetItem(room["description"].toString()));
    }

    Logger::getInstance()->log("刷新客房列表，共 " + QString::number(rooms.size()) + " 条记录");
}

void RoomManagement::clearForm()
{
    ui->lineEdit_roomnum->clear();
    ui->lineEdit_type->clear();
    ui->lineEdit_price->clear();
    ui->comboBox_status->setCurrentIndex(0);
    ui->textEdit_desc->clear();
    m_selectedRoomId = -1;
}

bool RoomManagement::validateForm()
{
    QString roomNum = ui->lineEdit_roomnum->text().trimmed();
    QString type = ui->lineEdit_type->text().trimmed();
    QString price = ui->lineEdit_price->text().trimmed();

    if (roomNum.isEmpty()) {
        QMessageBox::warning(this, "警告", "房间号不能为空！");
        return false;
    }

    if (type.isEmpty()) {
        QMessageBox::warning(this, "警告", "房间类型不能为空！");
        return false;
    }

    if (price.isEmpty()) {
        QMessageBox::warning(this, "警告", "价格不能为空！");
        return false;
    }

    return true;
}

void RoomManagement::on_pushButton_add_clicked()
{
    if (!validateForm()) return;

    // 获取表单数据
    QString roomNum = ui->lineEdit_roomnum->text().trimmed();
    QString type = ui->lineEdit_type->text().trimmed();
    double price = ui->lineEdit_price->text().toDouble();
    QString status = ui->comboBox_status->currentText();
    QString desc = ui->textEdit_desc->toPlainText().trimmed();

    // 添加客房
    if (m_roomMgr->addRoom(roomNum, type, price, status, desc)) {
        QMessageBox::information(this, "成功", "客房添加成功！");
        refreshRoomList();
        clearForm();
    } else {
        QMessageBox::critical(this, "失败", "客房添加失败！");
    }
}

void RoomManagement::on_pushButton_update_clicked()
{
    if (m_selectedRoomId == -1) {
        QMessageBox::warning(this, "警告", "请先选择要修改的客房！");
        return;
    }

    if (!validateForm()) return;

    // 获取表单数据
    QString roomNum = ui->lineEdit_roomnum->text().trimmed();
    QString type = ui->lineEdit_type->text().trimmed();
    double price = ui->lineEdit_price->text().toDouble();
    QString status = ui->comboBox_status->currentText();
    QString desc = ui->textEdit_desc->toPlainText().trimmed();

    // 修改客房
    if (m_roomMgr->updateRoom(m_selectedRoomId, roomNum, type, price, status, desc)) {
        QMessageBox::information(this, "成功", "客房修改成功！");
        refreshRoomList();
        clearForm();
    } else {
        QMessageBox::critical(this, "失败", "客房修改失败！");
    }
}

void RoomManagement::on_pushButton_delete_clicked()
{
    if (m_selectedRoomId == -1) {
        QMessageBox::warning(this, "警告", "请先选择要删除的客房！");
        return;
    }

    // 确认删除
    int ret = QMessageBox::question(this, "确认", "是否删除选中的客房？");
    if (ret == QMessageBox::Yes) {
        if (m_roomMgr->deleteRoom(m_selectedRoomId)) {
            QMessageBox::information(this, "成功", "客房删除成功！");
            refreshRoomList();
            clearForm();
        } else {
            QMessageBox::critical(this, "失败", "客房删除失败（可能存在关联预订）！");
        }
    }
}

void RoomManagement::on_pushButton_clear_clicked()
{
    clearForm();
}

void RoomManagement::on_tableWidget_rooms_itemClicked(QTableWidgetItem *item)
{
    // 获取选中行
    int row = item->row();
    m_selectedRoomId = ui->tableWidget_rooms->item(row, 0)->text().toInt();

    // 加载数据到表单
    QVariantMap room = m_roomMgr->getRoomById(m_selectedRoomId);
    ui->lineEdit_roomnum->setText(room["room_number"].toString());
    ui->lineEdit_type->setText(room["type"].toString());
    ui->lineEdit_price->setText(QString::number(room["price"].toDouble(), 'f', 2));
    ui->comboBox_status->setCurrentText(room["status"].toString());
    ui->textEdit_desc->setText(room["description"].toString());
}
