/** 
 *  ==============================================================================
 * 
 *          \file   trie.h
 *
 *        \author   chenghuige  
 *          
 *          \date   2013-08-10 22:53:20.110877
 *  
 *  \Description:
 *  ==============================================================================
 */

#ifndef TRIE_H_
#define TRIE_H_

#include <map>
#include <iostream>
#include "default_value.h"

template<typename T, typename ValueType = int>
class Trie
{
public:

    Trie()
    {
        root_ == new Node();
    }
    struct Node;
    typedef std::map<T, Node*> Map;

    struct Node
    {

        Node()
        {
            follow = NULL;
            DefaultValue(val);
        }
        T key;
        ValueType val;
        Map* follow;
    };

    struct AddFunc
    {

        void operator()(Node* node)
        {
            node->val++;
        }
    };

    struct SetFunc
    {
        ValueType val;

        SetFunc(ValueType _val)
        {
            val = _val;
        }

        void operator()(Node* node)
        {
            node->val = this->val;
        }
    };

    //���һ����¼�� ��������value
    //�����������ӵķ���true
    bool Add(const T* src, int n, ValueType val)
    {
        return add(src, n, SetFunc(val));
    }

    //���һ����¼�� ���Ҷ�value�ۼ�
    //�����������ӵķ���true
    bool Add(const T* src, int n)
    {
        return add(src, n, AddFunc());
    }
    
//    ValueType& operator[](const T* src, int n)
//    {
//        
//    }

private:
    Node* root_;
    
    //������´�����true
    template<typename Func>
    bool add(const T* src, int n, const Func& func)
    {
        std::cout << "Good" << std::endl;

        Node* node = root_;

        for (int i = 0; i < n; i++)
        {
        }

    }
};

#endif  //----end of TRIE_H_
