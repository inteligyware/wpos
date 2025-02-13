// file      :  usersmoduledb.cpp
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "usersmoduledb.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

#include <iostream>
using namespace std;

bool UserData::isEmpty() const
{
    return employee_id.isEmpty()
        && name.isEmpty()
        && last_name.isEmpty()
        && address.isEmpty()
        && nss.isEmpty()
        && phone.isEmpty()
        && cellular.isEmpty()
        && email.isEmpty()
        && company_id.isEmpty()
        && picture.isEmpty()
        && !gerente
        && !active;
}

void UserData::clear()
{
    using std::swap;
    UserData tmp;
    swap(*this, tmp);
}

UsersModuleDB::UsersModuleDB(
    const QString& con_name,
    const QString& host,
    const QString& db,
    const QString& user,
    const QString& pwd)
    : BasicDatabase(con_name, host, db, user, pwd){}

UsersModuleDB::UsersModuleDB(
    const QString& con_name,
    XmlConfig* xml)
    : BasicDatabase(con_name, xml){}

UsersModuleDB::UsersModuleDB(
    const QString& con_name,
    const QString& conf_path)
    : BasicDatabase(con_name, conf_path){}

bool UsersModuleDB::addUser(const UserData& user_data)
{
    QString sql = "INSERT INTO staff ";
    sql += "(employee_id, name, last_name, address, nss, phone, cellular, email, company_id, picture, active ) ";
    sql += "VALUES ('" + user_data.employee_id + "', ";
    sql += "'" + user_data.name + "', '" + user_data.last_name + "', ";
    sql += "'" + user_data.address + "', '" + user_data.nss + "', ";
    sql += "'" + user_data.phone + "', '" + user_data.cellular + "', ";
    sql += "'" + user_data.email + "', '" + user_data.company_id + "', ";
    sql += "'" + user_data.picture + "', ";
    sql += "'t') ;";

    QSqlQuery query(sql, dbHandle());
    QSqlError error = query.lastError();
    if (error.type() != QSqlError::NoError) {
        cerr << "Error at UsersModuleDB"
             << __PRETTY_FUNCTION__ << ";"
             << __LINE__ << " " << error.text().toStdString() << '\n';
    }

    setUserAdminPermission(user_data.employee_id, user_data.gerente);
    return ((bool)!error.type());
}

bool UsersModuleDB::updateUser(const UserData& user_data)
{
    QString sql = "UPDATE staff SET name = '" + user_data.name + "', ";
    sql += "last_name = '" + user_data.last_name + "', ";
    sql += "address = '" + user_data.address + "', ";
    sql += "nss = '" + user_data.nss + "', ";
    sql += "phone = '" + user_data.phone + "', ";
    sql += "cellular = '" + user_data.cellular + "', ";
    sql += "email = '" + user_data.email + "', ";
    sql += "picture = '" + user_data.picture + "', ";
    sql += "company_id = '" + user_data.company_id + "' ";
    sql += "WHERE employee_id ='" + user_data.employee_id + "';";

    QSqlQuery query (sql, dbHandle());
    QSqlError error = query.lastError();
    if (error.type() != QSqlError::NoError) {
        cerr << "Error at UsersModuleDB"
             << __PRETTY_FUNCTION__ << ";" << __LINE__ << " "
             << error.text().toStdString() << '\n';
        return false;
    }
    setUserAdminPermission(user_data.employee_id, user_data.gerente);
    return true;
}

bool UsersModuleDB::delUser(const QString& id)
{
    return setActiveFlag(id, false);
}

bool UsersModuleDB::realDelUser(const QString& id)
{
    QSqlError error;
    // the system user can't be deleted
    if (id == "1") return false;

    QString sql = "DELETE FROM staff WHERE employee_id='" + id + "';";
    if (getUserAdminPermission(id))
        setUserAdminPermission(id, false);

    QSqlQuery query(sql, dbHandle());
    error = query.lastError();
    if (error.type() != QSqlError::NoError) {
        cerr << "Error at UsersModuleDB"
             << __PRETTY_FUNCTION__ << ";" << __LINE__
             << " " << error.type() << endl;
        return false;
    }
    return true;
}

