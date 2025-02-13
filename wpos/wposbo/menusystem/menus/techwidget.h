// file      :  techwidget.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef TECHWIDGET_H
#define TECHWIDGET_H

#include <QString>
#include <QWidget>

#include "ui_techwidgetbase.h"

class TechWidget : public QWidget,
                   private Ui::TechWidgetBase {
    Q_OBJECT
public:
    explicit TechWidget(QWidget* parent = 0, const QString& name = QString {});
};

#endif // TECHWIDGET_H
