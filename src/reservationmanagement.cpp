#include "reservationmanagement.h"
#include "ui_reservationmanagement.h"
#include "logger.h"
#include <QMessageBox>
#include <QDateTime>

ReservationManagement::ReservationManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReservationManagement),
    m_reserveMgr(ReservationManager::getInstance()),
    m_roomMgr(RoomManager::getInstance()),
    m_guestMgr(GuestManager::getInstance()),
    m_userMgr(UserManager::getInstance()),
    m_selectedReserveId(-1)
{
    ui->setupUi(this);
    initTable();
    loadRoomComboBox();    // 加载客房下拉框
    loadGuestComboBox();   // 加载客户下拉框
    refreshReservationList(); // 刷新预订列表

    // 设置默认日期为当前时间
    ui->datetimeEdit_checkin->setDateTime(QDateTime::currentDateTime());
    ui->datetimeEdit_checkout->setDateTime(QDateTime::currentDateTime().addDays(1));
}

ReservationManagement::~ReservationManagement()
{
    delete ui;
}

void ReservationManagement::initTable()
{
    // 设置表格列头和列数
    QStringList headers = {"ID", "客房号", "客房类型", "客户姓名", "身份证号",
                           "入住日期", "退房日期", "预订状态", "创建人", "创建时间"};
    ui->tableWidget_reservations->setColumnCount(headers.size());
    ui->tableWidget_reservations->setHorizontalHeaderLabels(headers);

    // 表格样式配置
    ui->tableWidget_reservations->setSelectionBehavior(QTableWidget::SelectRows);  // 整行选中
    ui->tableWidget_reservations->setEditTriggers(QTableWidget::NoEditTriggers);   // 禁止编辑
    ui->tableWidget_reservations->horizontalHeader()->setStretchLastSection(true); // 最后一列自适应
    // 设置表格垂直表头的默认行高（所有行适用）
    ui->tableWidget_reservations->verticalHeader()->setDefaultSectionSize(25);
}

void ReservationManagement::loadRoomComboBox()
{
    // 清空下拉框
    ui->comboBox_room->clear();

    // 获取所有空闲/已预订的客房
    QList<QVariantMap> rooms = m_roomMgr->getAllRooms();
    for (const QVariantMap& room : rooms) {
        QString roomText = QString("%1 - %2（%3）")
                               .arg(room["room_number"].toString())
                               .arg(room["type"].toString())
                               .arg(room["status"].toString());
        // 存储客房ID为用户数据
        ui->comboBox_room->addItem(roomText, room["id"].toInt());
    }

    Logger::getInstance()->log("加载客房下拉框，共 " + QString::number(rooms.size()) + " 条客房数据");
}

void ReservationManagement::loadGuestComboBox()
{
    // 清空下拉框
    ui->comboBox_guest->clear();

    // 获取所有客户
    QList<QVariantMap> guests = m_guestMgr->getAllGuests();
    for (const QVariantMap& guest : guests) {
        QString guestText = QString("%1 - %2")
        .arg(guest["name"].toString())
            .arg(guest["id_card"].toString());
        // 存储客户ID为用户数据
        ui->comboBox_guest->addItem(guestText, guest["id"].toInt());
    }

    Logger::getInstance()->log("加载客户下拉框，共 " + QString::number(guests.size()) + " 条客户数据");
}

