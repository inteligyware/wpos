// file      :  optioncreationwidget.h
// birth     :  4/27/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef OPTION_CREATION_WIDGET_H
#define OPTION_CREATION_WIDGET_H

#include "optioneditionwidget.h"
#include <QObject>

class OptionCreationWidget : public OptionEditionWidget {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.OptionCreationModel")

public:
    static const QString DBusObjectPath;

    explicit OptionCreationWidget(QWidget* parent = 0, const QString& name = QString {});
    ~OptionCreationWidget() = default;

signals:
    Q_SCRIPTABLE void optionChanged(); // dbus_signals

protected slots:
    virtual void notify() override;
    void processType(); // processType()
    void processOption(); // processOption()
};

#endif // OPTION_CREATION_WIDGET_H
