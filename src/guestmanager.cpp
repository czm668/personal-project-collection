#include "guestmanager.h"
#include "dbmanager.h"
#include "logger.h"
#include <QSqlQuery>
#include <QSqlError>

GuestManager* GuestManager::m_instance = nullptr;

GuestManager::GuestManager()
{
    m_db = DbManager::getInstance()->getDatabase();
}

GuestManager* GuestManager::getInstance()
{
    if (m_instance == nullptr) {
        m_instance = new GuestManager();
    }
    return m_instance;
}

bool GuestManager::addGuest(const QString& name, const QString& idCard, const QString& phone,
                            const QString& address)
{
    // 检查身份证号重复
    if (isIdCardExists(idCard)) {
        Logger::getInstance()->logError("添加客户失败：身份证号 " + idCard + " 已存在");
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO guests (name, id_card, phone, address) "
                  "VALUES (:name, :id_card, :phone, :address)");
    query.bindValue(":name", name);
    query.bindValue(":id_card", idCard);
    query.bindValue(":phone", phone);
    query.bindValue(":address", address);

    if (!query.exec()) {
        Logger::getInstance()->logError("添加客户失败: " + query.lastError().text());
        return false;
    }

    Logger::getInstance()->log("添加客户成功：" + name + "（身份证：" + idCard + "）");
    return true;
}

QList<QVariantMap> GuestManager::getAllGuests()
{
    QList<QVariantMap> guests;
    QSqlQuery query(m_db);
    if (!query.exec("SELECT * FROM guests ORDER BY name")) {
        Logger::getInstance()->logError("获取客户列表失败: " + query.lastError().text());
        return guests;
    }

    while (query.next()) {
        QVariantMap guest;
        guest["id"] = query.value("id").toInt();
        guest["name"] = query.value("name").toString();
        guest["id_card"] = query.value("id_card").toString();
        guest["phone"] = query.value("phone").toString();
        guest["address"] = query.value("address").toString();
        guests.append(guest);
    }
    return guests;
}

QVariantMap GuestManager::getGuestById(int id)
{
    QVariantMap guest;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM guests WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        guest["id"] = query.value("id").toInt();
        guest["name"] = query.value("name").toString();
        guest["id_card"] = query.value("id_card").toString();
        guest["phone"] = query.value("phone").toString();
        guest["address"] = query.value("address").toString();
    }
    return guest;
}

bool GuestManager::updateGuest(int id, const QString& name, const QString& idCard, const QString& phone,
                               const QString& address)
{
    // 检查身份证号重复（排除当前ID）
    if (isIdCardExists(idCard, id)) {
        Logger::getInstance()->logError("修改客户失败：身份证号 " + idCard + " 已存在");
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("UPDATE guests SET name = :name, id_card = :id_card, "
                  "phone = :phone, address = :address WHERE id = :id");
    query.bindValue(":name", name);
    query.bindValue(":id_card", idCard);
    query.bindValue(":phone", phone);
    query.bindValue(":address", address);
    query.bindValue(":id", id);

    if (!query.exec()) {
        Logger::getInstance()->logError("修改客户失败: " + query.lastError().text());
        return false;
    }

    Logger::getInstance()->log("修改客户成功：ID=" + QString::number(id));
    return true;
}

bool GuestManager::deleteGuest(int id)
{
    // 检查客户是否有关联预订
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM reservations WHERE guest_id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        Logger::getInstance()->logError("删除客户失败：该客户存在预订记录");
        return false;
    }

    // 删除客户
    query.prepare("DELETE FROM guests WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        Logger::getInstance()->logError("删除客户失败: " + query.lastError().text());
        return false;
    }

    Logger::getInstance()->log("删除客户成功：ID=" + QString::number(id));
    return true;
}

bool GuestManager::isIdCardExists(const QString& idCard, int excludeId)
{
    QSqlQuery query(m_db);
    if (excludeId == -1) {
        query.prepare("SELECT id FROM guests WHERE id_card = :id_card");
    } else {
        query.prepare("SELECT id FROM guests WHERE id_card = :id_card AND id != :id");
        query.bindValue(":id", excludeId);
    }
    query.bindValue(":id_card", idCard);
    query.exec();
    return query.next();
}
