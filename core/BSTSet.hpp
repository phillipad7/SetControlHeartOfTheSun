// BSTSet.hpp
//
// ICS 46 Spring 2017
// Project #3: Set the Controls for the Heart of the Sun
//
// A BSTSet is an implementation of a Set that is a binary search tree,
// albeit one that makes no attempt to remain balanced.  (You explicitly
// should not do any balancing here; do that in AVLSet instead, if you
// choose to work on that one.)
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector).  Instead, you'll need
// to implement your binary search tree using your own dynamically-allocated
// nodes, with pointers connecting them.

#ifndef BSTSET_HPP
#define BSTSET_HPP

#include "Set.hpp"



template <typename T>
class BSTSet : public Set<T>
{
public:
    // Initializes a BSTSet to be empty.
    BSTSet();

    // Cleans up the BSTSet so that it leaks no memory.
    virtual ~BSTSet();

    // Initializes a new BSTSet to be a copy of an existing one.
    BSTSet(const BSTSet& s);

    // Initializes a new BSTSet whose contents are moved from an
    // expiring one.
    BSTSet(BSTSet&& s);

    // Assigns an existing BSTSet into another.
    BSTSet& operator=(const BSTSet& s);

    // Assigns an expiring BSTSet into another.
    BSTSet& operator=(BSTSet&& s);


    // isImplemented() should be modified to return true if you've
    // decided to implement a BSTSet, false otherwise.
    virtual bool isImplemented() const;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function runs in O(n) time when there
    // are n elements in the binary search tree, and is sometimes as fast as
    // O(log n) (when the tree is relatively balanced).
    virtual void add(const T& element);


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in O(n) time when there
    // are n elements in the binary search tree, and is sometimes as fast as
    // O(log n) (when the tree is relatively balanced).
    virtual bool contains(const T& element) const;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const;


private:
    struct Node
    {
        T key;
        Node* left = nullptr;
        Node* right = nullptr;
    };

    int bstsize = 0;
    Node* head=nullptr;

    Node* copyAll(Node* cn);
    Node* insertNode(Node* cn, const T& element);
    void destroyAll(Node* cn);
};


template <typename T>
BSTSet<T>::BSTSet()
    :head{nullptr}
{
}


template <typename T>
BSTSet<T>::~BSTSet()
{
    destroyAll(head);
}


template <typename T>
BSTSet<T>::BSTSet(const BSTSet& s)
    :head{nullptr}
{
    copyAll(s.head);
}


template <typename T>
BSTSet<T>::BSTSet(BSTSet&& s)
    :head{nullptr}
{
    std::swap(head, s.head);
}


template <typename T>
BSTSet<T>& BSTSet<T>::operator=(const BSTSet& s)
{
    if (this != &s)
    {
        destroyAll(head);
        copyAll(s.head);
    }
    return *this;
}


template <typename T>
BSTSet<T>& BSTSet<T>::operator=(BSTSet&& s)
{
    std::swap(head, s.head);
    return *this;
}


template <typename T>
bool BSTSet<T>::isImplemented() const
{
    return true;
}


template <typename T>
void BSTSet<T>::add(const T& element)
{
    head = insertNode(head, element);
    bstsize ++;
}


template <typename T>
bool BSTSet<T>::contains(const T& element) const
{
    Node* curr = head;

    while(curr != nullptr)
    {
        if(element == curr->key)
            return true;

        else if(element < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    return false;
}


template <typename T>
unsigned int BSTSet<T>::size() const
{
    return bstsize;
}


template <typename T>
typename BSTSet<T>::Node* BSTSet<T>::copyAll(Node* nn)
{
    Node* curr = new Node();

    if(nn == nullptr)
        return nullptr;
    else
    {
        curr->key = nn->key;
        curr->left = copyAll(nn->left);
        curr->right = copyAll(nn->right);
    }
    return curr;
}


template <typename T>
void BSTSet<T>::destroyAll(Node* nn)
{
    if(nn != nullptr)
    {
        destroyAll(nn->left);
        destroyAll(nn->right);
        delete nn;
    }
}



template <typename T>
typename BSTSet<T>::Node* BSTSet<T>::insertNode(Node* nn, const T& element)
{
    if(nn == nullptr)
    {
        nn = new Node();
        nn->key = element;
        return nn;
    }
    else
    {
        if(element == nn->key)
            return nn;
        if(element < nn->key)
            nn->left = insertNode(nn->left, element);
        if(element > nn->key)
            nn->right = insertNode(nn->right, element);
        return nn;
    }
}



#endif // BSTSET_HPP

