// file      :  productsmoduledb.cpp
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "productsmoduledb.h"

#include <libbslxml/xmlconfig.h>

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <iostream>
using namespace std;

ProductsModuleDB::ProductsModuleDB(
    QString con_name,
    QString con_param,
    QString db,
    QString user,
    QString pwd)
    : BasicDatabase(con_name, con_param, db, user, pwd)
{
}

ProductsModuleDB::ProductsModuleDB(
    const QString& con_name,
    XmlConfig* xml)
    : BasicDatabase(con_name, xml)
{
}

ProductsModuleDB::ProductsModuleDB(
    const QString& con_name,
    const QString& con_param)
    : BasicDatabase(con_name, con_param)
{
}

bool ProductsModuleDB::existProduct(const QString& product_code)
{
    if (product_code.isEmpty()) {
        cerr << "Codigo del producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    QString sql = "SELECT COUNT(product_code) FROM products ";
    sql += "WHERE product_code = '" + product_code + "';";

    QSqlQuery query(sql, dbHandle());
    return (query.next() && query.value(0).toInt() > 0);
}

bool ProductsModuleDB::deleteProduct(const QString& product_code)
{
    if (product_code.isEmpty()) {
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    QString sql = "DELETE FROM products WHERE product_code = '" + product_code + "';";
    QSqlQuery query(sql, dbHandle());

    QSqlError error = query.lastError();
    return error.type() == QSqlError::NoError;
}

ProductData ProductsModuleDB::getProduct(const QString& product_code)
{
    ProductData product;

    // This check will be removed in order to query empty product_code
    // provided product_code is required NOT NULL in database
    if (product_code.isEmpty()) {
        cerr << "Codigo del producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return product;
    }

    QString sql = "SELECT product_code, product, price, tax, logo, description ";
    sql += "FROM products WHERE product_code = '" + product_code + "';";

    QSqlQuery query(sql, dbHandle());

    if (!query.isActive()) {
        return product;
    }

    if (query.first()) {
        product.code = query.value(0).toString();
        product.name = query.value(1).toString();
        product.price = query.value(2).toDouble();
        product.tax = query.value(3).toString();
        product.logo = query.value(4).toString();
        product.description = query.value(5).toString();
    }
    return product;
}

QString ProductsModuleDB::getProductLike(const QString& product_name)
{
    QString code = "";
    if (product_name.isEmpty())
        return code;

    QString sql = "SELECT product_code ";
    sql += "FROM products ";
    sql += "WHERE product LIKE '" + product_name + "%' ORDER BY product;";

    QSqlQuery query(sql, dbHandle());

    if (!query.isActive() || !query.size())
        return code;

    if (query.first())
        code = query.value(0).toString();
    return code;
}

QStringList ProductsModuleDB::getFamilies()
{
    QString sql;
    QSqlQuery query(sql, dbHandle());

    if (!query.isActive() || !query.size()) {
        return QStringList {};
    }

    QStringList families;
    while (query.next())
        families.emplaceBack(query.value(0).toString());
    return families;
}

ProductData ProductsModuleDB::getLogo(const QString& product_name)
{
    QString sql = "SELECT product_code, product, logo ";
    sql += "FROM products ";
    sql += "WHERE product = '" + product_name + "';";

    QSqlQuery query(sql, dbHandle());
    ProductData product;
    if (query.isActive() && query.next()) {
        product.code = query.value(0).toString();
        product.name = query.value(1).toString();
        product.logo = query.value(2).toString();
    }
    return product;
}

QVector<ProductData> ProductsModuleDB::getLogos(const QStringList& product_names)
{
    QVector<ProductData> products;
    for (const QString& product_name : product_names) {
        ProductData product = getLogo(product_name);
        products.emplaceBack(product);
    }
    return products;
}

QStringList ProductsModuleDB::getTaxes()
{
    QString sql = "SELECT tax ";
    sql += "FROM taxes ";
    sql += "ORDER BY tax;";

    QStringList taxes;
    QSqlQuery query(sql, dbHandle());
    if (query.isActive()) {
        while (query.next())
            taxes.emplaceBack(query.value(0).toString());
    }
    return taxes;
}

QVector<ProductData> ProductsModuleDB::getProductsExtend()
{
    QString sql = "SELECT product_code, product, price, tax, logo, description ";
    sql += "FROM products ";
    sql += "ORDER BY product;";

    QVector<ProductData> products;
    QSqlQuery query(sql, dbHandle());
    if (query.isActive()) {
        while (query.next()) {
            ProductData product;
            product.code = query.value(0).toString();
            product.name = query.value(1).toString();
            product.price = query.value(2).toString().toDouble();
            product.tax = query.value(3).toString();
            product.logo = query.value(4).toString();
            product.description = query.value(5).toString();
            products.emplaceBack(product);
        }
    }
    return products;
}

QVector<ProductData> ProductsModuleDB::getProducts()
{
    QString sql = "SELECT product_code, product, logo ";
    sql += "FROM products ";
    sql += "ORDER BY product;";

    QSqlQuery query(sql, dbHandle());
    QVector<ProductData> products;
    if (query.isActive()) {
        while (query.next()) {
            ProductData product;
            product.code = query.value(0).toString();
            product.name = query.value(1).toString();
            product.logo = query.value(2).toString();
            products.emplaceBack(product);
        }
    }
    return products;
}

QVector<ProductData> ProductsModuleDB::getUnitaryProducts()
{
    QString sql = "SELECT p.product_code, p.product, p.price, p.tax, p.logo, p.description ";
    sql += "FROM products p, prod_composition c ";
    sql += "WHERE p.product_code = c.product_code ";
    sql += "AND c.product_code = c.ingredient_code ";
    sql += "ORDER BY product;";

    QSqlQuery query(sql, dbHandle());
    QVector<ProductData> products;
    if (query.isActive()) {
        while (query.next()) {
            ProductData product;
            product.code = query.value(0).toString();
            product.name = query.value(1).toString();
            product.price = query.value(2).toDouble();
            product.tax = query.value(3).toString();
            product.logo = query.value(4).toString();
            product.description = query.value(5).toString();
            products.emplaceBack(product);
        }
    }
    return products;
}

bool ProductsModuleDB::isUnitaryProduct(const QString& product_code)
{
    int num_ingredient = 0;
    if (product_code.isEmpty()) {
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    QString sql = "SELECT COUNT(ingredient_code) ";
    sql += "FROM prod_composition ";
    sql += "WHERE product_code = '" + product_code + "';";

    QSqlQuery query(sql, dbHandle());

    if (!query.isActive()) {
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (query.next())
        num_ingredient = query.value(0).toInt();

    if (num_ingredient == 0)
        return false;

    if (num_ingredient == 1)
        return true;

    if (num_ingredient > 1)
        return false;
    return false;
}

bool ProductsModuleDB::insertProduct(const ProductData& product)
{
    if (product.code.isEmpty()) {
        cerr << "Codigo de producto vacio "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (product.name.isEmpty()) {
        cerr << "Nombre de producto vacio "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (existProduct(product.code))
        return updateProduct(product);

    QString sql = "INSERT INTO products (product_code, product, price, tax, description, logo) ";
    sql += "VALUES ('" + product.code + "', '" + product.name + "',";
    sql += " " + QString::number(product.price) + ", '" + product.tax + "',";
    sql += " '" + product.description + "', '" + product.logo + "');";

    QSqlQuery query(sql, dbHandle());
    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
        break;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << " Statement: " << sql.toStdString() << endl;
    default:
        return false;
        break;
    }
}

bool ProductsModuleDB::updateProduct(const ProductData& product)
{
    QSqlError error;

    if (product.code.isEmpty()) {
        cerr << "Codigo de producto vacio "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (product.name.isEmpty()) {
        cerr << "Nombre de producto vacio "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }
    // anadir la actualizacion del logo
    QString sql = "UPDATE products ";
    sql += "SET product = '" + product.name + "', ";
    sql += "description = '" + product.description + "', ";
    sql += "price = " + QString::number(product.price) + ", ";
    sql += "tax = '" + product.tax + "', ";
    sql += "logo = '" + product.logo + "' ";
    sql += "WHERE product_code = '" + product.code + "';";

    if (!existProduct(product.code))
        return insertProduct(product);

    QSqlQuery query(sql, dbHandle());

    error = query.lastError();
    switch (error.type()) {
    case QSqlError::NoError:
        return true;
        break;
    case QSqlError::StatementError:
        cerr << "Error in SQL statement "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << " Statement: " << sql.toStdString() << endl;
    default:
        return false;
        break;
    }
}

QVector<ProductData> ProductsModuleDB::getCompositions()
{
    QString sql = "SELECT DISTINCT p.product_code, p.product, p.price, p.tax, p.logo, p.description ";
    sql += "FROM products p, prod_composition c ";
    sql += "WHERE NOT(c.ingredient_code = c.product_code) ";
    sql += "AND c.product_code = p.product_code ";
    sql += "ORDER BY product;";

    QVector<ProductData> products;
    QSqlQuery query(sql, dbHandle());
    if (query.isActive()) {
        while (query.next()) {
            ProductData product;
            product.code = query.value(0).toString();
            product.name = query.value(1).toString();
            product.price = query.value(2).toDouble();
            product.tax = query.value(3).toString();
            product.logo = query.value(4).toString();
            product.description = query.value(5).toString();
            products.emplaceBack(product);
        }
    }
    return products;
}

bool ProductsModuleDB::insertProductComposition(const QString& composition_code,
    const QString& ingredient_code,
    double quantity,
    int ordering)
{
    if (existProductComposition(composition_code, ingredient_code))
        return updateProductComposition(composition_code, ingredient_code, quantity, ordering);

    if (composition_code.isEmpty()) {
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (ingredient_code.isEmpty()) {
        cerr << "Codigo de producto vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    QString sql = "INSERT INTO prod_composition (product_code, ingredient_code, quantity,ordering) ";
    sql += "VALUES ('" + composition_code + "', '" + ingredient_code + "', ";
    sql += QString::number(quantity, 'f', 2) + "," + QString::number(ordering) + ");";

    QSqlQuery query(sql, dbHandle());

    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
        break;
    case QSqlError::StatementError:
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
    default:
        return false;
        break;
    }
}

bool ProductsModuleDB::updateProductComposition(const QString& composition_code,
    const QString& ingredient_code,
    double quantity,
    int ordering)
{
    if (!existProductComposition(composition_code, ingredient_code))
        return insertProductComposition(composition_code, ingredient_code, quantity, ordering);

    if (composition_code.isEmpty()) {
        cerr << "Codigo de composicion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (ingredient_code.isEmpty()) {
        cerr << "Codigo de ingrediente vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (quantity == 0.00) {
        cerr << "Cantidad de ingrediente igual a 0" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    QString sql = "UPDATE prod_composition ";
    sql += "SET quantity = " + QString::number(quantity, 'f', 2) + ", ";
    sql += "ordering=" + QString::number(ordering) + " ";
    sql += "WHERE product_code = '" + composition_code + "' AND ";
    sql += "ingredient_code = '" + ingredient_code + "';";

    QSqlQuery query(sql, dbHandle());
    switch (query.lastError().type()) {
    case QSqlError::NoError:
        return true;
        break;
    case QSqlError::StatementError:
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
    default:
        return false;
        break;
    }
}

bool ProductsModuleDB::existProductComposition(const QString& composition_code,
    const QString& ingredient_code)
{
    if (composition_code.isEmpty()) {
        cerr << "Codigo de composicion vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (ingredient_code.isEmpty()) {
        cerr << "Codigo de ingrediente vacio " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    QString sql = "SELECT product_code ";
    sql += "FROM prod_composition ";
    sql += "WHERE product_code = '" + composition_code;
    sql += "' AND ingredient_code = '" + ingredient_code + "';";

    QSqlQuery query(sql, dbHandle());

    return query.isActive() && query.next();
}

QVector<IngredientData>
ProductsModuleDB::getIngredients(const QString& product_code)
{
    if (product_code.isEmpty())
        return QVector<IngredientData> {};

    QString sql = "SELECT ingredient_code, quantity ";
    sql += "FROM prod_composition ";
    sql += "WHERE (product_code = '" + product_code + "') ORDER BY ordering;";

    QSqlQuery query(sql, dbHandle());

    if (!query.isActive() || !query.size()) {
        cerr << "No esta activa "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QVector<IngredientData> {};
    }

    QVector<IngredientData> ingredients;
    while (query.next()) {
        IngredientData ingredient;
        ingredient.ingredient_code = query.value(0).toString();
        ingredient.quantity = query.value(1).toDouble();
        ingredients.emplaceBack(ingredient);
    }
    return ingredients;
}

QVector<ProductData>
ProductsModuleDB::getCompositionsWithIngredient(const QString& ingredient_code)
{
    QString sql = "SELECT  p.product_code, p.product, p.logo  ";
    sql += "FROM products p, prod_composition c ";
    sql += "WHERE c.ingredient_code = '" + ingredient_code + "' AND ";
    sql += "NOT c.ingredient_code = c.product_code AND ";
    sql += "c.product_code = p.product_code ";
    sql += "ORDER BY p.product_code;";

    QVector<ProductData> products;
    QSqlQuery query(sql, dbHandle());
    if (query.isActive()) {
        while (query.next()) {
            ProductData product;
            product.code = query.value(0).toString();
            product.name = query.value(1).toString();
            product.logo = query.value(2).toString();
            products.emplaceBack(product);
        }
    }
    return products;
}

QString ProductsModuleDB::getProductCodeByProductName(const QString& product_name)
{
    QString sql = "SELECT product_code from products where product='" + product_name + "' ; ";
    QSqlQuery query(sql, this->dbHandle());

    if (!query.isActive() || !query.size()) {
        return QString();
    }

    query.first();
    return query.value(0).toString();
}

int ProductsModuleDB::getNumberOfProducts()
{
    QString sql = "SELECT count(product_code) FROM products where product_code!='varios';";

    QSqlQuery query(sql, dbHandle());
    if (!query.isActive() || !query.size()) {
        return -1;
    }
    query.first();
    return query.value(0).toInt();
}

void ProductsModuleDB::setProductAtPrinter(const QString& product_name, const QString& printer, bool on)
{
    QString product_code;
    product_code = getProductCodeByProductName(product_name);

    QString sql;
    if (on) {
        sql = "INSERT INTO product_printer_types (product_code,printer_type) ";
        sql += "VALUES ('" + product_code + "','" + printer + "') ;";
    } else {
        sql = "DELETE FROM product_printer_types WHERE ";
        sql += "product_code='" + product_code + "' AND printer_type='" + printer + "' ;";
    }

    QSqlQuery query(sql, dbHandle());
    switch (query.lastError().type()) {
    case QSqlError::NoError:
        break;
    case QSqlError::StatementError:
        cerr << "Error en la sentencia sql " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        cerr << "         Sentencia: " << sql.toStdString() << endl;
        break;
    default:
        break;
    }
}

bool ProductsModuleDB::getProductAtPrinter(const QString& product_name, const QString& printer)
{

    QString product_code = getProductCodeByProductName(product_name);

    QString sql = "SELECT product_code FROM product_printer_types ";
    sql += "WHERE product_code='" + product_code + "' AND printer_type='" + printer + "';";

    QSqlQuery query(sql, dbHandle());

    return query.isActive() && query.size() > 0;
}
