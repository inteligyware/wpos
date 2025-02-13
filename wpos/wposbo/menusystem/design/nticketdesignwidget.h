// file      :  nticketdesignwidget.h
// birth     :  4/19/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef NTICKETDESIGNWIDGET_H
#define NTICKETDESIGNWIDGET_H

#include <QWidget>

#include "ui_ticketdesigninterface.h"

class NTicketDesignWidget : public QWidget,
                            private Ui::TicketDesignInterface {

    Q_OBJECT
public:
    explicit NTicketDesignWidget(QWidget* parent = 0, const QString& name = QString {});
    ~NTicketDesignWidget() = default;

public slots:
    void acceptSlot();
    void clear();
    void somethingChanged(const QString& text);

protected:
    virtual void showEvent(QShowEvent* e) override;
    bool readXml();
    void writeXml();
};

#endif
