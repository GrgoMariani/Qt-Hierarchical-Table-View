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

#include "HTV/ht_item.h"

/* ***************************************************************************************
 * Some info:
 *  HT_Item class could be best described as a node in a tree like structure.
 *  There can be only one root element and all the other elements must be created from it.
 *  We can collapse some items by replacing them with a temporary placeholder/doppelganger.
 *  operator[] is overridden so we can get a child at required position.
 *  All the items share a pointer to their shared main tree information.
 * ***************************************************************************************/

struct HT_Item::TreeInfo
{
    HT_Item* const root_node;
    RotationType    _rotation;
    const int       _maxDepth;

    TreeInfo(HT_Item* htvitem, RotationType rotation, int maxDepth)
        : root_node(htvitem), _rotation(rotation), _maxDepth(maxDepth)
    {
        _rotation = ( rotation==RotationType::DEFAULT ? RotationType::HORIZONTAL : rotation );
    }
};

HT_Item::~HT_Item()
{
    for(int i=Size(); i>=0; --i)
        removeRow(0);
    if(_depth == 0)                                                         // if root node -> delete tree data
        delete _treeinfo;
}

HT_Item* HT_Item::CreateRootItem(const QString& name, RotationType rotation, int maxDepth)
{
    if(rotation==RotationType::DEFAULT)
        rotation=RotationType::HORIZONTAL;
    HT_Item* newItem = new HT_Item(name, rotation, nullptr, 0);
    newItem->_treeinfo = new HT_Item::TreeInfo(newItem, rotation, maxDepth);    // This is our only constructor for the complete data structure
    return newItem;
}

HT_Item* HT_Item::CastFromQStandardItem(QStandardItem* qsitem)
{
    return dynamic_cast<HT_Item*>(qsitem);
}

HT_Item& HT_Item::AppendNewItem(const QString newName, RotationType rotation)
{
    if(_treeinfo->_maxDepth>-1 && _treeinfo->_maxDepth<_depth+1)              // Make sure we don't cross our maximum depth
        return *this;
    if(rotation==RotationType::DEFAULT)
        rotation= (_treeinfo->_rotation==RotationType::HORIZONTAL) ? RotationType::VERTICAL : RotationType::HORIZONTAL;
    SetRotation(_treeinfo->_rotation);
    HT_Item* newItem = new HT_Item(newName, rotation, this, _depth+1);    //           Create Item -> ____
    this->appendRow(CastFromQStandardItem(newItem));                        //  ____ -> Append it as a child
    return *newItem;
}

bool HT_Item::RemoveItem(HT_Item* child)
{
    child = child->PlaceholderItemRemove();     // In case child is a placeholder
    if( _depth>=child->_depth )
        return false;
    int r = child->_depth;
    HT_Item* phtv = child;
    while(r != _depth)
    {
        phtv = phtv->_parent;
        r--;
    }
    if(phtv != this)
        return false;
    if( child->_parent->Size()==1 && child->_parent!=_treeinfo->root_node )
        child->_parent->SetRotation(_rotation==RotationType::HORIZONTAL ? RotationType::VERTICAL : RotationType::HORIZONTAL);
    child->_parent->removeRow(child->row());
    return true;
}

int HT_Item::Size()
{
    return this->rowCount();
}

HT_Item* HT_Item::PlaceholderItemPut()
{
    if( _depth==0 && row()>-1 )                                             // This function can only be used on children items
        return this;                                                        // which don't have placeholders currently active
    if(!hasChildren())                                                      // As well as the prerequisite that
        return this;                                                        // They must have some children
    int trow = row();
    _parent->takeRow(trow);                                                 // Usually returns a pointer to this
    RotationType newRotation = (_treeinfo->_rotation==RotationType::HORIZONTAL) ? RotationType::VERTICAL : RotationType::HORIZONTAL;
    auto* newItem = new HT_Item(QString("*")+_name, newRotation, _parent,
                                 _depth, ItemTypeEnum::PLACEHOLDER);        //           Create Item -> ____
    newItem->SetRotation(newRotation);                                      //  ____ -> Set Rotation -> ____
    _parent->insertRow(trow, CastFromQStandardItem(newItem));               //  ____ -> Append it as a child to parent
    newItem->_doppelganger = this;                                          // Link these two items
    _doppelganger = newItem;
    return this;
}

HT_Item* HT_Item::PlaceholderItemRemove()
{
    HT_Item *p = IsPlaceholder() ? _doppelganger : this;                   // We want the option
    if(_doppelganger == nullptr)                                            // of calling this function directly from
        return this;                                                        // the doppelganger
    int trow = p->_doppelganger->row();
    _parent->takeRow(trow);
    delete p->_doppelganger;
    p->_doppelganger = nullptr;
    _parent->insertRow(trow, p);
    return p;
}

HT_Item* HT_Item::PlaceholderToggle()
{
    HT_Item* res;
    if( IsPlaceholder() )
        res = PlaceholderItemRemove();
    else
        res = PlaceholderItemPut();
    return res;
}

HT_Item* HT_Item::operator[](int num)
{
    if(num<0 || num>=Size())
        return this;
    return HT_Item::CastFromQStandardItem(this->child(num));
}

HT_Item* HT_Item::GetLeaf(int num)
{
    if(num<0)
        return this;
    if(Size()==0)
        return this;
    for (int sum=0, trow=0; sum<=num; trow++)
    {
        HT_Item* temph = (*this)[trow];
        int res = temph->GetNumberOfChildren();
        if(num<sum+res)
            return (*temph).GetLeaf(num-sum);
        sum += res;
    }
    return this;
}

HT_Item* HT_Item::GetParentItemAtDepth(int depth)
{
    if( depth<0 )
        return this->_treeinfo->root_node;
    HT_Item* p = this;
    while(depth < p->_depth )
    {
        p = p->_parent;
    }
    return p;
}

bool HT_Item::IsPlaceholder()
{
    return _type==ItemTypeEnum::PLACEHOLDER;
}

int HT_Item::GetNumberOfChildren(int wantedDepth)
{
    if(_depth==wantedDepth)
        return 1;
    if(!hasChildren())
    {
        if(wantedDepth != -2)   // special case in case we only
            return 0;           // wish to count at specific depth
        return 1;
    }
    int result = 0;
    for(int i=0; i<Size(); i++)
        result += CastFromQStandardItem(child(i))->GetNumberOfChildren();
    return result;
}

HT_Item& HT_Item::SetRotation(RotationType rotation)
{
    if(rotation == RotationType::VERTICAL)
        this->setData(1, CustomRoles::Item_Rotated);
    else
        this->setData(QVariant(), CustomRoles::Item_Rotated);
    _rotation=rotation;
    return *this;
}

HT_Item::HT_Item(const QString& name, RotationType rotation, HT_Item* parent, int depth, ItemTypeEnum type)
    : QStandardItem (name), _name(name), _rotation(rotation), _parent(parent), _doppelganger(nullptr), _depth(depth), _type(type)
{
    if(depth>0)                                                             // Inherit the info from the parent
        _treeinfo = _parent->_treeinfo;
    this->setToolTip(_name);
    SetRotation(_rotation);
}
