// file      :  offereditionwidget.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef OFFER_EDITION_WIDGET_H
#define OFFER_EDITION_WIDGET_H

#include "productsmodule/offersmodule/productoffermodule.h"
#include "ui_offereditionwidgetbase.h"

class NumKeyboard;
class FloatKeyboard;
class QListWidgetItem;

class OfferEditionWidget : public QWidget,
                           protected Ui::OfferEditionWidgetBase {

    Q_OBJECT

public:
    explicit OfferEditionWidget(QWidget* parent = 0, const QString& name = QString {});
    ~OfferEditionWidget();

public slots:
    virtual void clear();
    virtual void typeChangedSlot(const QString& type) = 0;
    virtual void offerChangedSlot(const QString& type) = 0;
    virtual void acceptSlot() = 0;

    void radioButtonSlot();
    void logoItemClickedSlot(QListWidgetItem* icon);

    void rightButtonSlot();
    void leftButtonSlot();

protected:
    void showEvent(QShowEvent* e);
    virtual bool hasAllValues() { return false; }
    virtual bool save() { return false; }

    void loadIcons();
    void getOfferTypes();

protected:
    FloatKeyboard* float_kb;
    NumKeyboard* num_kb;
    ProductOfferModule mod;
};

#endif // OFFER_EDITION_WIDGET_H
