// file      :  productoffersmoduledb.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef PRODUCTOFFERSMODULEDB_H
#define PRODUCTOFFERSMODULEDB_H

#include <libbslxml/xmlconfig.h>
#include <wposcore/basicdatabase.h>

#include <QList>
#include <QObject>
#include <QString>

struct ProductOfferData {
    ProductOfferData() = default;
    ProductOfferData(const ProductOfferData* data)
        : ProductOfferData(*data){}

    QString offer_type;
    QString description_type;
    QString cpp_operator;
    QString offer_name;
    QString description_offer;
    QString logo;
    double value { 0.0 };
    bool is_default { false };
    bool isNull() const{
        return offer_type.isEmpty()
            && offer_name.isEmpty()
            && description_type.isEmpty()
            && description_offer.isEmpty()
            && logo.isEmpty();
    }
};

class ProductOffersModuleDB : public BasicDatabase {
public:
    ProductOffersModuleDB(QString con_name,
        QString con_path,
        QString db,
        QString user,
        QString pwd);

    ProductOffersModuleDB(const QString& con_name,
        XmlConfig* xml);

    ProductOffersModuleDB(const QString& con_name,
        const QString& conf_path);

    bool existOffer(const QString& offer_type, const QString& offer_name);

    bool existOfferType(const QString& offer_type);

    bool deleteOffer(const QString& offer_type, const QString& offer_name);

    bool deleteOfferType(const QString& offer_type);

    QString getDescriptionOffer(const QString& offer_type, const QString& offer_name);

    QString getDescriptionOfferType(const QString& offer_type);

    QString getCppOperator(const QString& offer_type);

    QString getLogoOffer(const QString& offer_type, const QString& offer_name);

    ProductOfferData getOffer(const QString& offer_type, const QString& offer_name);

    QVector<ProductOfferData> getOffers(const QString& offer_type);

    QVector<ProductOfferData> getAllOffers();

    ProductOfferData getOfferType(const QString& offer_type);

    QStringList getOfferTypes();

    bool insertOffer(const QString& offer_type, const QString& offer_name, const QString& description, const QString& logo = "");

    bool insertOfferType(const QString& offer_type, const QString& description, const QString& cpp_operator);

    int getNumOfferTypes();

    bool updateOffer(const QString& offer_type, const QString& offer_name, const QString& description, const QString& logo = "");

    bool updateOfferType(const QString& offer_type, const QString& description, const QString& cpp_operator);

    QVector<ProductOfferData> getProductOffers(const QString& product_code);

    bool existProductAndOffer(const QString& product_code, const QString& offer_type, const QString& offer_name);

    bool insertOfferToProduct(const QString& product_code, const QString& offer_type,
        const QString& offer_name, double value, const QString& is_default = "f");

    bool updateOfferToProduct(const QString& product_code, const QString& offer_type,
        const QString& offer_name, double value, const QString& is_default = "f");

    bool deleteOfferToProduct(const QString& product_code, const QString& offer_type,
        const QString& offer_name);

    bool setAllProductOfferValue(const QString& offer_type, const QString& offer_name, const QString& value);

    bool updateAllProductOfferValue(const QString& offer_type, const QString& offer_name, const QString& value);
};

#endif
