// file      :  submenuinfoframe.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "submenuinfoframe.h"
#include <QtWebEngineWidgets/QWebEngineView>

#include <QEvent>
#include <QLayout>

SubMenuInfoFrame::SubMenuInfoFrame(
    QWidget* parent,
    const QString& name)
    : QFrame(parent)
{
    setObjectName(name);

    layout = new QVBoxLayout(this);
    browser = new QWebEngineView(this);

    //@benes    setSizePolicy(parentWidget()->sizePolicy());
    setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    layout->addWidget(browser);
}

void SubMenuInfoFrame::load(const QString& infoFile)
{
    browser->setHtml(infoFile, QUrl("file://"));
}

void SubMenuInfoFrame::showEvent(QShowEvent* event)
{
    browser->resize(width(), height());
    QWidget::showEvent(event);
}
