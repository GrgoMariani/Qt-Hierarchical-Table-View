#include <QApplication>
#include <QDebug>
#include <QString>
#include <QTableView>

#include "HTV/ht_headerview.h"
#include "HTV/ht_model.h"
#include "HTV/ht_item.h"

/* ***************************************************************************************
 *
 * ***************************************************************************************/

void __CustomHierarchy__(HT_Item* rows, HT_Item* columns)
{
    // Define our table structure. It's possible to update it dynamically
	
    // Rows
    HT_Item* r1 = &rows->AppendNewItem("Item1");
    HT_Item* r2 = &rows->AppendNewItem("Item2");
    r1->AppendNewItem("subitem1");
    r1->AppendNewItem("subitem2");
    r1->AppendNewItem("subitem3");
    r1->AppendNewItem("subitem4");
    HT_Item* subitem3 = (*r1)[2];
    subitem3->AppendNewItem("subsubitem1");
    subitem3->AppendNewItem("subsubitem2");
    subitem3->AppendNewItem("subsubitem3");
    (*subitem3)[1]->AppendNewItem("subsubsubitem1");
    (*subitem3)[1]->AppendNewItem("subsubsubitem2");
    r2->AppendNewItem("subitem5");
    r2->AppendNewItem("subitem6");
    // Columns
    HT_Item* c1 = &columns->AppendNewItem("Item3");
    HT_Item* c2 = &columns->AppendNewItem("Item4");
    c1->AppendNewItem("subitem7");
    c1->AppendNewItem("subitem8");
    c1->AppendNewItem("subitem9");
    c2->AppendNewItem("subitemA");
    c2->AppendNewItem("subitemB");

    // delete example
    HT_Item* example_to_delete = &c2->AppendNewItem("subitemC").AppendNewItem("subitemD");
    c2->RemoveItem(example_to_delete);  // will delete only 'subitemD'
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HT_Item* rows       = HT_Item::CreateRootItem("VERTICAL HEADER", RotationType::VERTICAL);
    HT_Item* columns    = HT_Item::CreateRootItem("HORIZONTAL HEADER", RotationType::HORIZONTAL);

    __CustomHierarchy__(rows, columns);

    // Define the model with created item hierarchy
    QTableView tv;
    HT_Model* m = new HT_Model();
    m->setRow(rows).setColumn(columns);

    HT_HeaderView* horizontal_h = new HT_HeaderView(Qt::Horizontal, &tv);
    HT_HeaderView* vertical_h = new HT_HeaderView(Qt::Vertical, &tv);

    tv.setHorizontalHeader(horizontal_h);
    tv.setVerticalHeader(vertical_h);

    tv.setModel(m);

    // Optional
    // tv.horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);  tv.verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);   tv.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Show the table
    tv.show();
    return a.exec();
}