QVector<UserData> UsersModuleDB::getUserList(bool all)
{

       QString sql = "SELECT employee_id, name, last_name, address, nss, phone, cellular, email, company_id, picture ";
       if (all) sql += "FROM staff ORDER BY employee_id;";
       else sql += "FROM staff WHERE active='t' ORDER BY employee_id;";
    // QString sql = "SELECT s.employee_id, name, last_name, address, nss, phone, cellular, email, company_id, picture, permission FROM staff s, acl a WHERE s.employee_id = a.employee_id ORDER BY s.employee_id;";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive() || !query.size()) {
        return QVector<UserData>{};
    }

    QVector<UserData> users;
    while (query.next()) {
        UserData data;
        data.employee_id = query.value(0).toString();
        data.name = query.value(1).toString();
        data.last_name = query.value(2).toString();
        data.address = query.value(3).toString();
        data.nss = query.value(4).toString();
        data.phone = query.value(5).toString();
        data.cellular = query.value(6).toString();
        data.email = query.value(7).toString();
        data.company_id = query.value(8).toString();
        data.picture = query.value(9).toString();
        data.gerente = !query.value(10).toString().isEmpty();
        users.push_back(data);
    }

    return users;
}

UserData UsersModuleDB::getUserInfo(QString employee_id)
{

    QString sql = "SELECT employee_id, name, last_name, address, nss, phone, cellular, email, company_id, picture ";
    sql += "FROM staff WHERE employee_id='" + employee_id + "' ;";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive() || !query.size()) {
        return UserData();
    }

    query.first();
    UserData data;
    data.employee_id = query.value(0).toString();
    data.name = query.value(1).toString();
    data.last_name = query.value(2).toString();
    data.address = query.value(3).toString();
    data.nss = query.value(4).toString();
    data.phone = query.value(5).toString();
    data.cellular = query.value(6).toString();
    data.email = query.value(7).toString();
    data.company_id = query.value(8).toString();
    data.picture = query.value(9).toString();
    data.gerente = this->getUserAdminPermission(data.employee_id);
    return data;
}

bool UsersModuleDB::setUserAdminPermission(const QString& employee_id, bool permission)
{
    QSqlError error;
    bool aux_boolean;

    aux_boolean = getUserAdminPermission(employee_id);
    if (aux_boolean == permission)
        return true;

    if (permission) {
        QString sql = "INSERT INTO acl ";
        sql += "(permission, employee_id) ";
        sql += "VALUES ('administracion','" + employee_id + "');";

        QSqlQuery query(sql, dbHandle());
        error = query.lastError();
        if (error.type() != QSqlError::NoError) {
            // some errors ocurred
            cerr << "Error at UsersModuleDB"
                 << __PRETTY_FUNCTION__ << ";" << __LINE__ << " "
                 << error.text().toStdString() << endl;
        }
        return ((bool)!error.type());
    } else {
        QString sql = "DELETE FROM acl ";
        sql += "WHERE employee_id='" + employee_id + "');";

        QSqlQuery query(sql, this->dbHandle());
        error = query.lastError();
        if (error.type() != QSqlError::NoError) {
            // some errors ocurred
            cerr << "Error at UsersModuleDB"
                 << __PRETTY_FUNCTION__ << ";" << __LINE__ << " "
                 << error.text().toStdString() << endl;
        }
        return ((bool)!error.type());
    }
}

bool UsersModuleDB::getUserAdminPermission(const QString& employee_id)
{
    QString sql = "SELECT permission from acl ";
    sql += "WHERE employee_id='" + employee_id + "' and permission='administracion';";

    QSqlQuery query (sql, dbHandle());
    return  query.isActive() &&  !query.size() ;
}

bool UsersModuleDB::getActiveFlag(const QString& employee_id)
{
    QString sql = "SELECT active from staff ";
    sql += "WHERE employee_id='" + employee_id + ";";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive() || !query.size()) {
        return false;
    }

    return query.value(0).toBool();
}

bool UsersModuleDB::setActiveFlag(const QString& employee_id, bool flag)
{
    // the system user can't be deleted
    if (employee_id == "1") return false;

    QString sql;
    if (flag)
        sql = "UPDATE staff SET active='t' WHERE employee_id='" + employee_id + "';";
    else
        sql = "UPDATE staff SET active='f' WHERE employee_id='" + employee_id + "';";

    QSqlQuery query (sql, dbHandle());
    return query.lastError().type() == QSqlError::NoError;
}
