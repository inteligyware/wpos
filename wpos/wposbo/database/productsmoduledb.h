// file      :  productsmoduledb.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef PRODUCTSMODULEDB_H
#define PRODUCTSMODULEDB_H

#include <wposcore/basicdatabase.h>

#include <QString>

template <typename T>
class QList;
template <typename Key, typename Value>
class QMap;

template <typename Value>
using QPtrMap = QMap<QString, Value*>;

template <typename Value>
using QPtrList = QList<Value*>;

struct ProductData {
    ProductData(const ProductData* product)
        : ProductData(*product)
    {
    }
    ProductData() = default;
    //    ~ProductData() = default;
    //    ProductData(const ProductData& p) = default;
    //    ProductData& operator=(const ProductData& p) = default;

    QString code;
    QString name;
    QString tax;
    QString logo;
    QString description;
    double price { 0.0 };
};

struct IngredientData {
    QString ingredient_code;
    double quantity;
};

class ProductsModuleDB : public BasicDatabase {
public:
    ProductsModuleDB(QString con_name,
        QString con_param,
        QString db,
        QString user,
        QString pwd);

    ProductsModuleDB(const QString& con_name,
        XmlConfig* xml);

    ProductsModuleDB(const QString& con_name,
        const QString& con_param);

    virtual ~ProductsModuleDB() = default;

    bool existProduct(const QString& product_code);

    bool deleteProduct(const QString& product_code);

    ProductData getProduct(const QString& product_code);

    QString getProductLike(const QString& product_name);

    QStringList getFamilies();

    ProductData getLogo(const QString& product_name);

    QVector<ProductData> getLogos(const QStringList& product_names);

    QStringList getTaxes();

    QVector<ProductData> getProducts();

    QVector<ProductData> getProductsExtend();

    QVector<ProductData> getUnitaryProducts();

    bool isUnitaryProduct(const QString& product_code);

    bool insertProduct(const ProductData& product);

    bool updateProduct(const ProductData& product);

    QVector<ProductData> getCompositions();

    bool insertProductComposition(const QString& composition_code,
        const QString& ingredient_code,
        double quantity, int ordering = 0);

    bool updateProductComposition(const QString& composition_code,
        const QString& ingredient_code,
        double quantity, int ordering = 0);

    bool existProductComposition(const QString& compositon_code,
        const QString& ingredient_code);

    QVector<IngredientData> getIngredients(const QString& composition_code);

    QVector<ProductData> getCompositionsWithIngredient(const QString& ingredient_code);

    QString getProductCodeByProductName(const QString& product_name);

    int getNumberOfProducts();

    void setProductAtPrinter(const QString& product_name, const QString& printer, bool on);

    bool getProductAtPrinter(const QString& product_name, const QString& printer);
};

#endif
