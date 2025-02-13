// file      :  nprodandofferreportwidget.cpp
// birth     :  6/6/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "nprodandofferreportwidget.h"
#include "nprodandofferreportwidget_adaptor.h"

#include "usersmodule/usermodule.h"
#include <libbslxml/xmlconfig.h>
#include <wposgui/viewer/pdf_viewer.h>

#include <QButtonGroup>
#include <QFrame>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QListView>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QStackedWidget>
#include <QString>
#include <QTimer>

#include <QApplication>
#include <QDataStream>
#include <QLocale>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QStringList>
#include <QTabBar>
#include <QDateTime>

#define TIMER_SLOT 500

#define BUTTON_OFF QColor(238, 238, 230)
#define BUTTON_ON QColor(107, 181, 251)

#define USER_PIXMAP "controls32:personal.png"

#define USERS_LIST_DTD "dtddocs:users_userslist.dtd"
#define USERS_DTD "dtddocs:users_user.dtd"

#include <iostream>
using namespace std;

NProdAndOfferReportWidget::NProdAndOfferReportWidget(
    UserModule* user_module,
    QWidget* parent, const QString& name)
    : QWidget(parent)
    , mod { user_module }
{
    setupUi(this);
    setObjectName(name);

    new ProductAndOfferReportAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(NProdAndOfferReportWidget::DBusObjectPath, this);

    init();
}

NProdAndOfferReportWidget::~NProdAndOfferReportWidget()
{
    delete timer;
}

void NProdAndOfferReportWidget::init()
{

    // unique
    productos_button->setIcon(QPixmap("controls48:productos.png"));
    ofertas_button->setIcon(QPixmap("controls48:ksplash.png"));
    opciones_button->setIcon(QPixmap("controls48:ksplash.png"));
    prod_unitarios_button->setIcon(QPixmap("controls48:producto.png"));
    ofertas_prod_unitarios_button->setIcon(QPixmap("controls48:ksplash.png"));
    opciones_prod_unitarios_button->setIcon(QPixmap("controls48:ksplash.png"));
    precios_button->setIcon(QPixmap("controls48:modificar_productos.png"));

    ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    up_employee_button->setIcon(QPixmap("controls32:up.png"));
    down_employee_button->setIcon(QPixmap("controls32:down.png"));
    advanced_button->setIcon(QPixmap("controls32:run.png"));

    save_report_button->setIcon(QPixmap("controls48:stick_48x48.png"));
    exit_button->setIcon(QPixmap("controls48:button_cancel.png"));

    // unique
    start_gv_button->setIcon(QPixmap("controls32:start.png"));
    prev_gv_button->setIcon(QPixmap("controls32:2leftarrow.png"));
    down_gv_button->setIcon(QPixmap("controls32:down.png"));
    zoomin_gv_button->setIcon(QPixmap("controls32:viewmag+.png"));
    zoomout_gv_button->setIcon(QPixmap("controls32:viewmag-.png"));
    up_gv_button->setIcon(QPixmap("controls32:up.png"));
    next_gv_button->setIcon(QPixmap("controls32:2rightarrow.png"));
    last_gv_button->setIcon(QPixmap("controls32:finish.png"));

    employee_listview->sortByColumn(2, Qt::AscendingOrder);
    employee_listview->header()->hide();
    employee_listview->setSelectionMode(QListView::SingleSelection);
    employee_listview->setAllColumnsShowFocus(true);
    //    employee_listview->setColumnAlignment(1,Qt::AlignCenter); //@benes
    employee_listview->setColumnWidth(0, 50);
    employee_listview->setColumnWidth(1, 300);

    QVBoxLayout* vlayout = 0;
    if (!(vlayout = (QVBoxLayout*)kghostview_frame->layout()))
        vlayout = new QVBoxLayout(kghostview_frame);

    viewer = new PdfViewer(kghostview_frame); //@benes
    viewer->setWindowFlags(Qt::Widget);
    // viewer->statusBar()->hide();
    // viewer->menuBar()->hide();
    vlayout->addWidget(viewer);

    timer = new QTimer();

    //@benes
    //    connect(start_gv_button,SIGNAL(clicked()), part, SLOT(slotGotoStart()));
    //    connect(prev_gv_button,SIGNAL(clicked()), part, SLOT(slotPrevPage()));
    //    connect(up_gv_button,SIGNAL(clicked()), part, SLOT(slotScrollUp()));
    //    connect(zoomin_gv_button,SIGNAL(clicked()), part,SLOT(slotZoomIn()));
    //    connect(zoomout_gv_button,SIGNAL(clicked()), part,SLOT(slotZoomOut()));
    //    connect(down_gv_button,SIGNAL(clicked()), part,SLOT(slotScrollDown()));
    //    connect(next_gv_button,SIGNAL(clicked()), part,SLOT(slotNextPage()));
    //    connect(last_gv_button,SIGNAL(clicked()), part,SLOT(slotGotoEnd()));

    // unique
    button_group = new QButtonGroup(report_buttongroup);
    button_group->addButton(productos_button);
    button_group->addButton(ofertas_button);
    button_group->addButton(opciones_button);
    button_group->addButton(prod_unitarios_button);
    button_group->addButton(ofertas_prod_unitarios_button);
    button_group->addButton(opciones_prod_unitarios_button);
    button_group->addButton(precios_button);
    button_group->setExclusive(true);

    connect(button_group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
        this, &NProdAndOfferReportWidget::toggleStates);

    connect(advanced_button, &QPushButton::toggled,
            this, &NProdAndOfferReportWidget::advancedButtonToggleSlot);
    connect(up_employee_button, &QPushButton::clicked,
            this, &NProdAndOfferReportWidget::upUsersSlot);
    connect(down_employee_button, &QPushButton::clicked,
            this, &NProdAndOfferReportWidget::downUsersSlot);
    connect(employee_groupbox, &QGroupBox::toggled,
            this, &NProdAndOfferReportWidget::usersBoxCheckedSlot);
    connect(employee_listview, &QTreeWidget::itemSelectionChanged,
            this, &NProdAndOfferReportWidget::selectedUserSlot);

    connect(ok_button, &QPushButton::clicked,
            this, &NProdAndOfferReportWidget::acceptSlot);
    connect(exit_button, &QPushButton::clicked,
            this, &NProdAndOfferReportWidget::exitFromViewSlot);
    connect(save_report_button, &QPushButton::clicked,
            this, &NProdAndOfferReportWidget::saveReportSlot);

    //    connectDCOPSignal(0,0,"reportCreated(QString)","reportSlot(QString)", false); //@benes
    connect(this, &NProdAndOfferReportWidget::reportCreated,
            &NProdAndOfferReportWidget::reportSlot); //@benes

}

