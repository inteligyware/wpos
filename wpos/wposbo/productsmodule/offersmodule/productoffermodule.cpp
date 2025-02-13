// file      :  productoffermodule.cpp
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "productoffermodule.h"
#include "productoffermodule_adaptor.h"

#include "database/productsmoduledb.h"

#include <libbslxml/xmlconfig.h>
#include <wposcore/basicdatabase.h>

#include <iostream>
using namespace std;

static const QString& OFFERS_DTD = "dtddocs:products_offerslist.dtd";
const QString ProductOfferModule::DBusObjectPath = QString { "/wpos/wposbo/DBusBOProductOffer" };

ProductOfferModule::ProductOfferModule(QObject* parent, const QString& name)
    : QObject(parent)
{
    setObjectName(name);
    database.connect();
    new DBusBOProductOfferAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(ProductOfferModule::DBusObjectPath, this);
}

bool ProductOfferModule::deleteOffer(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "xml does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.delDomain();
    xml.setDomain("offers[0].offer[0]");
    if (xml.readString("offer_type").isEmpty()) {
        cerr << "Empty offer type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (xml.readString("offer_name").isEmpty()) {
        cerr << "Empty offer name "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.deleteOffer(
        xml.readString("offer_type"),
        xml.readString("offer_name"));
}

bool ProductOfferModule::deleteOfferType(const QString& offer_type)
{
    if (offer_type.isEmpty()) {
        cerr << "Empty offer type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }
    return database.deleteOfferType(offer_type);
}

bool ProductOfferModule::existOffer(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    xml.delDomain();
    xml.setDomain("offers[0]");
    xml.setDomain("offer[0]");
    return database.existOffer(
        xml.readString("offer_type"),
        xml.readString("offer_name"));
}

bool ProductOfferModule::existOfferType(const QString& offer_type)
{
    return database.existOfferType(offer_type);
}

QString ProductOfferModule::getDescriptionOffer(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString{};
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString{};
    }

    xml.delDomain();
    xml.setDomain("offers[0].offer[0]");
    return database.getDescriptionOffer(
        xml.readString("offer_type"),
        xml.readString("offer_name"));
}

QString ProductOfferModule::getDescriptionOfferType(const QString& offer_type)
{
    if (offer_type.isEmpty())
        return QString{};
    return database.getDescriptionOfferType(offer_type);
}

QString ProductOfferModule::getCppOperator(const QString& offer_type)
{
    return database.getCppOperator(offer_type);
}

QString ProductOfferModule::getLogoOffer(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString{};
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString{};
    }

    xml.delDomain();
    xml.setDomain("offers[0].offer[0]");
    if (xml.readString("offer_type").isEmpty()) {
        cerr << "Empty offer type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';

        return QString{};
    }

    if (xml.readString("offer_name").isEmpty()) {
        cerr << "Empty offer name "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString{};
    }

    return database.getLogoOffer(
        xml.readString("offer_type"),
        xml.readString("offer_name"));
}

QString ProductOfferModule::getLogo(const QString& type, const QString& name)
{
    return database.getOffer(type, name).logo;
}

QString ProductOfferModule::getOffer(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString{};
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString{};
    }

    xml.setDomain("offers[0].offer[0]");
    QString type = xml.readString("offer_type");
    QString name = xml.readString("offer_name");
    if (type.isEmpty() || name.isEmpty()) {
        cerr << "Empty offer type or offer name "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString{};
    }

    ProductOfferData offer = database.getOffer(type, name);
    if ( offer.isNull() ) {
        cerr << "The offer does not exists " << __PRETTY_FUNCTION__
             << " : " << __LINE__ << '\n';
        cerr << "Offer type: " << type.toStdString() << '\n';
        cerr << "Offer name: " << type.toStdString() << '\n';
        return QString{};
    }

    xml.clear();
    xml.delDomain();

    xml.createElementSetDomain("offers");
    xml.createElementSetDomain("offer");
    xml.createElement("offer_type", offer.offer_type);
    xml.createElement("offer_name", offer.offer_name);
    xml.createElement("description_offer", offer.description_offer);
    xml.createElement("cpp_operator", offer.cpp_operator);
    xml.createElement("logo", offer.logo);

    xml.releaseDomain("offer", true);
    xml.releaseDomain("offers", true);

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString{};
    }

    return xml.toString();
}

QString ProductOfferModule::getAllOffers()
{
    QVector<ProductOfferData>
    offers {database.getAllOffers()};

    if (offers.isEmpty()) {
        cerr << "There are not offers for this offer type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString{};
    }

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("offers");
    for (const ProductOfferData& offer : offers) {
        xml.createElementSetDomain("offer");
        xml.createElement("offer_type", offer.offer_type);
        xml.createElement("offer_name", offer.offer_name);
        xml.createElement("logo", offer.logo);
        xml.releaseDomain("offer", false);
    }
    xml.releaseDomain("offers", true);

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString{};
    }
    return xml.toString();
}

