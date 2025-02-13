// file      :  advancedproductupdatewidget.h
// birth     :  9/27/2023
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef NUPDATESELECTIONWIDGET_H
#define NUPDATESELECTIONWIDGET_H

#include "advancedproducteditionwidget.h"

class ProductModule;
class AdvancedProductInsertionWidget;

class AdvancedProductUpdateWidget : public AdvancedProductEditionWidget {
    Q_OBJECT

public:
    explicit AdvancedProductUpdateWidget(ProductModule* product_model, QWidget* parent = 0, const QString& name = QString {});
    ~AdvancedProductUpdateWidget() = default;

public slots:
    virtual void acceptSlot() override;

protected slots:
    void nUpdateAcceptSlot();
    void nUpdateCancelSlot();

private:
    AdvancedProductInsertionWidget* product_insertion_form {};
};

#endif
