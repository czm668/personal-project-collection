#include "querymanagement.h"
#include "ui_querymanagement.h"
#include "logger.h"
#include <QMessageBox>
#include <QDateTime>

QueryManagement::QueryManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryManagement),
    m_roomMgr(RoomManager::getInstance()),
    m_guestMgr(GuestManager::getInstance()),
    m_reserveMgr(ReservationManager::getInstance())
{
    ui->setupUi(this);
    // 初始化表格为客房查询模式
    initTable(0);

    // 设置日期默认值
    ui->dateEdit_start->setDate(QDate::currentDate().addDays(-30));
    ui->dateEdit_end->setDate(QDate::currentDate());
}

QueryManagement::~QueryManagement()
{
    delete ui;
}

void QueryManagement::initTable(int type)
{
    // 清空表格
    ui->tableWidget_results->clear();
    ui->tableWidget_results->setRowCount(0);

    QStringList headers;
    switch (type) {
    case 0: // 客房查询
        headers = {"ID", "房间号", "类型", "价格", "状态", "描述"};
        // 显示客房相关查询条件，隐藏其他条件
        ui->label_roomCondition->show();
        ui->lineEdit_roomCondition->show();
        ui->label_dateRange->hide();
        ui->dateEdit_start->hide();
        ui->dateEdit_end->hide();
        ui->label_guestCondition->hide();
        ui->lineEdit_guestCondition->hide();
        break;
    case 1: // 客户查询
        headers = {"ID", "客户姓名", "身份证号", "联系电话", "居住地址", "创建时间"};
        // 显示客户相关查询条件，隐藏其他条件
        ui->label_roomCondition->hide();
        ui->lineEdit_roomCondition->hide();
        ui->label_dateRange->hide();
        ui->dateEdit_start->hide();
        ui->dateEdit_end->hide();
        ui->label_guestCondition->show();
        ui->lineEdit_guestCondition->show();
        break;
    case 2: // 预订查询
        headers = {"ID", "客房号", "客户姓名", "入住日期", "退房日期", "预订状态", "创建人"};
        // 显示预订相关查询条件
        ui->label_roomCondition->show();
        ui->lineEdit_roomCondition->show();
        ui->label_dateRange->show();
        ui->dateEdit_start->show();
        ui->dateEdit_end->show();
        ui->label_guestCondition->show();
        ui->lineEdit_guestCondition->show();
        break;
    }

    ui->tableWidget_results->setColumnCount(headers.size());
    ui->tableWidget_results->setHorizontalHeaderLabels(headers);
    ui->tableWidget_results->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidget_results->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget_results->horizontalHeader()->setStretchLastSection(true);
}

void QueryManagement::refreshQueryResults()
{
    // 根据当前查询类型执行查询
    int type = ui->comboBox_queryType->currentIndex();
    switch (type) {
    case 0:
        queryRooms();
        break;
    case 1:
        queryGuests();
        break;
    case 2:
        queryReservations();
        break;
    }
}

void QueryManagement::clearQueryConditions()
{
    ui->lineEdit_roomCondition->clear();
    ui->lineEdit_guestCondition->clear();
    ui->dateEdit_start->setDate(QDate::currentDate().addDays(-30));
    ui->dateEdit_end->setDate(QDate::currentDate());
}

void QueryManagement::queryRooms()
{
    QString condition = ui->lineEdit_roomCondition->text().trimmed();
    QList<QVariantMap> allRooms = m_roomMgr->getAllRooms();
    QList<QVariantMap> results;

    // 过滤符合条件的客房
    for (const auto& room : allRooms) {
        if (condition.isEmpty() ||
            room["room_number"].toString().contains(condition) ||
            room["type"].toString().contains(condition) ||
            room["status"].toString().contains(condition) ||
            room["description"].toString().contains(condition)) {
            results.append(room);
        }
    }

    // 填充表格
    ui->tableWidget_results->setRowCount(0);
    for (int i = 0; i < results.size(); i++) {
        const auto& room = results[i];
        ui->tableWidget_results->insertRow(i);
        ui->tableWidget_results->setItem(i, 0, new QTableWidgetItem(QString::number(room["id"].toInt())));
        ui->tableWidget_results->setItem(i, 1, new QTableWidgetItem(room["room_number"].toString()));
        ui->tableWidget_results->setItem(i, 2, new QTableWidgetItem(room["type"].toString()));
        ui->tableWidget_results->setItem(i, 3, new QTableWidgetItem(QString::number(room["price"].toDouble(), 'f', 2)));
        ui->tableWidget_results->setItem(i, 4, new QTableWidgetItem(room["status"].toString()));
        ui->tableWidget_results->setItem(i, 5, new QTableWidgetItem(room["description"].toString()));
    }

    Logger::getInstance()->log("客房查询完成，找到 " + QString::number(results.size()) + " 条记录");
}