void NProdAndOfferReportWidget::showEvent(QShowEvent* e)
{
    if (!advanced_button->isDown())
        advanced_button->toggle();
    init_date_groupbox->hide();
    end_date_groupbox->hide();
    startShowing();
    QWidget::showEvent(e);
}

void NProdAndOfferReportWidget::hideEvent(QHideEvent* e)
{
    //    if (stack->visibleWidget()==view_page) part->closeURL(); //@benes
    QWidget::hideEvent(e);
}

void NProdAndOfferReportWidget::startShowing()
{
    clear();
    if (!checkReportModule())
        stack->setCurrentWidget(error_page);
    else
        stack->setCurrentWidget(main_page);
    getUsers();
}

void NProdAndOfferReportWidget::clear()
{
    employee_listview->clear();
    employee_groupbox->setChecked(false);
    for (auto button : button_group->buttons()) {
        if (!button->isDown())
            button->toggle();
        button->setPalette(QPalette(QColor(BUTTON_OFF)));
    }
    ok_button->setEnabled(checkAllValues());
}

void NProdAndOfferReportWidget::getUsers()
{
    employee_listview->clear();
    QString xml_string = mod->usersList(true);
    XmlConfig xml;
    xml.readXmlFromString(xml_string);
    if (!xml.wellFormed()
        || !xml.validateXmlWithDTD(USERS_LIST_DTD, true)) {
        cerr << "XML parse error or XML does not validate against dtd"
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << '\n';
        return;
    }

    xml.delDomain();
    xml.setDomain("users");
    for (auto i = 0; i < xml.howManyTags("user"); i++) {
        auto employee_id = xml.readString("user[" + QString::number(i) + "].employee_id");
        auto name = xml.readString("user[" + QString::number(i) + "].name");
        auto last_name = xml.readString("user[" + QString::number(i) + "].last_name");
        // auto picture = xml.readString("user[" + QString::number(i) + "].picture"); @benes

        if (employee_id == "1")
            continue; // do not show the system user.

        auto item = new QTreeWidgetItem(employee_listview);
        item->setIcon(0, QPixmap(USER_PIXMAP));
        // item->setIcon(0, QPixmap(picture)); @benes
        item->setText(1, name + " " + last_name);
        item->setText(2, employee_id);
    }
    employee_listview->clearSelection();
    ok_button->setEnabled(checkAllValues());
}

bool NProdAndOfferReportWidget::doIsClosing() const
{
    return getActualReport() == "precios_button";
}

QString NProdAndOfferReportWidget::getActualReport() const
{
    auto button = button_group->checkedButton();
    if (button) return button->objectName();
    return QString {};
}