QString ProductOfferModule::getOffers(const QString& offer_type)
{
    if (offer_type.isEmpty()) {
        cerr << "Empty offer type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString{};
    }

    QVector<ProductOfferData>
        offers  {database.getOffers(offer_type)};

    if (offers.isEmpty())  return QString();

    XmlConfig xml;
    xml.delDomain();

    xml.createElementSetDomain("offers");
    for (const ProductOfferData& offer : offers) {
        xml.createElementSetDomain("offer");
        xml.createElement("offer_type", offer.offer_type);
        xml.createElement("description_type", offer.description_type);
        xml.createElement("offer_name", offer.offer_name);
        xml.createElement("description_offer", offer.description_offer);
        xml.createElement("cpp_operator", offer.cpp_operator);
        xml.createElement("logo", offer.logo);

        xml.releaseDomain("offer", false);
    }
    xml.releaseDomain("offers", true);

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString{};
    }

    return xml.toString();
}

QString ProductOfferModule::getOfferType(const QString& offer_type)
{
    if (offer_type.isEmpty()) {
        cerr << "Empty offer type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString{};
    }

    ProductOfferData offer {database.getOfferType(offer_type)};

    if ( offer.isNull() ) {
        cerr << "offer type does not exists"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        cerr << "Offer type: " << offer_type.toStdString() << '\n';
        return QString{};
    }

    XmlConfig xml;
    xml.delDomain();

    xml.createElementSetDomain("offers");
    xml.createElementSetDomain("offer");
    xml.createElement("offer_type", offer.offer_type);
    xml.createElement("description_type", offer.description_type);
    xml.createElement("offer_name");
    xml.createElement("cpp_operator", offer.cpp_operator);
    xml.createElement("logo", offer.logo);
    xml.releaseDomain("offer");
    xml.releaseDomain("offers", true);

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString{};
    }
    return xml.toString();
}

QString ProductOfferModule::getOfferTypes()
{
    QStringList offer_types = database.getOfferTypes();
    if ( offer_types.isEmpty()) {
        cerr << "offer type does not exists"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString{};
    }

    XmlConfig xml;
    xml.delDomain();

    xml.createElementSetDomain("offers");
    for (const QString& offer_type : offer_types) {
        xml.createElementSetDomain("offer");
        xml.createElement("offer_type", offer_type);
        xml.createElement("offer_name");
        xml.releaseDomain("offer", true);
    }
    xml.releaseDomain("offers", true);

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString{};
    }

    return xml.toString();
}

bool ProductOfferModule::insertOffer(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    QString  offer_type = xml.readString("offer_type");
    QString  offer_name = xml.readString("offer_name");
    xml.setDomain("offers[0].offer[0]");
    if ( offer_type.isEmpty() || offer_name.isEmpty()) {
        cerr << "Empty offer type or offer name"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!database.existOfferType(xml.readString("offer_type"))) {
        database.insertOfferType(
            offer_type, xml.readString("description_type"), xml.readString("cpp_operator"));
    }

    return database.insertOffer(
        offer_type,
        offer_name,
        xml.readString("description_offer"),
        xml.readString("logo"));
}

bool ProductOfferModule::insertOfferType(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.setDomain("offers[0].offer[0]");
    if (xml.readString("offer_type").isEmpty()) {
        cerr << "Empty offer type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.insertOfferType(
        xml.readString("offer_type"),
        xml.readString("description_type"),
        xml.readString("cpp_operator"));
}

int ProductOfferModule::getNumOfferTypes()
{
    return database.getNumOfferTypes();
}

bool ProductOfferModule::updateOffer(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.delDomain();
    xml.setDomain("offers[0].offer[0]");
    if (xml.readString("offer_type").isEmpty()) {
        cerr << "Empty offer type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (xml.readString("offer_name").isEmpty()) {
        cerr << "Empty offer name"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.updateOffer(xml.readString("offer_type"), xml.readString("offer_name"),
            xml.readString("description_offer"), xml.readString("logo"));
}

bool ProductOfferModule::updateOfferType(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    xml.setDomain("offers[0].offer[0]");
    QString  offer_type = xml.readString("offer_type");
    if (offer_type.isEmpty()) {
        cerr << "Empty offer type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.updateOfferType(
            offer_type,
            xml.readString("description_type"),
            xml.readString("cpp_operator"));
}

QString ProductOfferModule::getProductOffers(const QString& product_code)
{
    if (product_code.isEmpty()) {
        cerr << "Empty product code"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return QString{};
    }

    XmlConfig xml;
    xml.delDomain();
    xml.createElementSetDomain("offers");

    QVector<ProductOfferData> offers = database.getProductOffers(product_code);
    for (const ProductOfferData& offer : offers) {
        xml.createElementSetDomain("offer");

        xml.createElement("offer_type", offer.offer_type);
        xml.createElement("description_type", offer.description_type);
        xml.createElement("offer_name", offer.offer_name);
        xml.createElement("description_offer", offer.description_offer);
        xml.createElement("cpp_operator", offer.cpp_operator);
        xml.createElement("value", QString::number(offer.value));

        if (offer.is_default) xml.createElement("default", "t");
        else  xml.createElement("default", "f");
        xml.createElement("logo", offer.logo);

        xml.releaseDomain("offer", false);
    }
    xml.releaseDomain("offers", true);

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return QString {};
    }

    return  xml.toString();
}

