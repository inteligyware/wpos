// file      :  advancedproductdeletionwidget.h
// birth     :  9/27/2023
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef ADVANCEDPRODUCTDELETIONWIDGET_H
#define ADVANCEDPRODUCTDELETIONWIDGET_H

#include "advancedproducteditionwidget.h"

class ProductModule;

class AdvancedProductDeletionWidget : public AdvancedProductEditionWidget {
    Q_OBJECT

public:
    explicit AdvancedProductDeletionWidget(ProductModule* product_model, QWidget* parent = 0, const QString& name = QString {});
    ~AdvancedProductDeletionWidget() = default;

public slots:
    virtual void acceptSlot() override;
    void cancelDeletedSlot();
    void deleteSlot();

protected:
    void deleteAllProductsContaining(const QString& product_code);
    bool checkUnitaryUse(const QString& product_code);
};

#endif // ADVANCEDPRODUCTDELETIONWIDGET_H
