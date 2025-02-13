// file      :  productmodule.cpp
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "productmodule.h"
#include "productmodule_adaptor.h"

#include <QSqlError>
#include <QSqlQuery>

#include <iostream>
using namespace std;

static const QString& PRODUCTS_LIST_DTD = "dtddocs:products_productslist.dtd";
static const QString& FAMILIES_LIST_DTD = "dtddocs:products_familieslist.dtd";
static const QString& TAXES_LIST_DTD = "dtddocs:products_taxeslist.dtd";
static const QString& COMPOSITION_DTD = "dtddocs:products_composition.dtd";

const QString ProductModule::DBusObjectPath = QString { "/wpos/wposbo/DBusBOProduct" };

ProductModule::ProductModule(QObject* parent, const QString& name)
    : QObject(parent)
{
    setObjectName(name);
    database.connect();
    options_db.connect();
    new DBusBOProductAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(ProductModule::DBusObjectPath, this);
}

bool ProductModule::isUnitaryProduct(QString product_code)
{
    return database.isUnitaryProduct(product_code);
}

bool ProductModule::deleteProduct(const QString& product_code)
{
    bool deleted = false;
    if (product_code.isEmpty()) {
        cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    deleted = database.deleteProduct(product_code);
    if (deleted)  emit productDeleted(product_code);
    return deleted;
}

QString ProductModule::getLogo(const QString& product_name)
{
    ProductData product = database.getLogo(product_name);

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("products");
    xml.createElementSetDomain("product");
    xml.createElement("code", product.code);
    xml.createElement("name", product.name);
    xml.createElement("logo", product.logo);
    xml.releaseDomain("product", false);
    xml.releaseDomain("products", true);

    return  xml.toString();
}

QString ProductModule::getLogoFromProductCode(const QString& product_code)
{
    return database.getProduct(product_code).logo;
}

QString ProductModule::getLogos(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert the sitring into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QString{};
    }

    if (!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD)) {
        cerr << "XML does not validate against DTD" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QString{};
    }
    xml.delDomain();
    xml.setDomain("products[0]");

    QStringList products_name;
    for (auto i = 0; i < xml.howManyTags("product"); i++)
        products_name.emplaceBack(xml.readString("product[" + QString::number(i) + "].name"));

    QVector<ProductData> products = database.getLogos(products_name);
    XmlConfig xml_product;
    xml_product.createElementSetDomain("products");

    for (const ProductData& product : products) {
        xml_product.createElementSetDomain("product");
        xml_product.createElement("code", product.code);
        xml_product.createElement("name", product.name);
        xml_product.createElement("logo", product.logo);
        xml_product.releaseDomain("product", false);
    }
    xml_product.releaseDomain("products", true);

    if (!xml_product.validateXmlWithDTD(PRODUCTS_LIST_DTD)) {
        cerr << "xml does not validate against DTD"
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QString{};
    }

    xml_product.delDomain();
    return xml_product.toString();;
}

QString ProductModule::getProduct(const QString& product_code)
{
    if (product_code.isEmpty()) {
        cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QString{};
    }

    XmlConfig xml;
    xml.delDomain();

    ProductData product = database.getProduct(product_code);
    xml.createElementSetDomain("products");
    xml.createElementSetDomain("product");
    xml.createElement("code", product.code);
    xml.createElement("name", product.name);
    xml.createElement("price", QString::number(product.price));
    xml.createElement("tax", product.tax);
    xml.createElement("logo", product.logo);
    xml.createElement("description", product.description);
    xml.releaseDomain("product", true);
    xml.releaseDomain("products", true);

    if (!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD, true)) {
        cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return QString{};
    }

    return xml.toString();
}

QString ProductModule::getProductName(const QString& product_code)
{
    return database.getProduct(product_code).name;
}

QString ProductModule::getProductLike(const QString& product_name)
{
    return database.getProductLike(product_name);
}

QString ProductModule::getFamilies()
{
    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("families");

    QStringList families = database.getFamilies();
    if (families.isEmpty()) {
        xml.releaseDomain("families");

        if (!xml.validateXmlWithDTD(FAMILIES_LIST_DTD, true)) {
            cerr << "xml does not validate against DTD" << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml.debug();
            return QString{};
        }
        return xml.toString();
    }
    for (const QString& family : families)
        xml.createElement("family", family);

    xml.releaseDomain("families", true);
    xml.delDomain();

    if (!xml.validateXmlWithDTD(FAMILIES_LIST_DTD, true)) {
        cerr << "xml does not validate against DTD"
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return QString{};
    }
    return xml.toString();
}

