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

#ifndef HT_MODEL_H
#define HT_MODEL_H

#include <QVariant>
#include <QAbstractTableModel>
#include "HTV/ht_item.h"

class HT_Model : public QAbstractTableModel
{
    Q_OBJECT
    /* -----------------------VARIABLES-------------------------- */
public:
protected:
private:
    QStandardItemModel* _rows_model;
    QStandardItemModel* _columns_model;

    /* ------------------------METHODS--------------------------- */
public:
    HT_Model(QObject* parent=nullptr);
    ~HT_Model();
    int         rowCount(const QModelIndex& /*parent*/) const;
    int         columnCount(const QModelIndex& /*parent*/) const;

    HT_Model&   setRow(HT_Item* rowItem);
    HT_Model&   setColumn(HT_Item* columnItem);

    HT_Item*    GetRootItem(Qt::Orientation orientation) const;
    HT_Item*    GetItemAtSection(int section, Qt::Orientation orientation) const;

    void        emit_layoutChanged() { emit layoutChanged(); }

    // data and headerData control what is shown
    QVariant    data(const QModelIndex& index, int role) const;
    QVariant    headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
protected:
private:
    /* ----------------------SLOTS AND SIGNALS------------------- */
signals:

public slots:

};


#endif // HT_MODEL_H
