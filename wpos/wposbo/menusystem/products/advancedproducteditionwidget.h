// file      :  advancedproducteditionwidget.h
// birth     :  5/3/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef NDELPRODUCTWIDGET_H
#define NDELPRODUCTWIDGET_H

#include "ui_advancedproducteditionwidgetbase.h"

class ProductModule;
class QTreeWidgetItem;

class AdvancedProductEditionWidget : public QWidget,
                                     protected Ui::AdvancedProductEditionWidgetBase {
    Q_OBJECT

public:
    enum ProductColumn {
        Icon = 0,
        Code = 1,
        Name = 2
    };

    explicit AdvancedProductEditionWidget(
        ProductModule* product_model,
        QWidget* parent = 0,
        const QString& name = QString {});
    ~AdvancedProductEditionWidget() = default;

public slots:
    void clear();
    void upButtonSlot();
    void downButtonSlot();

    virtual void acceptSlot() = 0;
    void cancelSlot();

    void productSelectedSlot();
    void nameChangedSlot(const QString& text);
    void searchButtonSlot();

protected slots:
    void startShowing();
    void setProgressSlot(int num, const QString& product);
    void enableOkButton();

signals:
    void progressSignal(int num, const QString& product);

protected:
    virtual void showEvent(QShowEvent* e) override;
    void fetchProducts();

    QTreeWidgetItem* selectedItem() const;

    inline ProductModule& productModel() const
    {
        return *product_model;
    }

private:
    void setupWidgets();
    void setWidgetsIcons();
    void connectWidgets();
    bool isItemSelected();

private:
    ProductModule* product_model;
};

#endif
