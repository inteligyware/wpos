// file      :  optioncreationwidget.cpp
// birth     :  4/27/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "optioncreationwidget.h"
#include "optioncreationwidget_adaptor.h"

#include "productsmodule/optionsmodule/productoptionmodule.h"
#include <libbslxml/xmlconfig.h>
#include <wposgui/keyboard/floatkeyboard.h>

#include <QMessageBox>
#include <QtDBus/QDBusConnection>

const QString OptionCreationWidget::DBusObjectPath = QString { "/wpos/wposbo/OptionCreationModel" };

OptionCreationWidget::OptionCreationWidget(QWidget* parent, const QString& name)
    : OptionEditionWidget(parent, name)
{
    new OptionCreationModelAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(OptionCreationWidget::DBusObjectPath, this);

    title_label->setText(tr("Create new Options"));
    del_type_button->hide();
    group_box->hide();

    // unique
    connect(add_type_button, &QPushButton::clicked, this, &OptionCreationWidget::processType);
    connect(add_option_button, &QPushButton::clicked, this, &OptionCreationWidget::processOption);
}

void OptionCreationWidget::processOption()
{

    auto items = type_listview->selectedItems();
    auto option = option_lineedit->text();

    if (option.isEmpty() || items.isEmpty())
        return;
    auto item = items.first();

    XmlConfig xml;
    xml.createElementSetDomain("options.option");
    xml.createElement("option_type", item->text(0));
    xml.createElement("option_name", option);
    xml.createElement("description_option", option + "_option");
    auto xml_string = xml.toString();

    auto title = tr("fallo al insertar la opcion");
    if (mod.existOption(xml_string)) {
        auto msg = tr("No se ha podido insertar la opcion\n ya existe una opcion con ese tipo y nombre\n");
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
        return;
    }

    if (!mod.insertOption(xml_string)) {
        auto msg = tr("No se ha podido insertar la opcion\nquiza exista una opcion con ese nombre\n");
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
        return;
    }

    getOptionTypes(item->text(0));
    option_lineedit->clear();
}

void OptionCreationWidget::processType()
{
    auto type = type_lineedit->text();
    if (type.isEmpty())
        return;

    auto title = tr("fallo al insertar el tipo de opcion");
    auto msg = tr("No se ha podido insertar el tipo de opcion\n ya existe una tipo de opcion con ese nombre\n");
    if (mod.existOptionType(type)) {
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
        return;
    }

    XmlConfig xml;
    xml.createElementSetDomain("options.option");
    xml.createElement("option_type", type);
    xml.createElement("description_type", type + "_option");
    xml.createElement("option_name");

    if (!mod.insertOptionType(xml.toString())) {
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
        return;
    }

    clear();
    getTypes();
}

void OptionCreationWidget::notify()
{
    //    QByteArray params;
    //    QDataStream stream(params, IO_WriteOnly);
    ////    kapp->dcopClient()->emitDCOPSignal("optionChanged()", params);
    emit optionChanged();
}
