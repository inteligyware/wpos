// file      :  productxmfileupdatewidget.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef UPDATEXML_H
#define UPDATEXML_H

#include "ui_productxmfileupdatewidgetbase.h"

#include <QWidget>

class ProductModule;
class ProductXmFileUpdateWidget : public QWidget,
                                  private Ui::ProductXmFileUpdateWidgetBase {
    Q_OBJECT

public:
    explicit ProductXmFileUpdateWidget(ProductModule* _product_mod, QWidget* parent = 0, const QString& name = QString {});
    ~ProductXmFileUpdateWidget() = default;

public slots:
    void acceptSlot();
    void updatingProduct(int num, const QString& product);

protected:
    void showEvent(QShowEvent* e);

private:
    ProductModule* product_mod {};
    int products { -1 };
};

#endif