void QueryManagement::queryGuests()
{
    QString condition = ui->lineEdit_guestCondition->text().trimmed();
    QList<QVariantMap> allGuests = m_guestMgr->getAllGuests();
    QList<QVariantMap> results;

    // 过滤符合条件的客户
    for (const auto& guest : allGuests) {
        if (condition.isEmpty() ||
            guest["name"].toString().contains(condition) ||
            guest["id_card"].toString().contains(condition) ||
            guest["phone"].toString().contains(condition) ||
            guest["address"].toString().contains(condition)) {
            results.append(guest);
        }
    }

    // 填充表格
    ui->tableWidget_results->setRowCount(0);
    for (int i = 0; i < results.size(); i++) {
        const auto& guest = results[i];
        ui->tableWidget_results->insertRow(i);
        ui->tableWidget_results->setItem(i, 0, new QTableWidgetItem(QString::number(guest["id"].toInt())));
        ui->tableWidget_results->setItem(i, 1, new QTableWidgetItem(guest["name"].toString()));
        ui->tableWidget_results->setItem(i, 2, new QTableWidgetItem(guest["id_card"].toString()));
        ui->tableWidget_results->setItem(i, 3, new QTableWidgetItem(guest["phone"].toString()));
        ui->tableWidget_results->setItem(i, 4, new QTableWidgetItem(guest["address"].toString()));
        ui->tableWidget_results->setItem(i, 5, new QTableWidgetItem(guest["created_at"].toString()));
    }

    Logger::getInstance()->log("客户查询完成，找到 " + QString::number(results.size()) + " 条记录");
}

void QueryManagement::queryReservations()
{
    QString roomCond = ui->lineEdit_roomCondition->text().trimmed();
    QString guestCond = ui->lineEdit_guestCondition->text().trimmed();
    QDate startDate = ui->dateEdit_start->date();
    QDate endDate = ui->dateEdit_end->date();

    QList<QVariantMap> allReservations = m_reserveMgr->getAllReservations();
    QList<QVariantMap> results;

    // 过滤符合条件的预订
    for (const auto& reserve : allReservations) {
        QDate checkinDate = QDate::fromString(reserve["checkin_date"].toString(), "yyyy-MM-dd");

        bool dateMatch = (checkinDate >= startDate && checkinDate <= endDate);
        bool roomMatch = roomCond.isEmpty() || reserve["room_number"].toString().contains(roomCond);
        bool guestMatch = guestCond.isEmpty() ||
                          reserve["guest_name"].toString().contains(guestCond) ||
                          reserve["guest_id_card"].toString().contains(guestCond);

        if (dateMatch && roomMatch && guestMatch) {
            results.append(reserve);
        }
    }

    // 填充表格
    ui->tableWidget_results->setRowCount(0);
    for (int i = 0; i < results.size(); i++) {
        const auto& reserve = results[i];
        ui->tableWidget_results->insertRow(i);
        ui->tableWidget_results->setItem(i, 0, new QTableWidgetItem(QString::number(reserve["id"].toInt())));
        ui->tableWidget_results->setItem(i, 1, new QTableWidgetItem(reserve["room_number"].toString()));
        ui->tableWidget_results->setItem(i, 2, new QTableWidgetItem(reserve["guest_name"].toString()));
        ui->tableWidget_results->setItem(i, 3, new QTableWidgetItem(reserve["checkin_date"].toString()));
        ui->tableWidget_results->setItem(i, 4, new QTableWidgetItem(reserve["checkout_date"].toString()));
        ui->tableWidget_results->setItem(i, 5, new QTableWidgetItem(reserve["status"].toString()));
        ui->tableWidget_results->setItem(i, 6, new QTableWidgetItem(reserve["creator"].toString()));
    }

    Logger::getInstance()->log("预订查询完成，找到 " + QString::number(results.size()) + " 条记录");
}

void QueryManagement::on_pushButton_query_clicked()
{
    refreshQueryResults();
}

void QueryManagement::on_pushButton_reset_clicked()
{
    clearQueryConditions();
}

void QueryManagement::on_comboBox_queryType_currentIndexChanged(int index)
{
    initTable(index);
}