QString ProductModule::getTaxes()
{
    QString xml_string;
    xml_string = "";
    QStringList taxes = database.getTaxes();

    if (taxes.isEmpty()) {
        cerr << "there are no taxes "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QString{};
    }

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("taxes");
    for (const QString& taxe : taxes)
        xml.createElement("tax", taxe);
    xml.releaseDomain("taxes", true);
    xml.delDomain();

    if (!xml.validateXmlWithDTD(TAXES_LIST_DTD, true)) {
        cerr << "XML does not validate against DTD"
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return QString{};
    }
    return xml.toString();
}

QString ProductModule::getProductsExtend()
{
    QVector<ProductData> products = database.getProductsExtend();
    if (products.isEmpty()) {
        cerr << "there are no products " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QString{};
    }

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("products");
    for (const ProductData& product: products) {
        xml.createElementSetDomain("product");
        xml.createElement("code", product.code);
        xml.createElement("name", product.name);
        xml.createElement("price", QString::number(product.price));
        xml.createElement("tax", product.tax);
        xml.createElement("logo", product.logo);
        xml.createElement("description", product.description);
        xml.releaseDomain("product");
    }
    xml.releaseDomain("products");
    if (!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD)) {
        cerr << "xml does not validate against DTD"
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return QString{};
    }
    return xml.toString();
}

QString ProductModule::getProducts()
{
    QVector<ProductData> products = database.getProducts();

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("products");
    for (const ProductData& product : products){
        xml.createElementSetDomain("product");

        xml.createElement("code", product.code);
        xml.createElement("name", product.name);
        xml.createElement("logo", product.logo);
        xml.releaseDomain("product");
    }
    xml.releaseDomain("products");
    if (!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD)) {
        cerr << "xml does not validate against DTD"
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return QString{};
    }
    return xml.toString();
}

QString ProductModule::getUnitaryProducts()
{
    QVector<ProductData> products = database.getUnitaryProducts();
    if (products.isEmpty()) {
        cerr << "there are no products "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QString{};
    }

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("products");
    for (const ProductData& product : products) {
        xml.createElementSetDomain("product");

        xml.createElement("code", product.code);
        xml.createElement("name", product.name);
        xml.createElement("price", QString::number(product.price));
        xml.createElement("tax", product.tax);
        xml.createElement("logo", product.logo);
        xml.createElement("description", product.description);

        xml.releaseDomain("product", false);
    }
    xml.releaseDomain("products", true);
    xml.delDomain();
    if (!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD, true)) {
        cerr << "xml does not validate against DTD"
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return QString{};
    }

    return xml.toString();
}

bool ProductModule::insertProduct(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "cannot convert the sitring into xml "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD, true)) {
        cerr << "xml does not validate against DTD"
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.setDomain("products[0]");
    xml.setDomain("product[0]");

    if (xml.readString("code").isEmpty()) {
        cerr << "Empty product code "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    ProductData product;
    product.code = xml.readString("code");

    if (xml.readString("name").isEmpty()) {
        cerr << "Nombre de producto vacio "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }
    product.name = xml.readString("name");
    product.price = (xml.readString("price")).toDouble();
    product.tax = xml.readString("tax");
    product.description = xml.readString("description");
    product.logo = xml.readString("logo");

    if (database.insertProduct(product)) {
        emit productCreated(product.code);
        return true;
    } else {
        return false;
    }
}

bool ProductModule::updateProduct(const QString& xml_string)
{
    bool updated = false;

    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert the string into xml "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD, true)) {
        cerr << "XLM does not validate against DTD"
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return false;
    }

    ProductData product;
    xml.setDomain("products[0]");
    xml.setDomain("product[0]");

    if (xml.readString("code").isEmpty()) {
        cerr << "Empty product code "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }
    product.code = xml.readString("code");

    if (xml.readString("name").isEmpty()) {
        cerr << "Nombre de producto vacio "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }
    product.name = xml.readString("name");
    product.price = xml.readString("price").toDouble();
    product.tax = xml.readString("tax");
    product.description = xml.readString("description");
    product.logo = xml.readString("logo");

    if (!database.existProduct(product.code))
        updated = database.insertProduct(product);
    else
        updated = database.updateProduct(product);

    if (updated)
        emit productUpdated(product.code);
    return updated;
}

QString ProductModule::getCompositions()
{
    QVector<ProductData> products = database.getCompositions();
    if (products.isEmpty()) {
        cerr << "there are no products "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QString{};
    }

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("products");
    for (const ProductData& product : products) {
        xml.createElementSetDomain("product");
        xml.createElement("code", product.code);
        xml.createElement("name", product.name);
        xml.createElement("price", QString::number(product.price));
        xml.createElement("tax", product.tax);
        xml.createElement("logo", product.logo);
        xml.createElement("description", product.description);
        xml.releaseDomain("product", false);
    }
    xml.releaseDomain("products", true);
    xml.delDomain();

    if (!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD, true)) {
        cerr << "xml does not validate against DTD"
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return QString {};
    }

    return xml.toString();
}

