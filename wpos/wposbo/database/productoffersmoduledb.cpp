// file      :  productoffersmoduledb.cpp
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "productoffersmoduledb.h"

#include <QSqlError>
#include <QVariant>

#include <iostream>
using namespace std;

ProductOffersModuleDB::ProductOffersModuleDB(
    QString con_name,
    QString con_path,
    QString db,
    QString user,
    QString pwd)
    : BasicDatabase(con_name, con_path, db, user, pwd){}

ProductOffersModuleDB::ProductOffersModuleDB(
    const QString& con_name,
    XmlConfig* xml)
    : BasicDatabase(con_name, xml){}

ProductOffersModuleDB::ProductOffersModuleDB(
    const QString& con_name,
    const QString& conf_path)
    : BasicDatabase(con_name, conf_path){}

bool ProductOffersModuleDB::existOffer(
    const QString& offer_type,
    const QString& offer_name)
{
    if (offer_type.isEmpty()
        || !existOfferType(offer_type)
        || offer_name.isEmpty())
        return false;

    QString sql = "SELECT prod_offer ";
    sql += "FROM offers_list ";
    sql += "WHERE offer_type = '" + offer_type + "' AND ";
    sql += "prod_offer = '" + offer_name + "';";

    QSqlQuery query(sql, dbHandle());
    return query.isActive() && query.size() && query.next();
}

bool ProductOffersModuleDB::existOfferType(const QString& offer_type)
{
    if (offer_type.isEmpty()) return false;

    QString sql = "SELECT offer_type ";
    sql += "FROM offer_types ";
    sql += "WHERE offer_type = '" + offer_type + "';";

    QSqlQuery query(sql, dbHandle());
    return query.isActive() && query.size() && query.next();
}

bool ProductOffersModuleDB::deleteOffer(
    const QString& offer_type,
    const QString& offer_name)
{
    if (offer_type.isEmpty()
        || !existOfferType(offer_type)
        || offer_name.isEmpty()
        || !existOffer(offer_type, offer_name))
        return false;

    QString sql = "DELETE FROM offers_list ";
    sql += "WHERE (offer_type = '" + offer_type + "') AND ";
    sql += "(prod_offer = '" + offer_name + "');";

    QSqlQuery query(sql, dbHandle());
    return query.lastError().type() == QSqlError::NoError;
}

bool ProductOffersModuleDB::deleteOfferType(const QString& offer_type)
{
    if (offer_type.isEmpty()) {
        cerr << "Empty offer type "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    QString sql = "DELETE FROM offer_types ";
    sql += "WHERE (offer_type = '" + offer_type + "');";

    QSqlQuery query(sql, dbHandle());
    QSqlError error = query.lastError();

    if (error.type() != QSqlError::NoError) {
        cerr << "Error in query "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << sql.toStdString() << '\n';
        cerr << error.text().toStdString();
        return false;
    }
    return true;
}

QString ProductOffersModuleDB::getDescriptionOffer(
    const QString& offer_type,
    const QString& offer_name)
{
    QString sql = "SELECT description ";
    sql += "FROM offers_list ";
    sql += "WHERE offer_type = '" + offer_type + "' AND ";
    sql += "prod_offer = '" + offer_name + "';";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive() || !query.next()) {
        cerr << "Error in SQL statement " << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "         Statement : " << sql.toStdString() << '\n';
        return QString{};
    }

    return query.value(0).toString();
}

QString ProductOffersModuleDB::getDescriptionOfferType(
    const QString& offer_type)
{
    QString sql = "SELECT description ";
    sql += "FROM offer_types ";
    sql += "WHERE offer_type = '" + offer_type + "'";

    QSqlQuery query(sql, dbHandle());

    if (!query.isActive() ||  !query.next()) {
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "         Statement : " << sql.toStdString() << '\n';
        return QString{};
    }
    return query.value(0).toString();
}

QString ProductOffersModuleDB::getCppOperator(const QString& offer_type)
{
    QString sql = "SELECT cpp_operator ";
    sql += "FROM offer_types ";
    sql += "WHERE offer_type = '" + offer_type + "'";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive() || !query.next() ) {
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "         Statement : " << sql.toStdString() << '\n';
        return QString{};
    }

    return query.value(0).toString();
}

