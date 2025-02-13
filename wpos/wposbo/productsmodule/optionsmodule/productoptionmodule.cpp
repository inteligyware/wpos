// file      :  productoptionmodule.cpp
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "productoptionmodule.h"
#include "productoptionmodule_adaptor.h"

#include <QtDBus/QDBusConnection>

static const QString& OPTIONS_DTD = "dtddocs:products_optionslist.dtd";

#include <iostream>
using namespace std;

const QString ProductOptionModule::DBusObjectPath = QString { "/wpos/wposbo/DBusBOProductOption" };

ProductOptionModule::ProductOptionModule(QObject* parent, const QString& name)
    : QObject(parent)
{
    setObjectName(name);
    database.connect();
    new DBusBOProductOptionAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(ProductOptionModule::DBusObjectPath, this);
}

bool ProductOptionModule::deleteOption(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml" << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD " << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.delDomain();
    xml.setDomain("options[0].option[0]");
    if (xml.readString("option_type").isEmpty()) {
        cerr << "Empty option type " << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (xml.readString("option_name").isEmpty()) {
        cerr << "Empty option name " << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.deleteOption(xml.readString("option_type"), xml.readString("option_name"));
}

bool ProductOptionModule::deleteOptionType(const QString& option_type)
{

    if (option_type.isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.deleteOptionType(option_type);
}

bool ProductOptionModule::existOption(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML doesn't validate with DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.delDomain();
    xml.setDomain("options[0]");
    xml.setDomain("option[0]");

    if (xml.readString("option_type").isEmpty()
        || xml.readString("option_name").isEmpty()) {
        cerr << "Empty type or Name for option type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.existOption(
        xml.readString("option_type"),
        xml.readString("option_name"));
}

bool ProductOptionModule::existOptionType(const QString& option_type)
{
    return database.existOptionType(option_type);
}

QString ProductOptionModule::getDescriptionOption(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot conver xml into string"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString {};
    }

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD " << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString {};
    }
    xml.delDomain();
    xml.setDomain("options[0].option[0]");

    return database.getDescriptionOption(
        xml.readString("option_type"),
        xml.readString("option_name"));
}

QString ProductOptionModule::getDescriptionOptionType(const QString& option_type)
{
    return database.getDescriptionOptionType(option_type);
}

QString ProductOptionModule::getOption(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString {};
    }

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString {};
    }

    xml.delDomain();
    xml.setDomain("options[0].option[0]");
    if (xml.readString("option_type").isEmpty()) {
        cerr << "Empty option type " << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString {};
    }

    if (xml.readString("option_name").isEmpty()) {
        cerr << "Empty option name " << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString {};
    }

    ProductOptionData
        option
        = database.getOption(
            xml.readString("option_type"),
            xml.readString("option_name"));

    if (option.isEmpty()) {
        cerr << "Option does not exists " << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        cerr << "Option type: " << xml.readString("option_type").toStdString() << '\n';
        cerr << "Option name: " << xml.readString("option_name").toStdString() << '\n';
        return QString {};
    }

    {
        XmlConfig xml;
        xml.delDomain();
        xml.createElementSetDomain("options");

        if (option.isEmpty()) {
            xml.releaseDomain("options", true);
            xml.delDomain();
            return xml.toString();
        }

        xml.createElementSetDomain("option");
        xml.createElement("option_type", option.option_type);
        xml.createElement("option_name", option.option_name);
        xml.createElement("description_option", option.description_option);
        xml.releaseDomain("option", true);
        xml.releaseDomain("options", true);

        if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
            cerr << "XML does not validate against DTD "
                 << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
            xml.debug();
            return QString {};
        }

        return xml.toString();
    }
}

QString ProductOptionModule::getOptions(const QString& option_type)
{
    if (option_type.isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString {};
    }

    QStringList options = database.getOptions(option_type);
    if (options.isEmpty()) {
        cerr << "there are no options for this option type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        cerr << "Option type: " << option_type.toStdString() << '\n';
        return QString {};
    }
    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("options");
    for (const QString& option : options) {
        xml.createElementSetDomain("option");
        xml.createElement("option_type");
        xml.createElement("option_name", option);
        xml.releaseDomain("option");
    }
    xml.releaseDomain("options", true);

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString {};
    }

    return xml.toString();
}

