// file      :  databasemodule.cpp
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "databasemodule.h"
#include "databasemodule_adaptor.h"

#include <libbslxml/xmlconfig.h>
#include <wposcore/config.h>

#include <QFile>
#include <QFileSystemWatcher>
#include <QProcess>
#include <QString>
#include <QtDBus/QDBusConnection>

#include <iostream>
using namespace std;

static const QString& DATABASE_CONFIG_DTD { "dtddocs:dbmodule_config.dtd" };
static const QString& DDBB_CONFIGURATION_DTD_NMAP { "dtddocs:dbmodule_config_nmap.dtd" };
static const QString& DATABASE_CONNECTION_FILE { cfg::xmlFileByKey(cfg::XMLKey::Database) };

const QString DatabaseModule::DBusObject { "/wpos/wposbo/DBusBOModel" };

DatabaseModule::DatabaseModule(
    QObject* parent,
    const QString& name)
    : QObject(parent)
{
    setObjectName(name);
    new DBusBOModelAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(DatabaseModule::DBusObject, this);

    // it should be used by other modules so please be sure that if not implemented here it
    // should be created before all the modules are created.
    file_watcher = new QFileSystemWatcher(this);
    file_watcher->addPath(DATABASE_CONNECTION_FILE);
    connect(file_watcher, &QFileSystemWatcher::fileChanged,
        this, &DatabaseModule::handleFileChanged);
}

DatabaseModule::~DatabaseModule() { }

// the documentation for each method is described at the moduleinterface
bool DatabaseModule::setConfig(QString xml_str)
{

    // translate to a XmlConfig
    XmlConfig xml;
    xml.readXmlFromString(xml_str);
    if (!xml.wellFormed() || !xml.validateXmlWithDTD(DDBB_CONFIGURATION_DTD_NMAP))
        return false;

    // all's ok... first we are going to remove the config file and then we are going to save the xml at it
    QFile file(DATABASE_CONNECTION_FILE);
    if (file.exists()) {
        if (!file.remove()) {
            cerr << "problems while trying to delete the config file " << DATABASE_CONNECTION_FILE.toStdString() << endl;
            cerr << "at : " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
            return false;
        }
    }

    if (!xml.save(DATABASE_CONNECTION_FILE)) {
        cerr << "problems while trying to save the config file " << DATABASE_CONNECTION_FILE.toStdString() << endl;
        cerr << "at : " << __PRETTY_FUNCTION__ << ":" << __LINE__ << endl;
        return false;
    }

    return true;
}

QString DatabaseModule::getConfig()
{
    return loadDatabaseConfig().toString();
}

void DatabaseModule::stopDatabase()
{
    QString host;
    QString command;
    QProcess process;
    XmlConfig xml = loadDatabaseConfig();

    // module to be done... it should stop the database at the defined location.
    xml.delDomain();
    host = xml.readString("database.hostname");

    command += "ssh " + host + " /etc/init.d/postgres stop";
    process.setArguments(QStringList() << command);
    process.start();
}

void DatabaseModule::startDatabase()
{
    QString host;
    QString command;
    QProcess process;
    XmlConfig xml = loadDatabaseConfig();

    // module to be done... it should stop the database at the defined location.
    xml.delDomain();
    host = xml.readString("database.hostname");

    command += "ssh " + host + " /etc/init.d/postgres start";
    process.setArguments(QStringList() << command);
    process.start();
}

bool DatabaseModule::databaseState()
{
    // check the state of the database...

    return true;
}

bool DatabaseModule::compact()
{
    // make a vacum to the database.

    return true;
}

bool DatabaseModule::saveDump()
{
    // makes a default dump to the database
    return true;
}

bool DatabaseModule::saveDump(QString file_path)
{
    return true;
}

bool DatabaseModule::restoreDump()
{
    return true;
}

bool DatabaseModule::restoreDump(QString file_path)
{
    return true;
}

XmlConfig DatabaseModule::loadDatabaseConfig()
{
    if (!QFile::exists(DATABASE_CONNECTION_FILE))
        return XmlConfig{};

    XmlConfig xml(DATABASE_CONNECTION_FILE);
    if (!xml.wellFormed()
        || !xml.validateXmlWithDTD(DATABASE_CONFIG_DTD)) {
        return XmlConfig{};
    }
    return xml;
}

void DatabaseModule::handleFileChanged(const QString& file_name)
{
    if (file_name == DATABASE_CONNECTION_FILE) {
        emit configChanged();
    }
    // this is not really needed...
    else if (file_name == DATABASE_CONFIG_DTD) {
    }
}