QString ProductOffersModuleDB::getLogoOffer(
    const QString& offer_type,
    const QString& offer_name)
{
    if (offer_type.isEmpty()
        || !existOfferType(offer_type)
        || offer_name.isEmpty()
        || !existOffer(offer_type, offer_name) )
        return QString{};

    QString sql = "SELECT logo ";
    sql += "FROM offers_list ";
    sql += "WHERE (offer_type = '" + offer_type + "') AND ";
    sql += "(prod_offer = '" + offer_name + "');";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive() || !query.next()) {
        cerr << "Error in SQL statement : "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "         Statement : " << sql.toStdString() << '\n';
        return QString{};
    }
    return query.value(0).toString();
}

ProductOfferData ProductOffersModuleDB::getOffer(
    const QString& offer_type,
    const QString& offer_name)
{
    if (offer_type.isEmpty()
        || !existOfferType(offer_type)
        || offer_name.isEmpty()
        || !existOffer(offer_type, offer_name))
        return ProductOfferData{};

    QString sql = "SELECT offer_type, prod_offer, description, logo ";
    sql += "FROM offers_list ";
    sql += "WHERE (offer_type = '" + offer_type + "') AND ";
    sql += "(prod_offer = '" + offer_name + "');";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive()) {
        cerr << "Error in SQL statement : "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << " Statement: " << sql.toStdString() << '\n';
        return ProductOfferData{};
    }

    ProductOfferData offer;
    if (query.next()) {
        offer.offer_type = query.value(0).toString();
        offer.offer_name = query.value(1).toString();
        offer.description_offer = query.value(2).toString();
        offer.logo = query.value(3).toString();
    }
    return offer;
}

QVector<ProductOfferData> ProductOffersModuleDB::getAllOffers()
{
    QString  sql = "SELECT offer_type, prod_offer, logo, description ";
    sql += "FROM offers_list ;";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive()) {
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "Statement : " << sql.toStdString() <<'\n';
        return QVector<ProductOfferData>{};
    }

    QVector<ProductOfferData> offers;
    while (query.next()) {
        ProductOfferData offer;
        offer.offer_type = query.value(0).toString();
        offer.offer_name = query.value(1).toString();
        offer.logo = query.value(2).toString();
        offer.description_offer = query.value(3).toString();
        offers.emplaceBack(offer);
    }
    return offers;
}

QVector<ProductOfferData> ProductOffersModuleDB::getOffers(
    const QString& offer_type)
{
    if (offer_type.isEmpty() || !existOfferType(offer_type))
        return QVector<ProductOfferData>{};

    QString sql = "SELECT t.offer_type, t.description, o.prod_offer, o.description, t.cpp_operator, o.logo ";
    sql += "FROM offers_list o, offer_types t ";
    sql += "WHERE (t.offer_type = '" + offer_type + "') AND ";
    sql += "(t.offer_type = o.offer_type);";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive()) {
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "         Statement : " << sql.toStdString() << '\n';

        return QVector<ProductOfferData>{};
    }

    QVector<ProductOfferData> offers;
    while (query.next()) {
        ProductOfferData offer;

        offer.offer_type = query.value(0).toString();
        offer.description_type = query.value(1).toString();
        offer.offer_name = query.value(2).toString();
        offer.description_offer = query.value(3).toString();
        offer.cpp_operator = query.value(4).toString();
        offer.logo = query.value(5).toString();

        offers.emplaceBack(offer);
    }

    return offers;
}

ProductOfferData ProductOffersModuleDB::getOfferType(
    const QString& offer_type)
{
    if (offer_type.isEmpty())
        return ProductOfferData{};

    QString sql = "SELECT offer_type, cpp_operator, description ";
    sql += "FROM offer_types ";
    sql += "WHERE offer_type = '" + offer_type + "';";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive() || !query.size() || !query.next() ) {
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "         Statement : " << sql.toStdString() << '\n';
        return ProductOfferData{};;
    }

    ProductOfferData offer;
    offer.offer_type = query.value(0).toString();
    offer.description_type = query.value(2).toString();
    offer.cpp_operator = query.value(1).toString();

    return offer;
}

QStringList ProductOffersModuleDB::getOfferTypes()
{
    QString sql = "SELECT offer_type ";
    sql += "FROM offer_types ";
    sql += "ORDER BY offer_type;";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive()) {
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "         Statement : " << sql.toStdString() << '\n';
        return QStringList{};
    }

    QStringList offer_types;
    while (query.next()) {
        offer_types.emplaceBack(query.value(0).toString());
    }
    return offer_types;
}

