#ifndef BST_HPP
#define BST_HPP

#include "BST.h"

#include <functional>
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

template <typename T, typename Compare>
BST<T, Compare>::BST() :root{nullptr}, value_compare{}, node_count{0} {}

template <typename T, typename Compare>
BST<T, Compare>::BST(const BST<T>& rhs) : root{nullptr}, node_count{0}
{
    node_pointer temp = clone_help(rhs.root); 
    this->root = temp;
    this->node_count = rhs.node_count;
}

template <typename T, typename Compare>
BST<T, Compare>::BST(BST<T>&& rhs) : root{nullptr}, node_count{0}
{
    this->root = rhs.root;
    this->node_count = rhs.node_count;
    rhs.root = nullptr;
    rhs.node_count = 0;
}

template <typename T, typename Compare>
BST<T, Compare>::~BST()
{
    destruct_tree(root);
    this->root = nullptr;
    this->node_count = 0;
}

template <typename T, typename Compare>
BST<T, Compare>& BST<T, Compare>::operator=(const BST<T>& rhs)
{
    if(this != &rhs) 
    {
        BST tmp(rhs);
        this->swap(tmp);
    }
    return *this;
}

template <typename T, typename Compare>
BST<T, Compare>& BST<T, Compare>::operator=(BST<T>&& rhs)
{
    if(this != &rhs)
    {
        BST tmp(std::move(rhs));
        this->swap(tmp);
    }
    return *this;
}

template <typename T, typename Compare>
bool BST<T, Compare>::find(const_reference target) const
{
    return find_help(root, target);
}

template <typename T, typename Compare>
bool BST<T, Compare>::search_iterative(const_reference target) const
{
    if(!root)
        return false;
    node_pointer temp = root;
    while(temp != nullptr)
    {
        if(value_compare(target,temp->data))
        {
            temp = temp->left;
        }
        else if(value_compare(temp->data, target))
        {
            temp = temp->right;
        }
        else
            return true;
    }
    return false;
}

template <typename T, typename Compare>
bool BST<T, Compare>::empty() const noexcept
{
    return !(this->root);
}

template <typename T, typename Compare>
int BST<T, Compare>::getHeight() const
{
    return getHeight_help(root);
}

template <typename T, typename Compare>
void BST<T, Compare>::clear()
{
    destruct_tree(root);
    root = nullptr;
    node_count = 0;
}

template <typename T, typename Compare>
int BST<T, Compare>::size() const noexcept
{
    return this->node_count;
}

