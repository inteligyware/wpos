// file      :  productoptionsmoduledb.cpp
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "productoptionsmoduledb.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <iostream>
using namespace std;

ProductOptionsModuleDB::ProductOptionsModuleDB(const QString& con_name, const QString& con_param, const QString& db, const QString& user, const QString& pwd)
    : BasicDatabase(con_name, con_param, db, user, pwd)
{
}

ProductOptionsModuleDB::ProductOptionsModuleDB(const QString& con_name, XmlConfig* xml)
    : BasicDatabase(con_name, xml)
{
}

ProductOptionsModuleDB::ProductOptionsModuleDB(const QString& con_name, const QString& conf_file)
    : BasicDatabase(con_name, conf_file)
{
}

bool ProductOptionsModuleDB::existOption(const QString& option_type, const QString& option_name)
{
    QString sql = "SELECT prod_option ";
    sql += "FROM products_options_list ";
    sql += "WHERE (option_type = '" + option_type + "') AND ";
    sql += "(prod_option = '" + option_name + "');";

    QSqlQuery query(sql, dbHandle());
    return query.isActive() && query.next();
}

bool ProductOptionsModuleDB::existOptionType(const QString& option_type)
{
    QString sql = "SELECT option_type ";
    sql += "FROM option_types ";
    sql += "WHERE option_type = '" + option_type + "';";

    QSqlQuery query(sql, dbHandle());
    return query.isActive() && query.next();
}

bool ProductOptionsModuleDB::deleteOption(const QString& option_type, const QString& option_name)
{
    QString sql = "DELETE FROM products_options_list ";
    sql += "WHERE (option_type = '" + option_type + "') AND ";
    sql += "(prod_option = '" + option_name + "');";

    QSqlQuery query(sql, dbHandle());
    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "  Statement : " << sql.toStdString() << '\n';
        return false;
    default:
        cerr << query.lastError().text().toStdString();
        return false;
        break;
    }
}

bool ProductOptionsModuleDB::deleteOptionType(const QString& option_type)
{
    if (option_type.isEmpty()) {
        cerr << "Emty option type " << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    QString sql = "DELETE FROM option_types ";
    sql += "WHERE (option_type = '" + option_type + "');";
    QSqlQuery query(sql, dbHandle());

    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "  Statement : " << sql.toStdString() << '\n';
        return false;
    default:
        cerr << query.lastError().text().toStdString();
        return false;
        break;
    }
}

QString ProductOptionsModuleDB::getDescriptionOption(const QString& option_type, const QString& option_name)
{
    QString sql = "SELECT description ";
    sql += "FROM products_options_list ";
    sql += "WHERE option_type = '" + option_type + "' AND ";
    sql += "option_name = '" + option_name + "';";

    QString description;
    QSqlQuery query(sql, dbHandle());
    if (query.isActive() && query.next())
        description = query.value(0).toString();

    return description;
}

QString ProductOptionsModuleDB::getDescriptionOptionType(const QString& option_type)
{

    QString sql = "SELECT description ";
    sql += "FROM option_types ";
    sql += "WHERE option_type = '" + option_type + "'";

    QString description;
    QSqlQuery query(sql, dbHandle());
    if (query.isActive() && query.next())
        description = query.value(0).toString();

    return description;
}

ProductOptionData ProductOptionsModuleDB::getOption(const QString& option_type, const QString& option_name)
{
    QString sql = "SELECT option_type, prod_option, description ";
    sql += "FROM products_options_list ";
    sql += "WHERE (option_type = '" + option_type + "') AND ";
    sql += "(prod_option = '" + option_name + "');";

    ProductOptionData option;
    QSqlQuery query(sql, dbHandle());
    if (query.isActive() && query.next()) {
        option.option_type = query.value(0).toString();
        option.option_name = query.value(1).toString();
        option.description_option = query.value(2).toString();
    }
    return option;
}

QStringList ProductOptionsModuleDB::getOptions(const QString& option_type)
{
    if (option_type.isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QStringList {};
    }

    QString sql = "SELECT prod_option ";
    sql += "FROM products_options_list ";
    sql += "WHERE (option_type='" + option_type + "');";

    QStringList options;
    QSqlQuery query(sql, dbHandle());
    if (query.isActive()) {
        while (query.next())
            options.emplaceBack(query.value(0).toString());
    }
    return options;
}

QVector<ProductOptionData> ProductOptionsModuleDB::getAllOptions()
{
    QString sql = "SELECT option_type,prod_option, logo, description ";
    sql += "FROM products_options_list ";
    sql += "ORDER BY option_type; ";

    QVector<ProductOptionData> options;
    QSqlQuery query(sql, dbHandle());
    if (query.isActive()) {
        while (query.next()) {
            ProductOptionData option;
            option.option_type = (query.value(0).toString());
            option.option_name = (query.value(1).toString());
            options.emplaceBack(option);
        }
    }
    return options;
}