bool ProductOfferModule::existProductAndOffer(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    QString product_code = xml.readString("product_code[0]");
    if (product_code.isEmpty()) {
        cerr << "Empty product code"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    QString offer_type = xml.readString("offer_type");
    xml.setDomain("offers[0].offer[0]");
    if (offer_type.isEmpty()) {
        cerr << "Empty offer type "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    QString offer_name = xml.readString("offer_name");
    if (offer_name.isEmpty()) {
        cerr << "Empty offer name" << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    return database.existProductAndOffer(product_code, offer_type, offer_name);
}

bool ProductOfferModule::insertOffersToProduct(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }
    xml.delDomain();

    QString product_code = xml.readString("product_code");
    if (product_code.isEmpty()) {
        cerr << "Empty product code"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    bool inserted = true;
    xml.setDomain("offers");
    for (auto i = 0; i < xml.howManyTags("offer"); i++) {
        xml.setDomain("offer[" + QString::number(i) + "]");

        auto offer_type = xml.readString("offer_type");
        auto offer_name = xml.readString("offer_name");

        if (!database.existOfferType(offer_type))
            if (!database.insertOfferType(
                    offer_type,
                    xml.readString("description_type"),
                    xml.readString("cpp_operator")))
                return false;

        if (!database.existOffer(offer_type, offer_name))
            if (!database.insertOffer(
                    offer_type,
                    offer_name,
                    xml.readString("description_offer")))
                return false;

        if (!database.insertOfferToProduct(
                product_code,
                offer_type,
                offer_name,
                xml.readString("value").toDouble(),
                "f"))
        {
            inserted = false;
            break;
        }
        xml.releaseDomain("offer", false);
    }

    return inserted;
}

bool ProductOfferModule::updateOffersToProduct(const QString& xml_string)
{
    XmlConfig xml;
    if (!xml.readXmlFromString(xml_string)) {
        cerr << "Cannot convert string into xml"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();
        return false;
    }

    QString product_code = xml.readString("product_code[0]");
    if (product_code.isEmpty()) {
        cerr << "Empty product code"
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        return false;
    }

    bool updated = true;
    xml.setDomain("offers");
    for (auto i = 0; i < xml.howManyTags("offer"); i++) {
        xml.setDomain("offer[" + QString::number(i) + "]");

        QString offer_type = xml.readString("offer_type");
        if (offer_type.isEmpty()) {
            cerr << "Empty offer type "
                 << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
            continue;
        }

        QString offer_name = xml.readString("offer_name");
        if (offer_name.isEmpty()) {
            cerr << "Empty offer name "
                 << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
            continue;
        }

        if (!database.updateOfferToProduct(
                product_code,
                offer_type,
                offer_name,
                xml.readString("value").toDouble(), "f"))
        {
            updated = false;
            break;
        }
        xml.releaseDomain("offer", true);
    }
    return updated;
}

bool ProductOfferModule::deleteOffersToProduct(const QString& product_code, const QString& offer_type, const QString& offer_name)
{
    return database.deleteOfferToProduct(product_code, offer_type, offer_name);
}

QString ProductOfferModule::getStringXmlOffer(const ProductOfferData& offer)
{
    XmlConfig xml;
    xml.delDomain();

    xml.createElementSetDomain("offers");
    xml.createElementSetDomain("offer");
    xml.createElement("offer_type", offer.offer_type);
    xml.createElement("description_type", offer.description_type);
    xml.createElement("offer_name", offer.offer_name);
    xml.createElement("description_offer", offer.description_offer);
    xml.createElement("cpp_operator", offer.cpp_operator);
    xml.createElement("value", QString::number(offer.value));

    if (offer.is_default)
        xml.createElement("default", "t");
    else
        xml.createElement("default", "f");
    xml.createElement("logo", offer.logo);

    xml.releaseDomain("offer", true);
    xml.releaseDomain("offers", true);

    if (!xml.validateXmlWithDTD(OFFERS_DTD, true)) {
        cerr << "XML does not validate against DTD "
             << __PRETTY_FUNCTION__ << " : " << __LINE__ << '\n';
        xml.debug();

        return QString{};
    }

    return  xml.toString();
}

void ProductOfferModule::setAllProductOfferValue(const QString& offer_type, const QString& offer_name, const QString& value)
{
    bool ok{false};
    double v = value.toDouble(&ok);
    if (!ok)  return;

    database.setAllProductOfferValue(offer_type, offer_name, value);
}

void ProductOfferModule::updateAllProductOfferValue(const QString& offer_type, const QString& offer_name, const QString& value)
{
    bool ok{false};
    double v = value.toDouble(&ok);
    if (!ok)  return;

    database.updateAllProductOfferValue(offer_type, offer_name, value);
}