void ReservationManagement::refreshReservationList()
{
    // 清空表格现有数据
    ui->tableWidget_reservations->setRowCount(0);

    // 从业务类获取所有预订数据
    QList<QVariantMap> reservations = m_reserveMgr->getAllReservations();

    // 填充表格数据
    for (int i = 0; i < reservations.size(); i++) {
        QVariantMap reserve = reservations[i];
        ui->tableWidget_reservations->insertRow(i);

        // 逐列设置数据
        ui->tableWidget_reservations->setItem(i, 0, new QTableWidgetItem(QString::number(reserve["id"].toInt())));
        ui->tableWidget_reservations->setItem(i, 1, new QTableWidgetItem(reserve["room_number"].toString()));
        ui->tableWidget_reservations->setItem(i, 2, new QTableWidgetItem(reserve["room_type"].toString()));
        ui->tableWidget_reservations->setItem(i, 3, new QTableWidgetItem(reserve["guest_name"].toString()));
        ui->tableWidget_reservations->setItem(i, 4, new QTableWidgetItem(reserve["guest_id_card"].toString()));
        ui->tableWidget_reservations->setItem(i, 5, new QTableWidgetItem(reserve["checkin_date"].toString()));
        ui->tableWidget_reservations->setItem(i, 6, new QTableWidgetItem(reserve["checkout_date"].toString()));
        ui->tableWidget_reservations->setItem(i, 7, new QTableWidgetItem(reserve["status"].toString()));
        ui->tableWidget_reservations->setItem(i, 8, new QTableWidgetItem(reserve["creator"].toString()));
        ui->tableWidget_reservations->setItem(i, 9, new QTableWidgetItem(reserve["created_at"].toString()));
    }

    Logger::getInstance()->log("刷新预订列表，共加载 " + QString::number(reservations.size()) + " 条预订记录");
}

void ReservationManagement::clearForm()
{
    // 重置下拉框选中状态
    ui->comboBox_room->setCurrentIndex(0);
    ui->comboBox_guest->setCurrentIndex(0);
    // 重置日期
    ui->datetimeEdit_checkin->setDateTime(QDateTime::currentDateTime());
    ui->datetimeEdit_checkout->setDateTime(QDateTime::currentDateTime().addDays(1));
    // 重置状态
    ui->comboBox_status->setCurrentIndex(0);
    // 重置选中ID
    m_selectedReserveId = -1;
}

bool ReservationManagement::validateForm()
{
    // 校验是否选中客房
    if (getSelectedRoomId() == -1) {
        QMessageBox::warning(this, "输入校验", "请选择客房！");
        return false;
    }

    // 校验是否选中客户
    if (getSelectedGuestId() == -1) {
        QMessageBox::warning(this, "输入校验", "请选择客户！");
        return false;
    }

    // 校验日期逻辑
    QDateTime checkin = ui->datetimeEdit_checkin->dateTime();
    QDateTime checkout = ui->datetimeEdit_checkout->dateTime();
    if (checkout <= checkin) {
        QMessageBox::warning(this, "输入校验", "退房日期必须晚于入住日期！");
        return false;
    }

    // 校验客房是否可用（添加时）
    if (m_selectedReserveId == -1 &&
        !m_reserveMgr->isRoomAvailable(getSelectedRoomId(), checkin, checkout)) {
        QMessageBox::warning(this, "输入校验", "该客房在指定时间段已被预订！");
        return false;
    }

    return true;
}

int ReservationManagement::getSelectedRoomId()
{
    // 从下拉框用户数据中获取客房ID
    return ui->comboBox_room->currentData().toInt();
}

int ReservationManagement::getSelectedGuestId()
{
    // 从下拉框用户数据中获取客户ID
    return ui->comboBox_guest->currentData().toInt();
}

void ReservationManagement::on_pushButton_add_clicked()
{
    // 先校验表单
    if (!validateForm()) return;

    // 获取表单数据
    int roomId = getSelectedRoomId();
    int guestId = getSelectedGuestId();
    QDateTime checkin = ui->datetimeEdit_checkin->dateTime();
    QDateTime checkout = ui->datetimeEdit_checkout->dateTime();
    int createdBy = m_userMgr->getCurrentUser()["id"].toInt(); // 当前登录用户ID

    // 调用业务类添加预订
    if (m_reserveMgr->addReservation(roomId, guestId, checkin, checkout, createdBy)) {
        QMessageBox::information(this, "操作成功", "客房预订添加成功！");
        Logger::getInstance()->log("添加预订成功：客房ID=" + QString::number(roomId) + "，客户ID=" + QString::number(guestId));
        refreshReservationList(); // 刷新列表
        loadRoomComboBox();      // 重新加载客房下拉框（更新状态）
        clearForm();             // 清空表单
    } else {
        QMessageBox::critical(this, "操作失败", "客房预订添加失败！");
    }
}