ProductOptionData
ProductOptionsModuleDB::getOptionType(const QString& option_type)
{
    if (option_type.isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return ProductOptionData {};
    }

    QString sql = "SELECT option_type, description ";
    sql += "FROM option_types ";
    sql += "WHERE option_type = '" + option_type + "';";

    ProductOptionData option;
    QSqlQuery query(sql, dbHandle());

    if (query.isActive() && query.next()) {
        option.option_type = query.value(0).toString();
        option.description_type = query.value(1).toString();
    }

    return option;
}

QStringList ProductOptionsModuleDB::getOptionTypes()
{
    QString sql = "SELECT option_type ";
    sql += "FROM option_types;";

    QStringList option_types;
    QSqlQuery query(sql, dbHandle());

    if (query.isActive()) {
        while (query.next())
            option_types.emplaceBack(query.value(0).toString());
    }
    return option_types;
}

bool ProductOptionsModuleDB::insertOption(
    const QString& option_type,
    const QString& option_name,
    const QString& option_description)
{
    if (option_type.isEmpty()) {
        cerr << "Warning : Empty Option type "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    if (option_name.isEmpty()) {
        cerr << "Warning : Empty name for option type "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    if (existOption(option_type, option_name))
        return updateOption(option_type, option_name, option_description);

    QString sql = "INSERT INTO products_options_list ";
    sql += "(option_type, prod_option, description) ";
    sql += "VALUES ('" + option_type + "', '";
    sql += option_name + "', '";
    sql += option_description + "');";

    QSqlQuery query(sql, dbHandle());

    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "  Statement : " << sql.toStdString() << '\n';
        return false;
    default:
        cerr << query.lastError().text().toStdString();
        return false;
        break;
    }
}

bool ProductOptionsModuleDB::insertOptionType(const QString& option_type,
    const QString& description_type)
{
    QString sql = "INSERT INTO option_types (option_type, description) ";
    sql += "VALUES ('" + option_type + "', '";
    sql += description_type + "');";

    if (existOptionType(option_type))
        return updateOptionType(option_type, description_type);

    QSqlQuery query(sql, dbHandle());
    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "  Statement : " << sql.toStdString() << '\n';
        return false;
    default:
        cerr << query.lastError().text().toStdString();
        return false;
        break;
    }
}

