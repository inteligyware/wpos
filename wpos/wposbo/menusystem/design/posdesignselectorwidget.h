// file      :  posdesignselectorwidget.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef POSDESIGNSELECTORWIDGET_H
#define POSDESIGNSELECTORWIDGET_H

#include "ui_posdesignselectorinterface.h"

class ProductScreenDesignWidget;
class InvitationScreenDesignWidget;

class POSDesignSelectorWidget : public QWidget,
                                private Ui::POSDesignSelectorInterface {
    Q_OBJECT

public:
    explicit POSDesignSelectorWidget(QWidget* parent = 0, const QString& name = QString {});
    ~POSDesignSelectorWidget() = default;

private slots:
    void actionSelectedChanged();
    void cancel();
    virtual void showEvent(QShowEvent* e) override;

private:
    ProductScreenDesignWidget* screen;
    InvitationScreenDesignWidget* offers;
    QMap<QListWidgetItem*, QString> submenu_item_relantionships;
};

#endif