QString ProductOptionModule::getAllOptions()
{
    QVector<ProductOptionData> options = database.getAllOptions();
    if (options.isEmpty())
        return QString {};

    XmlConfig xml;
    xml.createElementSetDomain("options");
    for (const ProductOptionData& option : options) {
        xml.createElementSetDomain("option");
        xml.createElement("option_type", option.option_type);
        xml.createElement("option_name", option.option_name);
        xml.releaseDomain("option");
    }
    xml.delDomain();
    return xml.toString();
}

QString ProductOptionModule::getOptionType(const QString& option_type)
{
    if (option_type.isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString {};
    }

    ProductOptionData option = database.getOptionType(option_type);

    if (option.isEmpty()) {
        cerr << "option type does not exists "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        cerr << "Option type: " << option_type.toStdString() << '\n';
        return QString {};
    }

    XmlConfig xml;
    xml.delDomain();

    xml.createElementSetDomain("options");
    xml.createElementSetDomain("option");
    xml.createElement("option_type", option.option_type);
    xml.createElement("description_type", option.description_type);
    xml.createElement("option_name");
    xml.releaseDomain("option", true);
    xml.releaseDomain("options", true);

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString {};
    }

    return xml.toString();
}

QString ProductOptionModule::getOptionTypes()
{
    QStringList options = database.getOptionTypes();
    if (options.isEmpty()) {
        cerr << "There are no option types "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString {};
    }

    XmlConfig xml;
    xml.createElement("product_code");
    xml.createElementSetDomain("options");

    for (const QString& option : options) {
        xml.createElementSetDomain("option");
        xml.createElement("option_type", option);
        xml.createElement("option_name");
        xml.releaseDomain("option", true);
    }
    xml.releaseDomain("option_types", true);

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString {};
    }
    return xml.toString();
}

