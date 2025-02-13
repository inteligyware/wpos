// file      :  deleteinvitationscreenwidget.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef DELETEINVITATIONSCREENWIDGET_H
#define DELETEINVITATIONSCREENWIDGET_H

#include <QMenu>
#include <QWidget>

#include "ui_deletescreeninvitationsinterface.h"

class DeleteInvitationScreenWidget : public QMenu,
                                     private Ui::DeleteScreenInvitationsInterface {
    Q_OBJECT
public:
    explicit DeleteInvitationScreenWidget(QWidget* parent = 0, const QString& name = QString {});
    ~DeleteInvitationScreenWidget() = default;

    void setScreen(const QString& screen);
    void showSelectScreen();

signals:
    void screenDeleted(bool deleted);

private slots:
    void acceptSlot();
    void cancelSlot();
    void deleteSlot();
};

#endif