bool ProductModule::insertProductComposition(const QString& xml_string)
{
    XmlConfig xml;
    xml.delDomain();

    if (!xml.readXmlFromString(xml_string)) {
        cerr << "No se pudo convertir en xml el string "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return false;
    }

    if (!xml.validateXmlWithDTD(COMPOSITION_DTD, true)) {
        cerr << "xml does not validate against DTD"
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return false;
    }

    xml.setDomain("composition[0]");
    const QString& composition_code = xml.readString("composition_code");
    for (auto i = 0; i < xml.howManyTags("ingredient"); i++) {
        xml.setDomain("ingredient[" + QString::number(i) + "]");

        const QString& ingredient_code = xml.readString("ingredient_code");
        if (ingredient_code.isEmpty()) {
            cerr << "empty product code " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml.debug();
            xml.releaseDomain("ingredient", true);
            continue;
        }

        if (xml.readString("quantity").isEmpty()) {
            cerr << "empty product cuantity " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            xml.releaseDomain("ingredient", true);
            continue;
        }

        double quantity = xml.readString("quantity").toDouble();
        if (!database.insertProductComposition(composition_code, ingredient_code, quantity, i)) {
            return false;
        }
        xml.releaseDomain("ingredient", true);
    }

    return true;
}

bool ProductModule::updateProductComposition(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "cannot convert string into xml "
            << __PRETTY_FUNCTION__ << ": " << '\n';
        return false;
    }

    xml.delDomain();
    xml.setDomain("composition[0]");
    const QString& comp_code = xml.readString("composition_code");
    if (comp_code.isEmpty()) {
        cerr << "Empty composition code "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    for (auto i = 0; i < xml.howManyTags("ingredient"); i++) {
        xml.setDomain("ingredient[" + QString::number(i) + "]");

        const QString& ing_code = xml.readString("ingredient_code");
        if (ing_code.isEmpty()) {
            cerr << "component code "
                << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
            xml.releaseDomain("ingredient", true);
            continue;
        }

        if (xml.readString("quantity").isEmpty()) {
            cerr << "component cuantity "
                << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
            xml.releaseDomain("ingredient", true);
            continue;
        }

        if (!database.updateProductComposition(comp_code, ing_code,
                xml.readString("quantity").toDouble()) ) {
            return false;
        }
        xml.releaseDomain("ingredient", true);
    }

    return true;
}

bool ProductModule::existProductComposition(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "cannot convert the sitring into xml "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    xml.delDomain();
    xml.setDomain("composition[0]");
    const QString& comp_code = xml.readString("composition_code");
    if (comp_code.isEmpty()) {
        cerr << "empty composition code "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return false;
    }

    for (auto i = 0; i < xml.howManyTags("ingredient"); i++) {
        xml.setDomain("ingredient[" + QString::number(i) + "]");
        const QString& ing_code = xml.readString("ingredient_code");

        if (ing_code.isEmpty()) {
            cerr << "empty component code "
                << __PRETTY_FUNCTION__ << ": " << __LINE__ <<'\n';
            xml.releaseDomain("ingredient", true);
            continue;
        }

        if (!database.existProductComposition(comp_code, ing_code)) {
            return false;
        }
    }

    return true;
}

QString ProductModule::getIngredients(const QString& product_code)
{
    QVector<IngredientData>
    ingredients = database.getIngredients(product_code);

    if (ingredients.isEmpty()) {
        cerr << "product "
            << product_code.toStdString()
            << " does not have ingredients "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QString{};
    }

    XmlConfig xml;
    xml.createElementSetDomain("composition");
    xml.createElement("composition_code", product_code);

    for (const IngredientData& ingredient : ingredients) {
        xml.createElementSetDomain("ingredient");
        xml.createElement("ingredient_code", ingredient.ingredient_code);
        xml.createElement("quantity", QString::number(ingredient.quantity));
        xml.releaseDomain("ingredient");
    }
    xml.releaseDomain("composition");

    if (!xml.validateXmlWithDTD(COMPOSITION_DTD, true)) {
        cerr << "xml does not validate against DTD "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return QString{};
    }
    return xml.toString();
}

