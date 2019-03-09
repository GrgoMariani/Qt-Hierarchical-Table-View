/*
Copyright (c) 2019, Grgo Mariani
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Krasnoshchekov Petr ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Krasnoshchekov Petr BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "HTV/ht_model.h"
#include "HTV/customconstants.h"

/* ***************************************************************************************
 * Some info:
 *  Check the data() and headerData() methods
 * ***************************************************************************************/

HT_Model::HT_Model(QObject* parent) : QAbstractTableModel(parent)
{
    _columns_model = new QStandardItemModel;
    _rows_model = new QStandardItemModel;
}

HT_Model::~HT_Model()
{
    delete _columns_model;
    delete _rows_model;
}

int HT_Model::rowCount(const QModelIndex& /*parent*/) const
{
    if(_rows_model->item(0) == nullptr)
        return 0;
    return HT_Item::CastFromQStandardItem(_rows_model->item(0))->GetNumberOfChildren();
}

int HT_Model::columnCount(const QModelIndex& /*parent*/) const
{
    if(_columns_model->item(0) == nullptr)
        return 0;
    return HT_Item::CastFromQStandardItem(_columns_model->item(0))->GetNumberOfChildren();
}

HT_Model& HT_Model::setRow(HT_Item* rowItem)
{
    _rows_model->setItem(0, 0, rowItem);
    return *this;
}

HT_Model& HT_Model::setColumn(HT_Item* columnItem)
{
    _columns_model->setItem(0, 0, columnItem);
    return *this;
}

HT_Item* HT_Model::GetRootItem(Qt::Orientation orientation) const
{
    if(orientation == Qt::Horizontal)
        return HT_Item::CastFromQStandardItem(_columns_model->item(0));
    else
        return HT_Item::CastFromQStandardItem(_rows_model->item(0));
}

HT_Item* HT_Model::GetItemAtSection(int section, Qt::Orientation orientation) const
{
    QStandardItemModel* rowcol = (orientation==Qt::Horizontal) ? _columns_model : _rows_model;
    return (*HT_Item::CastFromQStandardItem(rowcol->item(0))).GetLeaf(section);
}

// data and headerData control what is shown
QVariant HT_Model::data(const QModelIndex& index, int role) const
{
    int _X_ = index.column();
    int _Y_ = index.row();
    if(role==CustomRoles::HT_HeaderView_Rows)
    {
        QVariant v;
        v.setValue(_rows_model);
        return v;
    }
    if(role==CustomRoles::HT_HeaderView_Columns)
    {
        QVariant v;
        v.setValue(_columns_model);
        return v;
    }
    if(role == Qt::ToolTipRole)
    {
        return QString("Tooltip ")+QString::number(_Y_)+QString("-")+QString::number(_X_);
    }
    if(role == Qt::DisplayRole && index.isValid())
    {
        return QString("Data ")+GetItemAtSection(_Y_, Qt::Vertical)->_name + GetItemAtSection(_X_, Qt::Horizontal)->_name;
    }
    return QVariant();
}

QVariant HT_Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::ToolTipRole)
    {
        if (orientation == Qt::Horizontal) {
            return QString("HTooltip ")+GetItemAtSection(section, orientation)->_name;
        }
        if (orientation == Qt::Vertical) {
            return QString("HTooltip ")+GetItemAtSection(section, orientation)->_name;
        }
    }
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            return GetRootItem(orientation)->GetLeaf(section)->_name;
        }
        if (orientation == Qt::Vertical) {
        }
    }
  return QVariant();
}
