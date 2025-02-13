// file      :  persticketwidget.h
// birth     :  5/11/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef PERSTICKETWIDGET_H
#define PERSTICKETWIDGET_H

#include "database/historicdb.h"
#include "ui_persticketswidgetbase.h"
#include <wposcore/config.h>

class QDate;
class HistoricDB;
class QPixmap;
class OrderView;
class QTreeWidgetItem;
class XmlConfig;
class QTimer;

class PersTicketWidget : public QWidget, private Ui::PersTicketsWidgetBase {
    Q_OBJECT
public:
    explicit PersTicketWidget(QWidget* parent = 0, const QString& name = QString {});
    ~PersTicketWidget(){ database.disConnect(); }

public slots:
    void clear();
    void clearOrder();

    void upTicketSlot();
    void downTicketSlot();

    void firstTicketSlot();
    void lastTicketSlot();

    void tableClickedSlot();

    void ticketSelectedSlot(QTreeWidgetItem* item);

    void timerDone();

signals:
    void genericDataSignal(const QString& signal_name, XmlConfig* xml);

protected:
    // void showEvent(QShowEvent* e);
    void getPersTickets();

    bool checkAllValues();

    void startGetingZ();
    void stopGettingZ();

private:
    void layoutItems();

    QPixmap null_pixmap     { "controls:view_text.png" };
    QPixmap error_pixmap    { "controls:stop.png" };
    QPixmap anulation_pixmap{ "controls:view_text_red.png" };

    HistoricDB database { "PersTicketWidget", cfg::xmlFileByKey(cfg::XMLKey::Database) };
    OrderView* order_view {};
    QTimer* timer {};
    int progress_step {};
};

#endif
