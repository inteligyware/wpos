// file      :  instantiator.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef INSTANTIATOR_H
#define INSTANTIATOR_H

#include <QObject>
#include <QWidget>
#include <wposcore/hlist.h>

// class XmlConfig;

class UserModule;
class DatabaseModule;
class ProductModule;

class Instantiator : public QObject {
    Q_OBJECT
public:
    explicit Instantiator(QWidget* parent = 0, const QString& name = QString {});
    ~Instantiator() = default;

    bool hasMenu(const QString& menuName);

public slots:
    QWidget* getMenu(QWidget* parent, const QString& menuName); //@benes , maybe getSubMenu
    void showMenu(const QString& pageName);

signals:
    void showPageMenu(const QString& pageName);
    void splashRequested(const QString& msg, int flags, const QColor& color);

protected:
    void initDcopModules();
    void initSystemConfigMenu();
    void initUsersMenu();
    void initProductsMenu();
    void initDesignMenu();
    void initOffersAndOptionsMenu();
    void initHistoricMenu();
    void initReportMenu();
    void initSupportMenu();
    void initStickMenu();

    HList<QWidget> menus;
    QWidget* parent {};

    UserModule* user_mod {};
    DatabaseModule* db_mod {};
    ProductModule* product_mod {};

    Qt::Alignment splash_align { Qt::AlignBottom | Qt::AlignRight };
    Qt::GlobalColor splash_color { Qt::darkBlue };
};

#endif
