#ifndef GTREE_H
#define GTREE_H

#include <functional>
#include <iostream>
#include <vector>
#include <queue>
#include "Gtree_node.h"


class Gtree
{
    public:
        using value_type = Gtree_node::value_type;
        using size_type = size_t;
        using node_type = Gtree_node;
        using node_pointer = node_type*;

    protected:
        node_type *root;

    private:
        int helper_execute (node_pointer root, int num)
        {
            if(!root)
                return num;
            std::queue<node_pointer> queue;
            queue.push(root);
            while(!queue.empty())
            {
                node_pointer temp = queue.front();
                queue.pop();
                num = temp->val(num);  // exec function
                for(const auto it : temp->children)
                {
                    queue.push(it);
                }
            }
            return num;
        }

        node_pointer create_node(value_type val)
        {
            return new Gtree_node(val);
        }
        void DestroyRecursive(node_pointer node)
        {
            if (node)
            {
                for(auto it : node->children)
                {
                    DestroyRecursive(it);
                }
                delete node;
            }
        }
    public:

        Gtree() = default;

        ~Gtree()
        {
            DestroyRecursive(root);
        }
        node_pointer get_root()
        {
            return root;
        }
        void set_root(node_pointer ptr)
        {
            root = ptr;
        }
        int execute(int num)
        {
            if(!root)
                return num;
            return helper_execute(root, num);
        }
        void insert_children(node_pointer target, std::initializer_list<value_type> children)
        {
            for(auto &it : children)
            {
                target->children.push_back(create_node(it));
            }
        }
        void insert_child(node_pointer target, value_type child)
        {
            target->children.push_back(create_node(child));
        }
};




#endif