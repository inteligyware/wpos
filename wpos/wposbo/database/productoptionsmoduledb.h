// file      :  productoptionsmoduledb.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef PRODUCTOPTIONSMODULEDB_H
#define PRODUCTOPTIONSMODULEDB_H

#include <wposcore/basicdatabase.h>

#include <QList>
#include <QString>

struct ProductOptionData {
    QString option_type;
    QString description_type;
    QString option_name;
    QString description_option;
    double value;
    bool is_default;

    bool isEmpty() const
    {
        return option_type.isEmpty()
            && description_type.isEmpty()
            && option_name.isEmpty()
            && description_option.isEmpty();
    }
};

class ProductOptionsModuleDB : public BasicDatabase {
public:
    ProductOptionsModuleDB(
        const QString& con_name, const QString& con_param, const QString& db, const QString& user, const QString& pwd);
    ProductOptionsModuleDB(const QString& con_name, XmlConfig* xml);
    ProductOptionsModuleDB(const QString& con_name, const QString& conf_file);

    bool existOption(const QString& option_type, const QString& option_name);

    bool existOptionType(const QString& option_type);

    bool deleteOption(const QString& option_type, const QString& option_name);

    bool deleteOptionType(const QString& option_type);

    QString getDescriptionOption(const QString& option_type, const QString& option_name);

    QString getDescriptionOptionType(const QString& option_type);

    ProductOptionData getOption(const QString& option_type, const QString& option_name);

    QStringList getOptions(const QString& option_type);

    ProductOptionData getOptionType(const QString& option_type);

    QVector<ProductOptionData> getAllOptions();

    QStringList getOptionTypes();

    bool insertOption(const QString& option_type, const QString& option_name,
        const QString& option_description);

    bool insertOptionType(const QString& option_type, const QString& description_type);

    bool updateOption(const QString& option_type, const QString& option_name,
        const QString& description_type);

    bool updateOptionType(const QString& option_type, const QString& description_type);

    bool existProductAndOption(const QString& product_code, const QString& option_type,
        const QString& option_name);

    QVector<ProductOptionData> getProductOptions(const QString& product_code);

    bool insertOptionToProduct(const QString& product_code, const QString& option_type,
        const QString& option_name, double value, const QString& is_default);

    bool updateOptionToProduct(const QString& product_code, const QString& option_type,
        const QString& option_name, double value, const QString& is_default);

    bool deleteOptionToProduct(const QString& product_code, const QString& option_type,
        const QString& option_name);

    bool setAllProductOptionValue(const QString& option_type, const QString& option_name, const QString& value);

    bool updateAllProductOptionValue(const QString& option_type, const QString& option_name, const QString& value);
};

#endif
