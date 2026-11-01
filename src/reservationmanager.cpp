#include "reservationmanager.h"
#include "dbmanager.h"
#include "logger.h"
#include "roommanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

ReservationManager* ReservationManager::m_instance = nullptr;

ReservationManager::ReservationManager()
{
    m_db = DbManager::getInstance()->getDatabase();
}

ReservationManager* ReservationManager::getInstance()
{
    if (m_instance == nullptr) {
        m_instance = new ReservationManager();
    }
    return m_instance;
}

bool ReservationManager::addReservation(int roomId, int guestId, const QDateTime& checkinDate,
                                        const QDateTime& checkoutDate, int createdBy)
{
    // 校验日期逻辑
    if (checkoutDate <= checkinDate) {
        Logger::getInstance()->logError("添加预订失败：退房日期必须晚于入住日期");
        return false;
    }

    // 校验房间可用性
    if (!isRoomAvailable(roomId, checkinDate, checkoutDate)) {
        Logger::getInstance()->logError("添加预订失败：该客房在指定时间段已被预订");
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO reservations (room_id, guest_id, checkin_date, checkout_date, status, created_by) "
                  "VALUES (:room_id, :guest_id, :checkin_date, :checkout_date, :status, :created_by)");
    query.bindValue(":room_id", roomId);
    query.bindValue(":guest_id", guestId);
    query.bindValue(":checkin_date", checkinDate);
    query.bindValue(":checkout_date", checkoutDate);
    query.bindValue(":status", "预订中");
    query.bindValue(":created_by", createdBy);

    if (!query.exec()) {
        Logger::getInstance()->logError("添加预订失败: " + query.lastError().text());
        return false;
    }

    // 更新客房状态为“已预订”
    QVariantMap room = RoomManager::getInstance()->getRoomById(roomId);
    RoomManager::getInstance()->updateRoom(roomId, room["room_number"].toString(),
                                           room["type"].toString(), room["price"].toDouble(),
                                           "已预订", room["description"].toString());

    Logger::getInstance()->log("添加预订成功：客房ID=" + QString::number(roomId) + "，客户ID=" + QString::number(guestId));
    return true;
}

QList<QVariantMap> ReservationManager::getAllReservations()
{
    QList<QVariantMap> reservations;
    QSqlQuery query(m_db);

    // 关联查询（客房+客户+创建人信息）
    QString sql = "SELECT r.*, rm.room_number, rm.type, g.name AS guest_name, "
                  "g.id_card AS guest_idcard, u.name AS creator "
                  "FROM reservations r "
                  "LEFT JOIN rooms rm ON r.room_id = rm.id "
                  "LEFT JOIN guests g ON r.guest_id = g.id "
                  "LEFT JOIN users u ON r.created_by = u.id "
                  "ORDER BY r.checkin_date DESC";

    if (!query.exec(sql)) {
        Logger::getInstance()->logError("获取预订列表失败: " + query.lastError().text());
        return reservations;
    }

    while (query.next()) {
        QVariantMap reserve;
        reserve["id"] = query.value("id").toInt();
        reserve["room_id"] = query.value("room_id").toInt();
        reserve["room_number"] = query.value("room_number").toString();
        reserve["room_type"] = query.value("type").toString();
        reserve["guest_id"] = query.value("guest_id").toInt();
        reserve["guest_name"] = query.value("guest_name").toString();
        reserve["guest_idcard"] = query.value("guest_idcard").toString();
        reserve["checkin_date"] = query.value("checkin_date").toDateTime();
        reserve["checkout_date"] = query.value("checkout_date").toDateTime();
        reserve["status"] = query.value("status").toString();
        reserve["creator"] = query.value("creator").toString();
        reservations.append(reserve);
    }
    return reservations;
}

QVariantMap ReservationManager::getReservationById(int id)
{
    QVariantMap reserve;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM reservations WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        reserve["id"] = query.value("id").toInt();
        reserve["room_id"] = query.value("room_id").toInt();
        reserve["guest_id"] = query.value("guest_id").toInt();
        reserve["checkin_date"] = query.value("checkin_date").toDateTime();
        reserve["checkout_date"] = query.value("checkout_date").toDateTime();
        reserve["status"] = query.value("status").toString();
    }
    return reserve;
}

bool ReservationManager::updateReservation(int id, int roomId, int guestId, const QDateTime& checkinDate,
                                           const QDateTime& checkoutDate, const QString& status)
{
    // 校验日期逻辑
    if (checkoutDate <= checkinDate) {
        Logger::getInstance()->logError("修改预订失败：退房日期必须晚于入住日期");
        return false;
    }

    // 校验房间可用性（排除当前预订ID）
    if (!isRoomAvailable(roomId, checkinDate, checkoutDate, id)) {
        Logger::getInstance()->logError("修改预订失败：该客房在指定时间段已被预订");
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("UPDATE reservations SET room_id = :room_id, guest_id = :guest_id, "
                  "checkin_date = :checkin_date, checkout_date = :checkout_date, status = :status "
                  "WHERE id = :id");
    query.bindValue(":room_id", roomId);
    query.bindValue(":guest_id", guestId);
    query.bindValue(":checkin_date", checkinDate);
    query.bindValue(":checkout_date", checkoutDate);
    query.bindValue(":status", status);
    query.bindValue(":id", id);

    if (!query.exec()) {
        Logger::getInstance()->logError("修改预订失败: " + query.lastError().text());
        return false;
    }

    // 更新客房状态（空闲/已预订）
    QString roomStatus = (status == "已取消" || status == "已完成") ? "空闲" : "已预订";
    QVariantMap room = RoomManager::getInstance()->getRoomById(roomId);
    RoomManager::getInstance()->updateRoom(roomId, room["room_number"].toString(),
                                           room["type"].toString(), room["price"].toDouble(),
                                           roomStatus, room["description"].toString());

    Logger::getInstance()->log("修改预订成功：ID=" + QString::number(id));
    return true;
}

bool ReservationManager::deleteReservation(int id)
{
    // 获取原预订的客房ID
    QVariantMap reserve = getReservationById(id);
    if (reserve.isEmpty()) {
        Logger::getInstance()->logError("删除预订失败：预订不存在");
        return false;
    }
    int roomId = reserve["room_id"].toInt();

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM reservations WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        Logger::getInstance()->logError("删除预订失败: " + query.lastError().text());
        return false;
    }

    // 更新客房状态为“空闲”
    QVariantMap room = RoomManager::getInstance()->getRoomById(roomId);
    RoomManager::getInstance()->updateRoom(roomId, room["room_number"].toString(),
                                           room["type"].toString(), room["price"].toDouble(),
                                           "空闲", room["description"].toString());

    Logger::getInstance()->log("删除预订成功：ID=" + QString::number(id));
    return true;
}

bool ReservationManager::isRoomAvailable(int roomId, const QDateTime& checkinDate,
                                         const QDateTime& checkoutDate, int excludeId)
{
    QSqlQuery query(m_db);
    QString sql = "SELECT id FROM reservations WHERE room_id = :room_id "
                  "AND status NOT IN ('已取消', '已完成') "
                  "AND ((checkin_date < :checkout_date AND checkout_date > :checkin_date)";
    if (excludeId != -1) {
        sql += " AND id != :exclude_id";
    }
    sql += ")";

    query.prepare(sql);
    query.bindValue(":room_id", roomId);
    query.bindValue(":checkin_date", checkinDate);
    query.bindValue(":checkout_date", checkoutDate);
    if (excludeId != -1) {
        query.bindValue(":exclude_id", excludeId);
    }

    query.exec();
    return !query.next(); // 无结果则表示房间可用
}
