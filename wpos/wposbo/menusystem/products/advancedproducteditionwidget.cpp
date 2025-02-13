// file      :  advancedproducteditionwidget.cpp
// birth     :  5/3/2004
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#include "advancedproducteditionwidget.h"
#include "menusystem/utils.h"

#include "productsmodule/productmodule.h"

#include <libbslxml/xmlconfig.h>
#include <wposcore/config.h>
#include <wposgui/common/toolkit.h>

#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <QTreeWidgetItem>

#include <iostream>
using namespace std;

static const QString& PRODUCTS_DTD { "dtddocs:products_productslist.dtd" };
static const int SCROLL_LENGTH { 150 };
static const double ICON_SIZE = 40.0;
static const uint TIME_OUT { 10 };

AdvancedProductEditionWidget::AdvancedProductEditionWidget(
    ProductModule* product_model,
    QWidget* parent, const QString& name)
    : QWidget(parent)
    , product_model { product_model }
{
    setupUi(this);
    setObjectName(name);
    setupWidgets();
    setWidgetsIcons();
    connectWidgets();
}

void AdvancedProductEditionWidget::setupWidgets(){
    product_listview->setColumnWidth(Icon, 90);
    product_listview->setColumnWidth(Code, 300);
    product_listview->setHeaderLabels({"Icon", "Code", "Name"});
    product_listview->setSelectionMode(QAbstractItemView::SingleSelection);
    product_listview->setAllColumnsShowFocus(true);
    stack->setCurrentWidget(progress_page);
}

void AdvancedProductEditionWidget::setWidgetsIcons(){
    ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    cancel_button->setIcon(QPixmap("controls48:button_cancel.png"));
    up_button->setIcon(QPixmap("controls48:up.png"));
    down_button->setIcon(QPixmap("controls48:down.png"));

    ok_delete_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    cancel_delete_button->setIcon(QPixmap("controls48:button_cancel.png"));
}

void AdvancedProductEditionWidget::connectWidgets(){
    connect(up_button, &QPushButton::clicked,
            this, &AdvancedProductEditionWidget::upButtonSlot);
    connect(down_button, &QPushButton::clicked,
            this, &AdvancedProductEditionWidget::downButtonSlot);
    connect(search_button, &QPushButton::clicked,
            this, &AdvancedProductEditionWidget::searchButtonSlot);
    connect(ok_button, &QPushButton::clicked,
            this, &AdvancedProductEditionWidget::acceptSlot);
    connect(cancel_button, &QPushButton::clicked,
            this, &AdvancedProductEditionWidget::cancelSlot);

    connect(this, &AdvancedProductEditionWidget::progressSignal,
            this, &AdvancedProductEditionWidget::setProgressSlot);

    connect(product_listview, &QTreeWidget::itemSelectionChanged,
            this, &AdvancedProductEditionWidget::productSelectedSlot);

    connect(product_listview->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &AdvancedProductEditionWidget::enableOkButton);

    connect(search_lineedit, &QLineEdit::textChanged,
            this, &AdvancedProductEditionWidget::nameChangedSlot);
}

void AdvancedProductEditionWidget::showEvent(QShowEvent* e)
{
    QTimer::singleShot(
        TIME_OUT,
        this,
        &AdvancedProductEditionWidget::startShowing);
    QWidget::showEvent(e);
}

void AdvancedProductEditionWidget::startShowing()
{
    if ( !product_page->isVisible() ){
        stack->setCurrentWidget(progress_page);
        fetchProducts();
        stack->setCurrentWidget(product_page);
    }
}

void AdvancedProductEditionWidget::clear()
{
    product_listview->clear();
    ok_button->setEnabled(isItemSelected());
    search_lineedit->clear();
    progress_label->clear();
    product_name_label->clear();
    progress_bar->setValue(0);
}

