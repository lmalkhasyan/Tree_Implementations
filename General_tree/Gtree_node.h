#ifndef GTREE_NODE
#define GTREE_NODE

#include "Gtree.h"
#include <vector>

struct Gtree_node 
{
    using node_pointer = Gtree_node*;
    using value_type = std::function<int(int)>;

    value_type    val;
    std::vector<node_pointer>    children;

    Gtree_node() 
    : val(0) {}
    Gtree_node(value_type x) 
    : val(x) {}
    Gtree_node(value_type x, std::initializer_list<node_pointer> children)
    : val(x), children(children) {}
};


#endif