// file      :  advancedproductinsertionwidget.h
// birth     :  5/3/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef NUPDATEPRODUCTWIDGET_H
#define NUPDATEPRODUCTWIDGET_H

#include "productinsertionwidget.h"

/**
 *@author Carlos Manzanedo
 */

class AdvancedProductInsertionWidget : public ProductInsertionWidget {
    Q_OBJECT
public:
    explicit AdvancedProductInsertionWidget(ProductModule* product_module, int mode = 0, QWidget* parent = 0, const QString& name = QString {});
    ~AdvancedProductInsertionWidget() = default;

public slots:
    void clear();
    void setMode(int num);
    void setProductWithName(const QString& product);
    void setProduct(const QString& product_code);
    void numkeyChangedSlot(double);

    void acceptSlot();

protected:
    void showEvent(QShowEvent* e);
    void setComposition(const QString& product_code);
    void setOptions(const QString& product_code);
    void setOffers(const QString& product_code);
    void setLogo(const QString& logo);

    bool saveProduct();
    bool saveProductComposition();

    QString p_code;
};

#endif
