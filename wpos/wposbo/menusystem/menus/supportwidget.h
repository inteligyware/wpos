// file      :  supportwidget.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef SUPPORTWIDGET_H
#define SUPPORTWIDGET_H

#include "ui_supportwidgetbase.h"
#include <QWidget>

class SupportWidget : public QWidget,
                      private Ui::SupportWidgetBase {
    Q_OBJECT
public:
    explicit SupportWidget(QWidget* parent = nullptr, const QString& name = QString {});
};

#endif // SUPPORTWIDGET_H
