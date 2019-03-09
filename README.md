# Qt Hierarchical Header View
##
###
- - - -
- - - -
#### Copyright
This code is based on and modified from :
[HierarchicalHeaderView]( https://www.linux-apps.com/content/show.php/HierarchicalHeaderView?content=103154 )
 
Please check the licenses in the source code.
Copyright (c) 2009, Krasnoshchekov Petr
- - - -
- - - -

#### Brief

You can check the description on the above link. HierarchicalHeaderView is actually a QTableView that implements tree-like grouping of elements.

This code is mostly the same as original but offers a few extra things such as:
* fix errors for recent Qt versions
* easier hierarchy management
* automatic text alignment
* collapsible sections
* selectable header parent items
* some additional functions and explanations

The original code actually fares quite well given it is almost 10 years old. Good job Petr!

#### Usage

___main.cpp___ should cover most of the usage:

```
// Create root item
HT_Item* root = HT_Item::CreateRootItem("ROOT", RotationType::VERTICAL);

// Append some items to it
root->AppendNewItem("first");
root->AppendNewItem("second");

// ... bla bla bla ...
// ... why don't you just check the main.cpp ?

```

- - - -
- - - -

#### One more thing...
Have you found some time today to think about global warming ???