void NProdAndOfferReportWidget::toggleStates(QAbstractButton* button)
{
    button->setBackgroundRole(QPalette::Button);
    for (auto others : button_group->buttons())
        if (others != button)
            others->setPalette(button->palette());
    button->setPalette(QPalette(QColor(BUTTON_ON)));

    if (doIsClosing()) {
        employee_groupbox->setChecked(false);
        employee_groupbox->setEnabled(false);
    } else
        employee_groupbox->setEnabled(true);
}

void NProdAndOfferReportWidget::advancedButtonToggleSlot(bool on)
{
    if (!advanced_button->isDown()) {
        init_date_groupbox->show();
        end_date_groupbox->show();
    } else {
        init_date_groupbox->hide();
        end_date_groupbox->hide();
    }
    ok_button->setEnabled(checkAllValues());
}

void NProdAndOfferReportWidget::usersBoxCheckedSlot(bool on)
{
    // employee_listview->clear();
    // if (employee_groupbox->isChecked()){
    //     getUsers();
        ok_button->setEnabled(checkAllValues());
    // }
}

bool NProdAndOfferReportWidget::checkAllValues()
{
    return !getActualReport().isEmpty()
           && employee_groupbox->isChecked()
           && !employee_listview->selectedItems().isEmpty();
}

void NProdAndOfferReportWidget::upUsersSlot()
{
    // employee_listview->scrollBy(0, -70);
}

void NProdAndOfferReportWidget::downUsersSlot()
{
    // employee_listview->scrollBy(0, 70);
}

bool NProdAndOfferReportWidget::checkReportModule()
{
    // DCOPClient* client = 0;
    // client = kapp->dcopClient();
    // if (!client->isAttached())
    //     client->attach();

    // // if the dcopreceipt is not running  exit without saving
    // return (client->isApplicationRegistered("dcopreports"));
    return true;
}

bool NProdAndOfferReportWidget::checkStickModule()
{
    // DCOPClient* client = 0;
    // client = kapp->dcopClient();
    // if (!client->isAttached())
    //     client->attach();

    // // if the dcopreceipt is not running  exit without saving
    // return (client->isApplicationRegistered("dcopstick"));
    return false;
}

void NProdAndOfferReportWidget::selectedUserSlot()
{
    ok_button->setEnabled(checkAllValues());
}

void NProdAndOfferReportWidget::acceptSlot()
{
    if (!checkReportModule()) {
        stack->setCurrentWidget(error_page);
        return;
    }

    QString informe = getActualReport();
    informe.replace("_button", "");

    QString employee_id;
    if (employee_groupbox->isChecked()) {
        if (employee_listview->selectedItems().isEmpty())
            return;
        auto item = employee_listview->selectedItems().first();
        employee_id = item->text(2);
    }

    QString start_date, end_date;
    if (!advanced_button->isDown()) {
        if (init_datepicker->date() > end_datepicker->date()) {
            QString text = tr("The dates entered are not correct.\n\The start date is before the end date\n");
            QMessageBox::warning(this, tr("Can't get report"), text);
            return;
        }
        start_date = init_datepicker->date().toString("dd/MM/yyyy");
        end_date = end_datepicker->date().toString("dd/MM/yyyy");
    }
    else
        doSelectReport(informe, start_date, end_date);

    QByteArray data, reply_data;
    QDataStream out_stream(&data, QIODevice::ReadWrite);
    QDataStream reply(&reply_data, QIODevice::ReadWrite);

    doSetOutputStream(out_stream, informe, start_date, end_date, employee_id);

    QString reply_type;
    // idem
    // if (!client->call("dcopreports", "DCOPReportReportman", "createReport(QString,QString,QString,QString)", data, reply_type, reply_data)) {
    //     cerr << "problems while trying to call dcopreports" << endl;
    //     QString text = tr("Ha fallado la llamada al modulo de informes.\n");
    //     QMessageBox::warning(this, tr("ha fallado la generacion de informes"), text, 0);
    // }

    bool ret { false };
    if (reply_type == "bool")
        reply >> ret;

    //    if (!ret){
    //        QString text = tr("The call to the report module has failed.\n");
    //        QMessageBox::warning(this,tr("Report Generation Failiure"),text);
    //        startShowing();
    //        return;
    //    }

    last_report_name = informe;
    if (!start_date.isEmpty()) last_report_name += "_" + start_date;
    if (!end_date.isEmpty())  last_report_name += "_" + end_date;
    if (!employee_id.isEmpty()) last_report_name += "_" + employee_id;

    last_report_name += ".pdf";
    last_report_name.replace('/', "-");

    stack->setCurrentWidget(progress_page);
    progress_bar->setValue(0);
    timer->start(5);
    connect(
        timer, &QTimer::timeout,
        this, &NProdAndOfferReportWidget::timerSlot);
    pos = 0;
    emit reportCreated(last_report_name);
}

