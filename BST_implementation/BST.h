#ifndef BST_H
#define BST_H
#include <functional>
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>

template <typename T, typename Compare=std::less<T>>
class BST
{
public:
    using value_type = T;
    using reference_type = value_type&;
    using const_reference = const value_type&;
    using key_compare = Compare;
    using size_type = std::size_t;

private:
    struct BST_node
    {
        using node_type = BST_node;
        using node_pointer = node_type *;
        value_type data;
        node_pointer left;
        node_pointer right;
        BST_node(value_type t = value_type(), node_pointer l = nullptr, node_pointer r = nullptr) 
        : data{t}, left{l}, right{r} {}
    };

public:
    using node_type = BST_node;
    using node_pointer = node_type *;

private:
    node_type *root;
    key_compare value_compare;
    size_type node_count;

public:
    BST();
    BST(const BST<T>& rhs);
    BST(BST<T>&& rhs);
    ~BST();

    BST& operator=(const BST<T>& rhs);
    BST& operator=(BST<T>&& rhs);

    bool find(const_reference target) const;

    bool search_iterative(const_reference target) const;

    bool empty() const noexcept;

    int getHeight() const;

    void clear();

    int size() const noexcept;

    bool insert(const_reference data);

    void erase(const_reference data);

    void swap(BST &other) noexcept;

    bool contains(const_reference key) const;

    void preorder();

    void inorder();

    void postorder();

    std::string serialize() const;

    void deserialize(const std::string &data);


private: // helpers
    bool find_help(node_pointer root, const_reference target) const;

    node_pointer getMin_help(node_pointer root) const;

    node_pointer getMax_help(node_pointer root) const;

    node_pointer getMin_iterative() const;

    node_pointer getMax_iterative() const;

    node_pointer clone_help(node_pointer root) const;

    void destruct_tree(node_pointer root);

    node_pointer getSuccessor(node_pointer root, node_pointer target) const;

    node_pointer getPredecessor(node_pointer root, node_pointer target) const;

    int getHeight_help(node_pointer root) const;

    node_pointer insert_help(node_pointer root, const_reference data);

    node_pointer erase_help(node_pointer root, const_reference data);

    void preorder_help(node_pointer root);

    void inorder_help(node_pointer root);

    void postorder_help(node_pointer root);
    
    void preorder_serialize(node_pointer root, std::string &result) const;  
};

#include "BST.hpp"

#endif // BST_H