#include "roommanager.h"
#include "dbmanager.h"
#include "logger.h"
#include <QSqlQuery>
#include <QSqlError>

RoomManager* RoomManager::m_instance = nullptr;

RoomManager::RoomManager()
{
    m_db = DbManager::getInstance()->getDatabase();
}

RoomManager* RoomManager::getInstance()
{
    if (m_instance == nullptr) {
        m_instance = new RoomManager();
    }
    return m_instance;
}

bool RoomManager::addRoom(const QString& roomNumber, const QString& type, double price,
                          const QString& status, const QString& description)
{
    // 检查房间号重复
    if (isRoomNumberExists(roomNumber)) {
        Logger::getInstance()->logError("添加客房失败：房间号 " + roomNumber + " 已存在");
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO rooms (room_number, type, price, status, description) "
                  "VALUES (:room_number, :type, :price, :status, :description)");
    query.bindValue(":room_number", roomNumber);
    query.bindValue(":type", type);
    query.bindValue(":price", price);
    query.bindValue(":status", status);
    query.bindValue(":description", description);

    if (!query.exec()) {
        Logger::getInstance()->logError("添加客房失败: " + query.lastError().text());
        return false;
    }

    Logger::getInstance()->log("添加客房成功：" + roomNumber);
    return true;
}

QList<QVariantMap> RoomManager::getAllRooms()
{
    QList<QVariantMap> rooms;
    QSqlQuery query(m_db);
    if (!query.exec("SELECT * FROM rooms ORDER BY room_number")) {
        Logger::getInstance()->logError("获取客房列表失败: " + query.lastError().text());
        return rooms;
    }

    while (query.next()) {
        QVariantMap room;
        room["id"] = query.value("id").toInt();
        room["room_number"] = query.value("room_number").toString();
        room["type"] = query.value("type").toString();
        room["price"] = query.value("price").toDouble();
        room["status"] = query.value("status").toString();
        room["description"] = query.value("description").toString();
        rooms.append(room);
    }
    return rooms;
}

QVariantMap RoomManager::getRoomById(int id)
{
    QVariantMap room;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM rooms WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        room["id"] = query.value("id").toInt();
        room["room_number"] = query.value("room_number").toString();
        room["type"] = query.value("type").toString();
        room["price"] = query.value("price").toDouble();
        room["status"] = query.value("status").toString();
        room["description"] = query.value("description").toString();
    }
    return room;
}

bool RoomManager::updateRoom(int id, const QString& roomNumber, const QString& type, double price,
                             const QString& status, const QString& description)
{
    // 检查房间号重复（排除当前ID）
    if (isRoomNumberExists(roomNumber, id)) {
        Logger::getInstance()->logError("修改客房失败：房间号 " + roomNumber + " 已存在");
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("UPDATE rooms SET room_number = :room_number, type = :type, "
                  "price = :price, status = :status, description = :description WHERE id = :id");
    query.bindValue(":room_number", roomNumber);
    query.bindValue(":type", type);
    query.bindValue(":price", price);
    query.bindValue(":status", status);
    query.bindValue(":description", description);
    query.bindValue(":id", id);

    if (!query.exec()) {
        Logger::getInstance()->logError("修改客房失败: " + query.lastError().text());
        return false;
    }

    Logger::getInstance()->log("修改客房成功：ID=" + QString::number(id));
    return true;
}

bool RoomManager::deleteRoom(int id)
{
    // 检查客房是否有关联预订
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM reservations WHERE room_id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        Logger::getInstance()->logError("删除客房失败：该客房存在预订记录");
        return false;
    }

    // 删除客房
    query.prepare("DELETE FROM rooms WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        Logger::getInstance()->logError("删除客房失败: " + query.lastError().text());
        return false;
    }

    Logger::getInstance()->log("删除客房成功：ID=" + QString::number(id));
    return true;
}

bool RoomManager::isRoomNumberExists(const QString& roomNumber, int excludeId)
{
    QSqlQuery query(m_db);
    if (excludeId == -1) {
        query.prepare("SELECT id FROM rooms WHERE room_number = :room_number");
    } else {
        query.prepare("SELECT id FROM rooms WHERE room_number = :room_number AND id != :id");
        query.bindValue(":id", excludeId);
    }
    query.bindValue(":room_number", roomNumber);
    query.exec();
    return query.next();
}
