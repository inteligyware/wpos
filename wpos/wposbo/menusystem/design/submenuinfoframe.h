// file      :  submenuinfoframe.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef BSLSUBMENUINFO_H
#define BSLSUBMENUINFO_H

#include <QFrame>

class QVBoxLayout;
class Browser;
class QShowEvent;
class QWebEngineView;

class SubMenuInfoFrame : public QFrame {
    Q_OBJECT
public:
    explicit SubMenuInfoFrame(
        QWidget* parent = 0,
        const QString& name = QString {});

    ~SubMenuInfoFrame() = default;

    void load(const QString& infoFile);
    void showEvent(QShowEvent* event) override;

private:
    QVBoxLayout* layout {};
    QWebEngineView* browser {};
};

#endif