void NProdAndOfferReportWidget::doSelectReport(
    QString& start_date,
    QString& end_date,
    const QString& informe)
{
    (void)informe;
    // year
    QDate aux_date, date;
    if (informe == tr("anual")){
        date = QDate::currentDate();
        aux_date.setDate(date.year(), 1, 1);
        start_date = aux_date.toString("dd/MM/yyyy");
        aux_date.setDate(date.year(), 12, 31);
        end_date = aux_date.toString("dd/MM/yyyy");
    }

    // month
    // date = QDate::currentDate();
    // aux_date.setYMD(date.year(),date.month(),1);
    // start_date = aux_date.toString("dd/MM/yyyy");
    // aux_date.setYMD(date.year(),date.month(),date.daysInMonth());
    // end_date = aux_date.toString("dd/MM/yyyy");

    // week
    // int first_day,last_day;
    // date = QDate::currentDate();
    // first_day = date.day() - date.dayOfWeek();
    // last_day = date.day() + (7-date.dayOfWeek());
    // aux_date.setYMD(date.year(),date.month(),first_day);
    // start_date = aux_date.toString("dd/MM/yyyy");
    // aux_date.setYMD(date.year(),date.month(),last_day);
    // end_date = aux_date.toString("dd/MM/yyyy");

    // day
    // date = QDate::currentDate();
    // aux_date.setYMD(date.year(),date.month(),date.day());
    // start_date = aux_date.toString("dd/MM/yyyy");
    // aux_date.setYMD(date.year(),date.month(),date.day());
    // end_date = aux_date.toString("dd/MM/yyyy");
}

void NProdAndOfferReportWidget::doSetOutputStream(
    QDataStream& out_stream, const QString& informe,
    const QString& start_date, const QString& end_date,
    const QString& employee_id)
{
    // client = kapp->dcopClient();
    out_stream << informe;
    if (informe == "precios") {
        out_stream << "";
        out_stream << "";
        out_stream << "";
    } else {
        //   out_stream << QString(start_date+" 00:00:00");
        //   out_stream << QString(end_date+" 23:59:59");
        out_stream << QString(start_date);
        out_stream << QString(end_date);
        out_stream << employee_id;
    }
}

void NProdAndOfferReportWidget::timerSlot()
{
    pos++;
    progress_bar->setValue(pos);
    qApp->processEvents();
}

void NProdAndOfferReportWidget::reportSlot(QString file)
{
    file = "share/wpos/wposbo/wposbo.pdf";
    QObject::disconnect(
        timer, &QTimer::timeout,
        this, &NProdAndOfferReportWidget::timerSlot);
    progress_bar->setValue(0);
    timer->stop();

    if (file.isEmpty()) {
        QString text = tr("The call to the report module has failed.\n");
        QMessageBox::warning(this, tr("Report Generation Failiure"), text);
        startShowing();
        return;
    }

    last_report_file = file;
    if (checkStickModule())
        save_report_button->show();
    else
        save_report_button->hide();

    stack->setCurrentWidget(view_page);
    viewer->open(QUrl::fromLocalFile(file));
}

void NProdAndOfferReportWidget::exitFromViewSlot()
{
    viewer->close();
    startShowing();
}

void NProdAndOfferReportWidget::saveReportSlot()
{
    // will be implemented by the pdfviewer
    // bool ret = false;
    // DCOPClient* client = 0;
    // QCString reply_type;
    // QByteArray data, reply_data;
    // QDataStream out_stream(data, IO_ReadWrite);
    // QDataStream reply(reply_data, IO_ReadWrite);

    // client = kapp->dcopClient();
    // out_stream << last_report_file;
    // out_stream << last_report_name;

    // if (!client->call("dcopstick", "dcopstick", "saveReportToStick(QString,QString)",
    //         data, reply_type, reply_data)) {
    //     cerr << "problems while trying to call dcopstick module" << endl;
    //     QString text = tr("Ha fallado la llamada al modulo de guardar a memory stick .\n");
    //     QMessageBox::warning(this, tr("ha fallado la operacion de guardar en el stick"), text, 0);
    // }
    // if (reply_type == "bool")
    //     reply >> ret;

    // if (!ret) {
    //     QString text = tr("ha fallado la operacion de guardar en el stick.\n");
    //     text += tr("puede que este no este pinchado\n en una ranura USB o este ya montado");
    //     QMessageBox::warning(this, tr("ha fallado la operacion de guardar en el stick"), text, 0);
    //     return;
    // }
}
