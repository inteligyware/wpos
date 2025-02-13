// file      :  offerupdatewidget.h
// birth     :  4/19/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef OFFER_UPDATE_WIDGET_H
#define OFFER_UPDATE_WIDGET_H

#include "offereditionwidget.h"

class OfferUpdateWidget : public OfferEditionWidget {
    Q_OBJECT
public:
    explicit OfferUpdateWidget(QWidget* parent = 0, const QString& name = QString {});

public slots:
    virtual void clear() override;
    virtual void typeChangedSlot(const QString& type) override;
    virtual void offerChangedSlot(const QString& type) override;
    virtual void acceptSlot() override;

protected:
    virtual bool hasAllValues() override;
    virtual bool save() override;

private:
    void showOffersFromType(const QString& type); // unique
    bool searchIcon(const QString& type, const QString& name); // unique
};

#endif // OFFER_UPDATE_WIDGET_H