template <typename T, typename Compare>
bool BST<T, Compare>::insert(const_reference data)
{
    try
    {
        root = insert_help(root, data);
        ++node_count;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

template <typename T, typename Compare>
void BST<T, Compare>::erase(const_reference data)
{
    root = erase_help(root, data);
}

template <typename T, typename Compare>
void BST<T, Compare>::swap(BST &other) noexcept
{
    using std::swap;
    
    swap(this->root, other.root);
    swap(this->node_count, other.node_count);
}

template <typename T, typename Compare>
bool BST<T, Compare>::contains(const_reference key) const
{
    return find_help(root, key);
}

template <typename T, typename Compare>
void BST<T, Compare>::preorder()
{
    preorder_help(root);
}

template <typename T, typename Compare>
void BST<T, Compare>::inorder()
{
    inorder_help(root);
}

template <typename T, typename Compare>
void BST<T, Compare>::postorder()
{
    postorder_help(root);
}

template <typename T, typename Compare>
std::string BST<T, Compare>::serialize() const
{
    std::string result;
    preorder_serialize(root, result);
    return result;
}

template <typename T, typename Compare>
void BST<T, Compare>::deserialize(const std::string &data) 
{
    for (size_t i = 0; i < data.size(); i += sizeof(int)) 
    {
        value_type val;
        memcpy(&val, &data[i], sizeof(value_type));
        this->insert(val);
    }
}

// helpers
template <typename T, typename Compare>
bool BST<T, Compare>::find_help(node_pointer root, const_reference target) const
{
    if(!root)
        return false;
    if(value_compare(root->data,target))
    {
        return find_help(root->right, target);
    }
    else if(value_compare(target, root->data))
    {
        return find_help(root->left, target);
    }
    else 
        return true;
}

template <typename T, typename Compare>
typename BST<T, Compare>::node_pointer BST<T, Compare>::getMin_help(node_pointer root) const
{
    if(!root)
        return nullptr;
    if(root->left == nullptr)
    {
        return root;
    }
    return getMin_help(root->left);
}

template <typename T, typename Compare>
typename BST<T, Compare>::node_pointer BST<T, Compare>::getMax_help(node_pointer root) const
{
    if(!root)
        return nullptr;
    if(root->right == nullptr)
    {
        return root;
    }
    return getMax_help(root->right);
}

template <typename T, typename Compare>
typename BST<T, Compare>::node_pointer BST<T, Compare>::getMin_iterative() const 
{
    if(!root)
        return nullptr;
    node_pointer temp = root;
    while(temp->left)
    {
        temp = temp->left;
    }
    return temp;
}

template <typename T, typename Compare>
typename BST<T, Compare>::node_pointer BST<T, Compare>::getMax_iterative() const
{
    if(!root)
        return nullptr;
    node_pointer temp = root;
    while(temp->right)
    {
        temp = temp->right;
    }
    return temp;
}

template <typename T, typename Compare>
typename BST<T, Compare>::node_pointer BST<T, Compare>::clone_help(node_pointer root) const
{
    if (!root)
        return nullptr;
    return new node_type(root->data, clone_help(root->left), clone_help(root->right));

}

template <typename T, typename Compare>
void BST<T, Compare>::destruct_tree(node_pointer root)
{
    if(!root)
        return;
    destruct_tree(root->left);
    destruct_tree(root->right);
    delete(root);
    root = nullptr;
}

template <typename T, typename Compare>
typename BST<T, Compare>::node_pointer BST<T, Compare>::getSuccessor(node_pointer root, node_pointer target) const
{
    if(!root)
        return nullptr;
    if(target->right != nullptr)
        return getMin_help(target->right);

    node_pointer succ = nullptr;
    node_pointer curr = root;
    while(curr != nullptr)
    {
        if(value_compare(target->data, curr->data))
        {
            succ = curr;
            curr = curr->left;
        }
        else if(value_compare(curr->data, target->data))
        {
            curr = curr->right;
        }
        else
        {
            break;
        }
    }
    return succ;
}

template <typename T, typename Compare>
typename BST<T, Compare>::node_pointer BST<T, Compare>::getPredecessor(node_pointer root, node_pointer target) const
{
    if(!root)
        return nullptr;
    if(target->left != nullptr)
        return getMax_help(target->left);

    node_pointer pred = nullptr;
    node_pointer curr = root;
    while(curr != nullptr)
    {
        if(value_compare(target->data, curr->data))
        {
            curr = curr->left;
        }
        else if(value_compare(curr->data, target->data))
        {
            pred = curr;
            curr = curr->right;
        }
        else
        {
            break;
        }
    }
    return pred;
}

template <typename T, typename Compare>
int BST<T, Compare>::getHeight_help(node_pointer root) const
{
    if(!root)
        return -1;
    return 1 + std::max(getHeight_help(root->left), getHeight_help(root->right));
}

template <typename T, typename Compare>
typename BST<T, Compare>::node_pointer BST<T, Compare>::insert_help(node_pointer root, const_reference data)
{
    if(!root)
        return new node_type(data);
    if(value_compare(root->data, data))
    {
        root->right = insert_help(root->right, data);
    }
    else if (value_compare(data, root->data))
    {
        root->left = insert_help(root->left, data);
    }
    else
    {
        throw std::runtime_error("Duplicate value"); 
    }
    return root;
}

template <typename T, typename Compare>
typename BST<T, Compare>::node_pointer BST<T, Compare>::erase_help(node_pointer root, const_reference data)
{
    if(!root)
        return nullptr;
    
    if(value_compare(root->data, data))
    {
        root->right = erase_help(root->right, data);
    }
    else if(value_compare(data, root->data))
    {
        root->left = erase_help(root->left, data);
    }
    else
    {
        if(!root->left && !root->right)
        {
            delete root;
            --node_count;
            return nullptr;
        }
        else if(root->left == nullptr)
        {
            node_pointer temp = root->right;
            --node_count;
            delete root;
            return temp;
        }
        else if(root->right == nullptr)
        {
            node_pointer temp = root->left;
            delete root;
            --node_count;
            return temp;
        }
        node_pointer successor = getMin_help(root->right);
        root->data = successor->data;
        root->right = erase_help(root->right, successor->data);
    }
    return root;
}

template <typename T, typename Compare>
void BST<T, Compare>::preorder_help(node_pointer root)
{
    if(!root)
        return;
    std::cout << root->data << " ";
    preorder_help(root->left);
    preorder_help(root->right);
}

template <typename T, typename Compare>
void BST<T, Compare>::inorder_help(node_pointer root)
{
    if(!root)
        return;
    inorder_help(root->left);
    std::cout << root->data << " ";
    inorder_help(root->right);

}

template <typename T, typename Compare>
void BST<T, Compare>::postorder_help(node_pointer root)
{
    if(!root)
        return;
    postorder_help(root->left);
    postorder_help(root->right);
    std::cout << root->data << " ";

}

template <typename T, typename Compare>
void BST<T, Compare>::preorder_serialize(node_pointer root, std::string &result) const  
{
    if(!root)
        return ;
    char *ptr = (char *)&root->data;
    for(int i = 0; i < sizeof(value_type); ++i)
    {
        result.push_back(*(ptr + i));
    }
    preorder_serialize(root->left, result);
    preorder_serialize(root->right, result);

}   


template<class T, class Compare>
void swap(BST<T, Compare>& lhs, BST<T, Compare>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename T, typename Compare = std::less<T>>
void BST_in_file(const BST<T, Compare> &tree, const std::string &filePath)
{
    std::string serialized = tree.serialize();
    std::ofstream file(filePath);
    if (file.is_open()) 
    {
        file << serialized;
        file.close();
    } 
    else {
        throw std::runtime_error("Save operation Error");
    }
}

template <typename T, typename Compare = std::less<T>>
BST<T, Compare> BST_out_file(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        throw std::runtime_error("File could not be opened");
    }

    std::string serialized((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
    file.close();

    BST<T, Compare> tree;
    tree.deserialize(serialized);

    return tree;
}

#endif