// file      :  productoffercreationwidget.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef BSLADDOFFERWIDGET_H
#define BSLADDOFFERWIDGET_H

#include "ui_productoffercreationwidgetbase.h"

#include "productsmodule/offersmodule/productoffermodule.h"

#include <QList>
#include <QWidget>

class ProductOfferData;
class FloatKeyboard;

class ProductOfferCreationWidget : public QWidget,
                                   private Ui::ProductOfferCreationWidgetBase {

    Q_OBJECT

public:
    explicit ProductOfferCreationWidget(double _product_price, QWidget* parent = 0, const QString& name = QString {});
    ~ProductOfferCreationWidget() = default;

    void setProductPrice(double _product_price);
    void initOfferTypes();
    void setEditable(bool editable);
    bool saveOffersProduct(const QString& product_code);
    bool saveOffer(ProductOfferData* offer);
    void saveOffers();
    void setOffer(const QString& offer_type, const QString& offer_name);
    void checkDefaultOffers();
    QList<ProductOfferData*>* getOffers();

    void showError(const QString& offer_type, const QString& offer_name);
    void showTooOffers();
    bool canInsertOfferType(const QString& offer_type);
    void clearWidget();
    void clear();

signals:
    void offerSaved(ProductOfferData* offer);

public slots:
    void offerTypeActivated(const QString& text);
    void saveOfferButtonClicked();
    void tooOfferButtonClicked();
    void acceptErrorButtonClicked();

protected:
    virtual void showEvent(QShowEvent* e) override;
    void updateOffers();
    void clearDefaults();
    void initLogos();

private:
    ProductOfferModule model;
    FloatKeyboard* float_keyboard_offer {};
    QList<ProductOfferData*> offers;
    double product_price { 0 };
};

#endif