void ReservationManagement::on_pushButton_update_clicked()
{
    // 校验是否选中预订
    if (m_selectedReserveId == -1) {
        QMessageBox::warning(this, "操作提示", "请先从表格中选择要修改的预订！");
        return;
    }

    // 校验表单
    if (!validateForm()) return;

    // 获取表单数据
    int roomId = getSelectedRoomId();
    int guestId = getSelectedGuestId();
    QDateTime checkin = ui->datetimeEdit_checkin->dateTime();
    QDateTime checkout = ui->datetimeEdit_checkout->dateTime();
    QString status = ui->comboBox_status->currentText();

    // 调用业务类修改预订
    if (m_reserveMgr->updateReservation(m_selectedReserveId, roomId, guestId, checkin, checkout, status)) {
        QMessageBox::information(this, "操作成功", "客房预订修改成功！");
        Logger::getInstance()->log("修改预订成功：ID=" + QString::number(m_selectedReserveId));
        refreshReservationList(); // 刷新列表
        loadRoomComboBox();      // 重新加载客房下拉框（更新状态）
        clearForm();             // 清空表单
    } else {
        QMessageBox::critical(this, "操作失败", "客房预订修改失败！");
    }
}

void ReservationManagement::on_pushButton_delete_clicked()
{
    // 校验是否选中预订
    if (m_selectedReserveId == -1) {
        QMessageBox::warning(this, "操作提示", "请先从表格中选择要删除的预订！");
        return;
    }

    // 确认删除
    int ret = QMessageBox::question(this, "确认删除", "是否确定删除选中的预订？删除后无法恢复！",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    // 调用业务类删除预订
    if (m_reserveMgr->deleteReservation(m_selectedReserveId)) {
        QMessageBox::information(this, "操作成功", "客房预订删除成功！");
        Logger::getInstance()->log("删除预订成功：ID=" + QString::number(m_selectedReserveId));
        refreshReservationList(); // 刷新列表
        loadRoomComboBox();      // 重新加载客房下拉框（更新状态）
        clearForm();             // 清空表单
    } else {
        QMessageBox::critical(this, "操作失败", "客房预订删除失败！");
    }
}

void ReservationManagement::on_pushButton_clear_clicked()
{
    clearForm(); // 清空表单
}

void ReservationManagement::on_tableWidget_reservations_itemClicked(QTableWidgetItem *item)
{
    // 获取选中行
    int row = item->row();

    // 获取选中预订ID
    m_selectedReserveId = ui->tableWidget_reservations->item(row, 0)->text().toInt();

    // 加载选中预订数据到表单
    QVariantMap reserve = m_reserveMgr->getReservationById(m_selectedReserveId);
    if (reserve.isEmpty()) {
        return;
    }

    // 设置日期
    ui->datetimeEdit_checkin->setDateTime(reserve["checkin_date"].toDateTime());
    ui->datetimeEdit_checkout->setDateTime(reserve["checkout_date"].toDateTime());

    // 设置状态
    ui->comboBox_status->setCurrentText(reserve["status"].toString());

    // 选中对应的客房
    for (int i = 0; i < ui->comboBox_room->count(); i++) {
        if (ui->comboBox_room->itemData(i).toInt() == reserve["room_id"].toInt()) {
            ui->comboBox_room->setCurrentIndex(i);
            break;
        }
    }

    // 选中对应的客户
    for (int i = 0; i < ui->comboBox_guest->count(); i++) {
        if (ui->comboBox_guest->itemData(i).toInt() == reserve["guest_id"].toInt()) {
            ui->comboBox_guest->setCurrentIndex(i);
            break;
        }
    }
}