bool ProductOptionsModuleDB::updateOption(const QString& option_type,
    const QString& option_name,
    const QString& description_option)
{
    if (option_type.isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    if (option_name.isEmpty()) {
        cerr << "Empty name for option type "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    if (!existOption(option_type, option_name))
        return insertOption(option_type, option_name, description_option);

    QString sql = "UPDATE products_options_list ";
    sql += "SET description = '" + description_option + "' ";
    sql += "WHERE option_type = '" + option_type + "' AND ";
    sql += "prod_option = '" + option_name + "';";

    QSqlQuery query(sql, dbHandle());
    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "  Statement : " << sql.toStdString() << '\n';
        return false;
    default:
        cerr << query.lastError().text().toStdString();
        return false;
        break;
    }
}

bool ProductOptionsModuleDB::updateOptionType(const QString& option_type,
    const QString& description_type)
{
    if (option_type.isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    if (!existOptionType(option_type))
        return insertOptionType(option_type, description_type);

    QString sql = "UPDATE option_types ";
    sql += "SET description = '" + description_type + "' ";
    sql += "WHERE option_type = '" + option_type + "';";

    QSqlQuery query(sql, dbHandle());
    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "  Statement : " << sql.toStdString() << '\n';
        return false;
    default:
        cerr << query.lastError().text().toStdString();
        return false;
        break;
    }
}

bool ProductOptionsModuleDB::existProductAndOption(const QString& product_code,
    const QString& option_type,
    const QString& option_name)
{
    if (product_code.isEmpty()) {
        cerr << "Empty product code "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    if (option_type.isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    if (option_name.isEmpty()) {
        cerr << "Option type has empty name "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    QString sql = "SELECT product_code ";
    sql += "FROM init_prod_options ";
    sql += "WHERE (product_code='" + product_code + "') AND ";
    sql += "(option_type='" + option_type + "') AND ";
    sql += "(prod_option='" + option_name + "');";

    QSqlQuery query(sql, dbHandle());
    return query.isActive() && query.next();
}

QVector<ProductOptionData>
ProductOptionsModuleDB::getProductOptions(const QString& product_code)
{
    if (product_code.isEmpty()) {
        cerr << "Empty product code "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return QVector<ProductOptionData> {};
    }

    QString sql = "SELECT t.option_type, t.description, o.prod_option, o.description, i.value, i.is_default ";
    sql += "FROM option_types t, products_options_list o, init_prod_options i ";
    sql += "WHERE i.product_code = '" + product_code + "' AND ";
    sql += "i.option_type = t.option_type AND ";
    sql += "i.prod_option = o.prod_option;";

    QVector<ProductOptionData> options;
    QSqlQuery query(sql, dbHandle());
    if (query.isActive()) {
        while (query.next()) {
            ProductOptionData option;
            option.option_type = query.value(0).toString();
            option.description_type = query.value(1).toString();
            option.option_name = query.value(2).toString();
            option.description_option = query.value(3).toString();
            option.value = query.value(4).toDouble();

            QString is_default = query.value(5).toString();
            option.is_default = (is_default == "true" || is_default == "t");

            options.emplaceBack(option);
        }
    }
    return options;
}

bool ProductOptionsModuleDB::insertOptionToProduct(const QString& product_code,
    const QString& option_type,
    const QString& option_name,
    double value, const QString& is_default)
{
    if (option_type.isEmpty()) {
        cerr << "Empty option type"
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    if (!existOptionType(option_type)) {
        insertOptionType(option_type, "");
    }

    if (option_name.isEmpty()) {
        cerr << "Empty option type name"
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    if (!existOption(option_type, option_name)) {
        insertOption(option_type, option_name, "");
    }

    if (product_code.isEmpty()) {
        cerr << "Empty product code"
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    if (existProductAndOption(product_code, option_type, option_name))
        return updateOptionToProduct(product_code, option_type, option_name, value, is_default);

    QString sql = "INSERT INTO init_prod_options ";
    sql += "(product_code, option_type, ";
    sql += "prod_option, value, ";
    sql += "is_default) ";
    sql += "VALUES ('" + product_code + "', '" + option_type + "', ";
    sql += "'" + option_name + "', ";
    sql += "'" + QString::number(value) + "', ";

    if (is_default == "f"
        || is_default == "false"
        || is_default.isEmpty())
        sql += "'f');";

    if (is_default == "t" || is_default == "true")
        sql += "'t');";

    QSqlQuery query(sql, dbHandle());

    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "  Statement : " << sql.toStdString() << '\n';
        return false;
    default:
        cerr << query.lastError().text().toStdString();
        return false;
        break;
    }
}

bool ProductOptionsModuleDB::updateOptionToProduct(const QString& product_code,
    const QString& option_type,
    const QString& option_name,
    double value, const QString& is_default)
{
    if (product_code.isEmpty()
        || option_type.isEmpty()
        || option_name.isEmpty()) {
        cerr << "Empty parameters " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (!existProductAndOption(product_code, option_type, option_name))
        return insertOptionToProduct(product_code, option_type, option_name, value, is_default);

    QString sql = "UPDATE init_prod_options ";
    sql += "SET value = '" + QString::number(value) + "', ";

    if (is_default == "f"
        || is_default == "false"
        || is_default.isEmpty())
        sql += "is_default = 'f' ";

    if (is_default == "t" || is_default == "true")
        sql += "is_default = 't' ";

    sql += "WHERE product_code = '" + product_code + "' AND ";
    sql += "option_type = '" + option_type + "' AND ";
    sql += "prod_option = '" + option_name + "';";

    QSqlQuery query(sql, dbHandle());
    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "  Statement : " << sql.toStdString() << '\n';
        return false;
    default:
        cerr << query.lastError().text().toStdString();
        return false;
        break;
    }
}

bool ProductOptionsModuleDB::setAllProductOptionValue(
    const QString& option_type,
    const QString& option_name,
    const QString& value)
{
    if (option_type.isEmpty() || option_name.isEmpty()) {
        cerr << "Empty parameters " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (!existOptionType(option_type) || !existOption(option_type, option_name))
        return false;

    QString sql = "SELECT set_init_prod_options ('" + option_type + "', '" + option_name + "', '" + value + "');";
    QSqlQuery query(sql, dbHandle());

    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "  Statement : " << sql.toStdString() << '\n';
        return false;
    default:
        cerr << query.lastError().text().toStdString();
        return false;
        break;
    }
    return true;
}

bool ProductOptionsModuleDB::updateAllProductOptionValue(const QString& option_type, const QString& option_name, const QString& value)
{
    if (option_type.isEmpty() || option_name.isEmpty()) {
        cerr << "Empty parameters " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (!existOptionType(option_type) || !existOption(option_type, option_name))
        return false;

    QString sql = "SELECT update_init_prod_options ('" + option_type + "', '" + option_name + "', '" + value + "');";

    QSqlQuery query(sql, dbHandle());
    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "  Statement : " << sql.toStdString() << '\n';
        return false;
    default:
        cerr << query.lastError().text().toStdString();
        return false;
        break;
    }
    return true;
}

bool ProductOptionsModuleDB::deleteOptionToProduct(const QString& product_code, const QString& option_type, const QString& option_name)
{
    if (product_code.isEmpty()
        || option_type.isEmpty()
        || option_name.isEmpty()) {
        cerr << "Empty parameters " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    QString sql = "DELETE  from init_prod_options ";
    sql += "WHERE  product_code='" + product_code + "' AND option_type='" + option_type + "' AND prod_option='" + option_name + "';";

    QSqlQuery query(sql, dbHandle());
    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "  Statement : " << sql.toStdString() << '\n';
        return false;
    default:
        cerr << query.lastError().text().toStdString();
        return false;
        break;
    }
    return true;
}
