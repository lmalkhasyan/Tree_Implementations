#include "Gtree.h"
#include <iostream>

int main()
{
    Gtree test_tree;
    test_tree.set_root(new Gtree_node([](int x){return x*x;}));                                    
    auto ptr = test_tree.get_root(); 

    test_tree.insert_children(ptr, {[](int x){return x * 2;}, [](int x){return x * 3;} });
    test_tree.insert_child(ptr->children[0], [](int x){return x + 10;});
    test_tree.insert_child(ptr->children[1], [](int x){return x + 10;});

    int result = test_tree.execute(100);
    std::cout << result << std::endl;
}