bool ProductOffersModuleDB::insertOffer(
    const QString& offer_type,
    const QString& offer_name,
    const QString& description,
    const QString& logo)
{
    if (offer_type.isEmpty()
        || !existOfferType(offer_type)
        || offer_name.isEmpty())
        return false;

    if (existOffer(offer_type, offer_name))
        return updateOffer(offer_type, offer_name, description, logo);

    QString sql = "INSERT INTO offers_list ";
    sql += "(offer_type, prod_offer, description, logo) ";
    sql += "VALUES ('" + offer_type + "', '";
    sql += offer_name + "', '";
    sql += description + "', '";
    sql += logo + "');";

    QSqlQuery query(sql, dbHandle());
    return query.lastError().type() == QSqlError::NoError;
}

bool ProductOffersModuleDB::insertOfferType(
    const QString& offer_type,
    const QString& description,
    const QString& cpp_operator)
{
    if (offer_type.isEmpty())
        return false;

    if (existOfferType(offer_type))
        return updateOfferType(offer_type, description, cpp_operator);

    QString sql = "INSERT INTO offer_types (offer_type, cpp_operator, description) ";
    sql += "VALUES ('" + offer_type + "', '";
    sql += cpp_operator + "', '";
    sql += description + "');";
    QSqlQuery query(sql, dbHandle());
    return query.lastError().type() == QSqlError::NoError;
}

int ProductOffersModuleDB::getNumOfferTypes()
{
    QString sql = "SELECT COUNT(offer_type) ";
    sql += "FROM offer_types;";

    int count = 0;
    QSqlQuery query(sql, dbHandle());
    if (query.next())
        count = query.value(0).toInt();
    return count;
}

bool ProductOffersModuleDB::updateOffer(
    const QString& offer_type,
    const QString& offer_name,
    const QString& description_offer,
    const QString& logo)
{
    if (offer_type.isEmpty()
        || !existOfferType(offer_type)
        || offer_name.isEmpty() )
        return false;

    QString sql = "UPDATE offers_list ";
    sql += "SET description = '" + description_offer + "', logo ='" + logo + "' ";
    sql += "WHERE offer_type = '" + offer_type + "' AND ";
    sql += "prod_offer = '" + offer_name + "';";

    if (!existOffer(offer_type, offer_name))
        return insertOffer(offer_type, offer_name, description_offer, logo);

    QSqlQuery query(sql, dbHandle());
    return query.lastError().type() == QSqlError::NoError;
}

bool ProductOffersModuleDB::updateOfferType(
    const QString& offer_type,
    const QString& description_type,
    const QString& cpp_operator)
{
    if (offer_type.isEmpty())
        return false;

    if (!existOfferType(offer_type))
        return insertOfferType(offer_type, description_type, cpp_operator);
    QString sql = "UPDATE offer_types ";
    sql += "SET description = '" + description_type + "', ";
    sql += "cpp_operator = '" + cpp_operator + "' ";
    sql += "WHERE offer_type = '" + offer_type + "';";

    QSqlQuery query(sql, dbHandle());
    return query.lastError().type() == QSqlError::NoError;
}

QVector<ProductOfferData> ProductOffersModuleDB::getProductOffers(
    const QString& product_code)
{
    if (product_code.isEmpty()) {
        cerr << "Codigo de producto vacio "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return QVector<ProductOfferData>{};
    }

    QString sql = "SELECT o.offer_type, t.description, t.cpp_operator, ";
    sql += "o.prod_offer, o.description, i.value, i.is_default, o.logo ";
    sql += "FROM init_prod_offers i, offers_list o, offer_types t ";
    sql += "WHERE i.product_code = '" + product_code + "' AND ";
    sql += "o.offer_type = i.offer_type AND ";
    sql += "o.prod_offer = i.prod_offer AND ";
    sql += "o.offer_type = t.offer_type;";

    QSqlQuery query(sql, dbHandle());

    if (!query.isActive())
        return QVector<ProductOfferData>{};

    QVector<ProductOfferData> offers;
    while (query.next()) {
        ProductOfferData offer;

        offer.offer_type = query.value(0).toString();
        offer.description_type = query.value(1).toString();
        offer.cpp_operator = query.value(2).toString();
        offer.offer_name = query.value(3).toString();
        offer.description_offer = query.value(4).toString();
        offer.value = (query.value(5).toString()).toDouble();
        offer.is_default = query.value(6).toBool();
        offer.logo = query.value(7).toString();

        offers.emplaceBack(offer);
    }
    return offers;
}

