// file      :  deleteofferwidget.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef DELETEOFFERWIDGET_H
#define DELETEOFFERWIDGET_H

#include "productsmodule/offersmodule/productoffermodule.h"
#include "ui_deleteofferproductinterface.h"

class ProductOfferModule;

class DeleteOfferWidget : public QWidget,
                          private Ui::DeleteOfferProductInterface {
    Q_OBJECT

public:
    explicit DeleteOfferWidget(QWidget* parent = 0, const QString& name = QString {});
    ~DeleteOfferWidget() = default;

    void setOffer(const QString& type, const QString& name);
    void showSelectOffer();
    void showOfferType(const QString& offer_type);

signals:
    void offerDeleted(bool deleted);
    void typeDeleted(bool deleted);

private slots:
    void deleteSlot();
    void cancelSlot();
    void acceptSlot();

private:
    void deleteOffer();
    void deleteType();

    ProductOfferModule product_offer_module {};
    QString offer_type, offer_name;
};

#endif
