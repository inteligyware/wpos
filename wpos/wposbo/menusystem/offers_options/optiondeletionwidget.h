// file      :  optiondeletionwidget.h
// birth     :  4/28/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef OPTION_DELETION_WIDGET_H
#define OPTION_DELETION_WIDGET_H

#include "optioneditionwidget.h"
#include <QObject>

class OptionDeletionWidget : public OptionEditionWidget {

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.OptionDeletionModel")

public:
    static const QString DBusObjectPath;

    explicit OptionDeletionWidget(QWidget* parent = 0, const QString& name = QString {});
    ~OptionDeletionWidget() = default;

signals:
    Q_SCRIPTABLE void optionChanged(); // dbus_signals

protected slots:
    virtual void acceptSlot() override;
    virtual void notify() override;

    void optionClickedSlot();
    void delTypeSlot(); // processType()
    bool delOption(const QString& type, const QString& option); // processOption()

protected:
    virtual void showEvent(QShowEvent* e) override;
    virtual void getTypes() override;
    virtual void getOptionTypes(const QString& type) override;
};

#endif // OPTION_DELETION_WIDGET_H
