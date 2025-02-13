// file      :  nprodandofferreportwidget.h
// birth     :  6/6/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef NPRODANDOFFERREPORTWIDGET_H
#define NPRODANDOFFERREPORTWIDGET_H

#include "reportinterface.h"
#include "ui_prodandofferreportwidgetbase.h"

#include <QWidget>
#include <QButtonGroup>

class PdfViewer;
class UserModule;
class QTimer;

class NProdAndOfferReportWidget : public QWidget,
                                  virtual public ReportInterface,
                                  private Ui::ProdAndOfferReportWidgetBase {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.ProductAndOfferReport")

public:
    const QString DBusObjectPath { "/wpos/wposbo/ProductAndOfferReport" };
    explicit NProdAndOfferReportWidget(
        UserModule* user_module,
        QWidget* parent = 0,
        const QString& name = QString{});
    ~NProdAndOfferReportWidget();

signals:
    void reportCreated(const QString& name);

public slots:
    Q_SCRIPTABLE virtual void reportSlot(QString) override; // ASYNC

protected slots:
    void clear();
    virtual void toggleStates(QAbstractButton* button);

    void upUsersSlot();
    void downUsersSlot();

    void advancedButtonToggleSlot(bool on);
    void usersBoxCheckedSlot(bool on);
    void selectedUserSlot();

    void acceptSlot();
    void timerSlot();

    void saveReportSlot();
    void exitFromViewSlot();

protected:
    void init();
    virtual void showEvent(QShowEvent* e) override;
    virtual void hideEvent(QHideEvent* e) override;
    virtual bool doIsClosing() const;
    virtual void doSelectReport(
        QString& start_date,
        QString& end_date,
        const QString& informe = QString());
    virtual void doSetOutputStream(
        QDataStream& out_stream,
        const QString& informe,
        const QString& start_date,
        const QString& end_date,
        const QString& employee_id);
    void getUsers();
    void startShowing();

    QString getActualReport() const;
    bool checkAllValues();
    bool checkReportModule();
    bool checkStickModule();

    UserModule* mod{};
    PdfViewer* viewer{};
    QTimer* timer{};
    QButtonGroup* button_group {};

    QString last_report_file;
    QString last_report_name;

    int pos;
};

#endif