void AdvancedProductEditionWidget::fetchProducts()
{
    XmlConfig xml;
    if (!xml.readXmlFromString(product_model->getUnitaryProducts())) {
        cerr << "can not convert the string into xml "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }
    if (!xml.validateXmlWithDTD(PRODUCTS_DTD, true)) {
        cerr << "xml does not validate against dtd "
             << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    xml.delDomain();
    xml.setDomain("products");
    progress_bar->setMinimum(0);
    auto limit = xml.howManyTags("product");
    progress_bar->setMaximum(limit);

    for (auto i = 0; i < limit; i++) {
        xml.setDomain("product[" + QString::number(i) + "]");
        auto name = xml.readString("name");
        auto code = xml.readString("code");

        if (name.isEmpty()
            || code.isEmpty()
            || (name == "varios" && code == "varios"))
            continue;

        auto item = new QTreeWidgetItem(product_listview);
        if (!xml.readString("logo").isEmpty()) {
            auto icon_path = cfg::PRODUCT_DIR + xml.readString("logo");
            item->setIcon(Icon, cropedIcon(icon_path, ICON_SIZE));
        }
        item->setText(Code, code);
        item->setText(Name, name);
        xml.releaseDomain("product", true);
        emit progressSignal(i, name);
    }
    xml.releaseDomain("products");
}

void AdvancedProductEditionWidget::setProgressSlot(int num, const QString& product)
{
    progress_bar->setValue(num);
    progress_label->setText(product);
    qApp->processEvents();
}

void AdvancedProductEditionWidget::enableOkButton(){
    ok_button->setEnabled(isItemSelected());
}

void AdvancedProductEditionWidget::upButtonSlot()
{
    // product_listview->scroll(0, -SCROLL_LENGTH);
    selectTreeItemUp(product_listview);

}

void AdvancedProductEditionWidget::downButtonSlot()
{
    // product_listview->scroll(0, SCROLL_LENGTH);
    selectTreeItemDown(product_listview);
}

void AdvancedProductEditionWidget::productSelectedSlot()
{
    auto item = selectedItem();
    if (!item) return;

    auto code = item->text(Code);
    if (item->childCount()) {
        item->setExpanded(!item->isExpanded());
        return;
    }

    XmlConfig xml;
    if (!xml.readXmlFromString(product_model->getCompositionsWithIngredient(code))) {
        cerr << "cannot convert the string into xml " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if (!xml.validateXmlWithDTD(PRODUCTS_DTD, true)) {
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }
    xml.delDomain();
    xml.setDomain("products");

    for (auto i = 0; i < xml.howManyTags("product"); i++) {
        auto new_item = new QTreeWidgetItem(item);
        auto name = xml.readString("product[" + QString::number(i) + "].name");
        auto code = xml.readString("product[" + QString::number(i) + "].code");
        new_item->setIcon(Icon, item->icon(Icon));
        new_item->setText(Code, code);
        new_item->setText(Name, name);
    }
    item->setExpanded(true);
    xml.releaseDomain("products", true);
}

bool AdvancedProductEditionWidget::isItemSelected()
{
    return product_listview->selectionModel()->hasSelection();
}

void AdvancedProductEditionWidget::nameChangedSlot(const QString& text)
{

    if (text.isEmpty()) {
        product_listview->clearSelection();
        return;
    }

    auto items = product_listview->findItems(text, Qt::MatchExactly, Name);

    if (!items.isEmpty()) {
        QTreeWidgetItem* item = items.first();
        product_listview->setCurrentItem(item);
        product_listview->scrollToItem(item);
    }
}

void AdvancedProductEditionWidget::searchButtonSlot()
{
    nameChangedSlot(search_lineedit->text());
}

void AdvancedProductEditionWidget::cancelSlot()
{
    startShowing();
}

QTreeWidgetItem* AdvancedProductEditionWidget::selectedItem() const
{
    auto source = qobject_cast<QTreeWidget*>(sender());

    if ( !source || source->selectedItems().isEmpty() )
        return nullptr;
    return source->selectedItems().first();
}
