// file      :  addinvitationscreenwidget.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76


#ifndef ADDINVITATIONSCREENWIDGET_H
#define ADDINVITATIONSCREENWIDGET_H

#include "ui_newscreeninvitationsinterface.h"
#include <QMenu>
#include <QObject>

class ProductOfferScreenData;
class XmlConfig;
class QListWidget;

class AddInvitationScreenWidget : public QMenu,
                                  private Ui::NewScreenInvitationsInterface {
    Q_OBJECT
public:
    explicit AddInvitationScreenWidget(QWidget* parent = 0, const QString& name = QString {});
    ~AddInvitationScreenWidget() = default;

public slots:
    void apply();
    void cancel();
    void clear();
    void setScreen(const ProductOfferScreenData* screen);
    void showLogos();

signals:
    void screenCreated(ProductOfferScreenData* new_screen);
    void screenUpdated(ProductOfferScreenData* screen, const QString& last_screen);

protected slots:
    void upButtonSlot();
    void downButtonSlot();
    void nameChangedSlot(const QString& text);

protected:
    void showEvent(QShowEvent* e);
    void hideEvent(QHideEvent* e);

    ProductOfferScreenData* updated_screen {};
    QString selected_logo;
};

#endif
