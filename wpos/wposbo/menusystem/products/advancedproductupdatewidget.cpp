// file      :  advancedproductupdatewidget.cpp
// birth     :  9/27/2023
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "advancedproductupdatewidget.h"
#include "advancedproductinsertionwidget.h"

AdvancedProductUpdateWidget::AdvancedProductUpdateWidget(
    ProductModule* product_model,
    QWidget* parent, const QString& name)
    : AdvancedProductEditionWidget(product_model, parent, name)
{
    QVBoxLayout* vlayout {};
    if (!(vlayout = qobject_cast<QVBoxLayout*>(update_frame->layout())))
        vlayout = new QVBoxLayout(update_frame);

    product_insertion_form = new AdvancedProductInsertionWidget(product_model, ProductInsertionWidget::Unitary, update_frame, "Update Widget");
    vlayout->addWidget(product_insertion_form);

    connect(product_insertion_form, &AdvancedProductInsertionWidget::cancelSignal, this, &AdvancedProductUpdateWidget::nUpdateCancelSlot);
    connect(product_insertion_form, &AdvancedProductInsertionWidget::acceptSignal, this, &AdvancedProductUpdateWidget::nUpdateAcceptSlot);
}

void AdvancedProductUpdateWidget::acceptSlot()
{
    auto items = product_listview->selectedItems();
    if (items.isEmpty())
        return;
    auto item = items.first();
    stack->setCurrentWidget(update_page);
    product_insertion_form->setProduct(item->text(Code));
}

void AdvancedProductUpdateWidget::nUpdateAcceptSlot()
{
    startShowing();
}

void AdvancedProductUpdateWidget::nUpdateCancelSlot()
{
    product_insertion_form->clear();
    stack->setCurrentWidget(product_page);
}
