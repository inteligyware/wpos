// file      :  devicesconfigwidget.h
// birth     :  5/28/2005
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef DEVICESCONFIG_H
#define DEVICESCONFIG_H

#include "ui_devicesconfigwidgetbase.h"
#include <QWidget>

/**
 *@author Carlos Manzanedo
 */

class DevicesConfig : public QWidget,
                      private Ui::DeviceConfigWidgetBase {
    Q_OBJECT
public:
    enum DrawerType {
        Serial = 0,
        Drawer = 1,
        Samsung = 2
    };

    enum PrinterType {
        Direct = 0,
        Ipp = 1
    };

    enum CashboxDevice {
        S0 = 0,
        S1 = 1,
        S2 = 2,
        S3 = 3,
        P0 = 4,
        P1 = 5,
        USB0 = 6,
        USB1 = 7,
        USB2 = 8,
        USB3 = 9
    };

    explicit DevicesConfig(QWidget* parent = 0, const QString& name = QString {});
    ~DevicesConfig() = default;

public slots:
    void acceptSlot();

    void printerTypeChanged(int index);
    void kitchenTypeChanged(int index);

    void printerDeviceChanged(int index);
    void kitchenDeviceChanged(int index);

    void printerIppDeviceChanged(const QString& text);
    void kitchenIppDeviceChanged(const QString& text);
    void somethingChanged(bool enable);
    void somethingChanged(int index);

protected:
    virtual void showEvent(QShowEvent* e) override;

    void readDevicesConfig();
    void readCashboxConfig();
    void readPrinterConfig();

    void writeDevicesConfig();
    void writePrinterConfig();
    void writeCashboxConfig();

    void prepareMainTickets(const QString& text);
    void prepareKitchenTickets(const QString& text);

private:
    QMap<QString, DrawerType> drawer_type_map;
    QMap<QString, PrinterType> printer_type_map;
    QMap<QString, CashboxDevice> device_map;

    QString preferred_printer_ipp_device;
    QString preferred_kitchen_ipp_device;
    int preferred_printer_device;
    int preferred_kitchen_device;
};

#endif