QString ProductModule::getCompositionsWithIngredient(const QString& ingredient_code)
{
    QString xml_string;
    QVector<ProductData> products = database.getCompositionsWithIngredient(ingredient_code);

    if (products.isEmpty()) {
        cerr << "there are no combinations "
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return QString{};
    }

    XmlConfig xml;
    xml.createElementSetDomain("products");
    if (products.isEmpty()) {
        xml.releaseDomain("products");
        return xml.toString();
    }

    for (const ProductData& product : products) {
        xml.createElementSetDomain("product");
        xml.createElement("code", product.code);
        xml.createElement("name", product.name);
        xml.releaseDomain("product", true);
    }
    xml.releaseDomain("products", true);

    if (!xml.validateXmlWithDTD(PRODUCTS_LIST_DTD, true)) {
        cerr << "xml does not validate against DTD"
            << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
    }

    return xml.toString();
}

QString ProductModule::getProductCodeByProductName(const QString& product_name)
{
    return  database.getProductCodeByProductName(product_name);
}

int ProductModule::getNumberOfProducts()
{
    return database.getNumberOfProducts();
}

void ProductModule::saveBarXml()
{
    XmlConfig xml(cfg::xmlFileByKey(cfg::XMLKey::Bar));
    xml.delDomain();
    while (xml.howManyTags("products"))
        xml.deleteElement("products");
    xml.createElementSetDomain("products");

    QVector<ProductData> products = database.getProducts();
    if (products.isEmpty()) {
        xml.save();
        return;
    }

    unsigned int i{0};
    for (const ProductData& product : products) {
        emit productAtXml(i++, product.name);
        QVector<IngredientData> ingredients = database.getIngredients(product.code);
        if ( ingredients.isEmpty())  continue;

        IngredientData ingredient = ingredients[0];
        int position = productPosition(ingredient.ingredient_code, xml);
        if (position < 0) {

            ProductData product = database.getProduct(ingredient.ingredient_code);
            xml.createElementSetDomain("product");
            xml.createElement("name", ingredient.ingredient_code);

            if (!product.logo.isEmpty())
                xml.createElement("icon", cfg::PRODUCT_DIR + product.logo);
        } else {
            xml.setDomain("product[" + QString::number(position) + "]");
        }

        if ( ingredients.size() > 1) {
            position = productPosition("solo", xml);
            if (position < 0)
                xml.createElement("product.name", "solo");
        }

        for (auto i = 1; i < ingredients.size(); i++) {
            position = productPosition(ingredients[i].ingredient_code, xml);
            if (position >= 0) {
                xml.setDomain("product[" + QString::number(i) + "]");
                continue;
            } else {
                xml.createElementSetDomain("product");
                xml.createElement("name", ingredients[i].ingredient_code);
            }
        }

        QVector<ProductOptionData> options = options_db.getProductOptions(product.code);
        writeOptions(options, xml);
        xml.releaseDomain("product");
    }

    xml.delDomain();
    xml.save();
}

void ProductModule::writeOptions(const QVector<ProductOptionData> &options, XmlConfig &xml)
{
    if ( options.isEmpty() ) return;

    xml.pushDomain();
    if (!xml.setDomain("options"))
        xml.createElementSetDomain("options");

    for (const ProductOptionData& option : options ) {
        auto j = xml.howManyTags("option");
        int i = 0;
        for ( i = 0; i < j; i++) {
            xml.setDomain("option[" + QString::number(i) + "]");
            if (xml.readString("type") == option.option_type)
                break;
            xml.releaseDomain("option", false);
        }

        if (i == j) {
            xml.createElementSetDomain("option");
            xml.createElement("type", option.option_type);
        }

        j = xml.howManyTags("value");
        for (i = 0; i < j; i++)
            if (xml.readString("value") == option.option_name)
                break;
        if (i == j)
            xml.createElement("value", option.option_name);

        if (option.is_default)
            xml.createAttribute("value[" + QString::number(i) + "]", "type", "default");
        xml.releaseDomain("option", false);
    }
    xml.releaseDomain("options");
    xml.popDomain();
}

int ProductModule::productPosition(const QString& name, XmlConfig &xml)
{
    // if the xml haven't this product's name, return a negative number
    int position = -1;
    for (auto i = 0; i < xml.howManyTags("product"); i++) {
        QString curr_name = xml.readString("product[" + QString::number(i) + "].name");
        if (curr_name == name) {
            position = i;
            break;
        }
    }
    return position;
}

void ProductModule::setProductAtPrinter(const QString& product_name, const QString& printer, bool on)
{
    database.setProductAtPrinter(product_name, printer, on);
}

bool ProductModule::getProductAtPrinter(const QString& product_name, const QString& printer)
{
    return  database.getProductAtPrinter(product_name, printer);
}
