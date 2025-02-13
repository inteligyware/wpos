// file      :  optionupdatewidget.h
// birth     :  4/28/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef OPTION_UPDATE_WIDGET_H
#define OPTION_UPDATE_WIDGET_H

#include "optioneditionwidget.h"
#include <QObject>

class OptionUpdateWidget : public OptionEditionWidget {

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.OptionUpdateModel")

public:
    static const QString DBusObjectPath;

    explicit OptionUpdateWidget(QWidget* parent = 0, const QString& name = QString {});
    ~OptionUpdateWidget() = default;

signals:
    Q_SCRIPTABLE void optionChanged(); // dbus_signals

protected slots:
    virtual void acceptSlot() override;
    virtual void notify() override;

    void optionClickedSlot(); // processOption()
    void applyToAllProductsSlot(); // processType()

protected:
    virtual void showEvent(QShowEvent* e) override;
    virtual void getTypes() override;
    virtual void getOptionTypes(const QString& type) override;
};

#endif // OPTION_UPDATE_WIDGET_H