bool ProductOptionModule::insertOption(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.delDomain();
    xml.setDomain("options.option");

    if (xml.readString("option_type").isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (xml.readString("option_name").isEmpty()) {
        cerr << "Empty option name "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.insertOption(
        xml.readString("option_type"),
        xml.readString("option_name"),
        xml.readString("description_option"));
}

bool ProductOptionModule::insertOptionType(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.delDomain();
    xml.setDomain("options[0].option");
    if (xml.readString("option_type").isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.insertOptionType(
        xml.readString("option_type"),
        xml.readString("description_type"));
}

bool ProductOptionModule::updateOption(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert xml into string "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.delDomain();
    xml.setDomain("options[0].option[0]");
    if (xml.readString("option_type").isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (xml.readString("option_name").isEmpty()) {
        cerr << "Empty option name "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.updateOption(
        xml.readString("option_type"),
        xml.readString("option_name"),
        xml.readString("description_option"));
}

bool ProductOptionModule::updateOptionType(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert xml into string "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.delDomain();
    xml.setDomain("options[0].option[0]");
    if (xml.readString("option_type").isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.updateOptionType(
        xml.readString("option_type"),
        xml.readString("description_type"));
}

bool ProductOptionModule::existProductAndOption(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.delDomain();
    QString product_code = xml.readString("product_code[0]");
    if (product_code.isEmpty()) {
        cerr << "Empty product code "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    xml.setDomain("options[0].option[0]");
    if (xml.readString("option_type").isEmpty()) {
        cerr << "Empty option type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (xml.readString("option_name").isEmpty()) {
        cerr << "Empty option name "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.existProductAndOption(
        product_code, xml.readString("option_type"),
        xml.readString("option_name"));
}

QString ProductOptionModule::getProductOptions(const QString& product_code)
{
    if (product_code.isEmpty()) {
        cerr << "Empty product code "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString {};
    }

    const QVector<ProductOptionData>&
        options
        = database.getProductOptions(product_code);
    if (options.isEmpty()) {
        cerr << "The product does not have options "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString {};
    }

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("options");
    for (const ProductOptionData& option : options) {
        xml.createElementSetDomain("option");

        xml.createElement("option_type", option.option_type);
        xml.createElement("description_type", option.description_type);
        xml.createElement("option_name", option.option_name);
        xml.createElement("description_option", option.description_option);
        xml.createElement("value", QString::number(option.value));

        if (option.is_default)
            xml.createElement("default", "true");
        else
            xml.createElement("default", "false");

        xml.releaseDomain("option", false);
    }
    xml.releaseDomain("options", true);

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString {};
    }
    return xml.toString();
}

bool ProductOptionModule::insertOptionsToProduct(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.delDomain();
    QString product_code = xml.readString("product_code");
    xml.setDomain("options");
    for (auto i = 0; i < xml.howManyTags("option"); i++) {
        xml.setDomain("option[" + QString::number(i) + "]");

        if (xml.readString("option_type").isEmpty()
            || xml.readString("option_name").isEmpty()
            || product_code.isEmpty()) {
            xml.releaseDomain("option", true);
            continue;
        }

        if (xml.readString("value").isEmpty()) {
            xml.releaseDomain("value", true);
            continue;
        }

        if (!database.existOptionType(xml.readString("option_type"))
            && !database.insertOptionType(
                xml.readString("option_type"),
                xml.readString("description_type")))
            return false;

        if (!database.existOption(xml.readString("option_type"), xml.readString("option_name"))
            && !database.insertOption(xml.readString("option_type"), xml.readString("option_name"),
                xml.readString("description_option")))
            return false;

        if (!database.insertOptionToProduct(product_code, xml.readString("option_type"),
                xml.readString("option_name"),
                (xml.readString("value")).toDouble(),
                xml.readString("default"))) {
            return false;
        }
        xml.releaseDomain("option");
    }
    xml.releaseDomain("options", false);
    return true;
}

bool ProductOptionModule::updateOptionsToProduct(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.delDomain();
    QString product_code = xml.readString("product_code");
    if (product_code.isEmpty()) {
        cerr << "Empty product code "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    xml.setDomain("options");
    for (auto i = 0; i < xml.howManyTags("option"); i++) {
        xml.setDomain("option[" + QString::number(i) + "]");

        if (xml.readString("option_type").isEmpty()) {
            cerr << "Empty option type "
                 << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
            xml.releaseDomain("option", true);
            continue;
        }

        if (xml.readString("option_name").isEmpty()) {
            cerr << "Empty option name "
                 << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
            xml.releaseDomain("option", true);
            continue;
        }

        if (xml.readString("value").isEmpty()) {
            cerr << "Empty option value "
                 << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
            xml.releaseDomain("option", true);
            continue;
        }

        if (!database.existOptionType(xml.readString("option_type"))
            && !database.insertOptionType(
                xml.readString("option_type"),
                xml.readString("description_type"))) {
            return false;
        }

        if (!database.existOption(
                xml.readString("option_type"),
                xml.readString("option_name"))
            && !database.insertOption(
                xml.readString("option_type"),
                xml.readString("option_name"),
                xml.readString("description_option"))) {
            return false;
        }

        if (!database.updateOptionToProduct(
                product_code,
                xml.readString("option_type"),
                xml.readString("option_name"),
                xml.readString("value").toDouble(),
                xml.readString("default"))) {
            return false;
        }
    }
    return true;
}

bool ProductOptionModule::deleteOptionsToProduct(const QString& product_code, const QString& option_type, const QString& option_name)
{
    return database.deleteOptionToProduct(product_code, option_type, option_name);
}

QString ProductOptionModule::getStringXmlOption(const ProductOptionData& option)
{
    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("options");
    xml.createElementSetDomain("option");
    xml.createElement("option_type", option.option_type);
    xml.createElement("description_type", option.description_type);
    xml.createElement("option_name", option.option_name);
    xml.createElement("description_option", option.description_option);
    xml.createElement("value", QString::number(option.value));

    if (option.is_default)
        xml.createElement("default", "t");
    else
        xml.createElement("default", "f");

    xml.releaseDomain("option", true);
    xml.releaseDomain("option", true);

    if (!xml.validateXmlWithDTD(OPTIONS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString {};
    }

    return xml.toString();
}

void ProductOptionModule::setAllProductOptionValue(const QString& option_type, const QString& option_name, const QString& value)
{
    bool ok { false };
    value.toDouble(&ok);
    if (!ok)
        return;

    database.setAllProductOptionValue(option_type, option_name, value);
}

void ProductOptionModule::updateAllProductOptionValue(const QString& option_type, const QString& option_name, const QString& value)
{
    bool ok { false };
    value.toDouble(&ok);
    if (!ok)
        return;
    database.updateAllProductOptionValue(option_type, option_name, value);
}