bool ProductOffersModuleDB::existProductAndOffer(
    const QString& product_code,
    const QString& offer_type,
    const QString& offer_name)
{
    if (product_code.isEmpty()
        || offer_type.isEmpty()
        || !existOfferType(offer_type)
        || offer_name.isEmpty()
        || !existOffer(offer_type, offer_name) )
        return false;

    QString sql = "SELECT product_code ";
    sql += "FROM init_prod_offers ";
    sql += "WHERE (product_code='" + product_code + "') AND ";
    sql += "(offer_type='" + offer_type + "') AND ";
    sql += "(prod_offer='" + offer_name + "');";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive()) {
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << "         Statement : " << sql.toStdString() << '\n';
        return false;
    }    return query.next();
}

bool ProductOffersModuleDB::insertOfferToProduct(
    const QString& product_code,
    const QString& offer_type,
    const QString& offer_name,
    double value,
    const QString& is_default)
{

    if (product_code.isEmpty()
        || offer_type.isEmpty()
        || !existOfferType(offer_type)
        || offer_name.isEmpty()
        || !existOffer(offer_type, offer_name) )
        return false;

    if (existProductAndOffer(product_code, offer_type, offer_name))
        return updateOfferToProduct(
            product_code, offer_type,
            offer_name, value, is_default);

    QString sql = "INSERT INTO init_prod_offers ";
    sql += "(product_code, offer_type, ";
    sql += "prod_offer, value, is_default) ";
    sql += "VALUES ('" + product_code + "', '";
    sql += offer_type + "', ";
    sql += "'" + offer_name + "', ";
    sql += "'" + QString::number(value) + "', ";
    sql += "'" + is_default + "');";

    QSqlQuery query(sql, dbHandle());
    return query.lastError().type() == QSqlError::NoError;
}

bool ProductOffersModuleDB::deleteOfferToProduct(
    const QString& product_code,
    const QString& offer_type,
    const QString& offer_name)
{
    if (product_code.isEmpty()
        || offer_type.isEmpty()
        || offer_name.isEmpty())
        return false;

    QString sql = "DELETE  from init_prod_offers ";
    sql += "WHERE  product_code='" + product_code + "' AND offer_type='" + offer_type + "' AND prod_offer='" + offer_name + "';";

    QSqlQuery query(sql, dbHandle());
    if (query.lastError().type() != QSqlError::NoError) {
        cerr << "fallo al efectuar la query "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << sql.toStdString() << '\n';
        return false;
    }

    return true;
}

bool ProductOffersModuleDB::updateOfferToProduct(
    const QString& product_code,
    const QString& offer_type,
    const QString& offer_name,
    double value,
    const QString& is_default)
{
    if (product_code.isEmpty()
        || offer_type.isEmpty()
        || !existOfferType(offer_type)
        || offer_name.isEmpty()
        || !existOffer(offer_type, offer_name))
        return false;

    if (!existProductAndOffer(product_code, offer_type, offer_name))
        return insertOfferToProduct(product_code, offer_type, offer_name, value, is_default);

    QString sql = "UPDATE init_prod_offers ";
    sql += "SET value = '" + QString::number(value) + "', ";
    sql += "is_default = '" + is_default + "' ";
    sql += "WHERE product_code = '" + product_code + "' AND ";
    sql += "offer_type = '" + offer_type + "' AND ";
    sql += "prod_offer = '" + offer_name + "';";

    QSqlQuery query(sql, dbHandle());
    return query.lastError().type() == QSqlError::NoError;
}

bool ProductOffersModuleDB::setAllProductOfferValue(
    const QString& offer_type,
    const QString& offer_name,
    const QString& value)
{
    if (offer_type.isEmpty()
        || !existOfferType(offer_type)
        || offer_name.isEmpty()
        || !existOffer(offer_type, offer_name))
        return false;

    QString sql = "SELECT set_init_prod_offers ('" + offer_type + "', '" + offer_name + "', '" + value + "');";

    QSqlQuery query(sql, dbHandle());

    if (query.lastError().type() != QSqlError::NoError) {
        cerr << "Error while running the query "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << sql.toStdString() << '\n';
        return false;
    }

    return true;
}

bool ProductOffersModuleDB::updateAllProductOfferValue(
    const QString& offer_type,
    const QString& offer_name,
    const QString& value)
{
    if (offer_type.isEmpty()
        || !existOfferType(offer_type)
        || offer_name.isEmpty()
        || !existOffer(offer_type, offer_name) )
        return false;

    QString sql = "SELECT update_init_prod_offers ('" + offer_type + "', '" + offer_name + "', '" + value + "');";

    QSqlQuery query(sql, dbHandle());
    if (query.lastError().type() != QSqlError::NoError) {
        cerr << "Error while running the query "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        cerr << sql.toStdString() << '\n';
        return false;
    }

    return true;
}
