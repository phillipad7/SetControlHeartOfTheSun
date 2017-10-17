// AVLSet.hpp
//
// ICS 46 Spring 2017
// Project #3: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector).  Instead, you'll need
// to implement your AVL tree using your own dynamically-allocated nodes,
// with pointers connecting them, and with your own balancing algorithms
// used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include "Set.hpp"



template <typename T>
class AVLSet : public Set<T>
{
public:
    // Initializes an AVLSet to be empty.
    AVLSet();

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet();

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s);

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s);


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const T& element);


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
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

    int avlsize = 0;

    Node* head=nullptr;

    Node* insertNode(Node* cn, const T& element);

    Node* llRotation(Node* cn);
    Node* rrRotation(Node* cn);
    Node* lrRotation(Node* cn);
    Node* rlRotation(Node* cn);

    Node* makeBalance(Node* cn);

    Node* copyAll(Node* cn);
    void destroyAll(Node* cn);

    int height(Node* cn);
    int heightDifference(Node* cn);
};


template <typename T>
AVLSet<T>::AVLSet()
:head(nullptr)
{
}


template <typename T>
AVLSet<T>::~AVLSet()
{
    destroyAll(head);
}


template <typename T>
AVLSet<T>::AVLSet(const AVLSet& s)
    :head(nullptr)
{
    copyAll(s.head);
}


template <typename T>
AVLSet<T>::AVLSet(AVLSet&& s)
    :head(nullptr)
{
    std::swap(head, s.head);
}


template <typename T>
AVLSet<T>& AVLSet<T>::operator=(const AVLSet& s)
{
    if (this != &s)
    {
        destroyAll(head);
        copyAll(s.head);
    }
    return *this;
}


template <typename T>
AVLSet<T>& AVLSet<T>::operator=(AVLSet&& s)
{
    std::swap(head, s.head);
    return *this;
}


template <typename T>
bool AVLSet<T>::isImplemented() const
{
    return true;
}


template <typename T>
void AVLSet<T>::add(const T& element)
{
    // std::cout<<" ----- in add ----- "<<std::endl;
    head = insertNode(head, element);
    avlsize++;
}


template <typename T>
bool AVLSet<T>::contains(const T& element) const
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
unsigned int AVLSet<T>::size() const
{
    return avlsize;
}


/* **************************************************************************** */

template <typename T>
void AVLSet<T>::destroyAll(Node* nn)
{
    if(nn != nullptr)
    {
        destroyAll(nn->left);
        destroyAll(nn->right);
        delete nn;
    }
}


template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::copyAll(Node* nn)
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



// 4 rotations

//LL rotate
template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::llRotation(Node* nn)
{
    // std::cout<<" reach rotation: ll"<<std::endl;
    Node* temp;
    temp = nn->left;
    nn->left = temp->right;
    temp->right = nn;
    return temp;
}


//RR rotate
template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::rrRotation(Node* nn)
{
    // std::cout<<" reach rotation: rr"<<std::endl;
    Node* temp;
    temp = nn->right;
    nn->right = temp->left;
    temp->left = nn;
    return temp;
}


//LR rotate
template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::lrRotation(Node* nn)
{
    // std::cout<<" reach rotation: lr"<<std::endl;
    Node* temp;
    temp = nn->left;
    nn->left = rrRotation(temp);
    return llRotation(nn);
}


//RL rotate
template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::rlRotation(Node* nn)
{
    // std::cout<<" reach rotation: rl"<<std::endl;
    Node* temp;
    temp = nn->right;
    nn->right = llRotation(temp);
    return rrRotation(nn);
}


//tree height
template <typename T>
int AVLSet<T>::height(Node* nn)
{
    // std::cout<<" *-* \t in height func -- "<<std::endl;
    if(nn == nullptr)
        return 0;
    else    
        return std::max(height(nn->left), height(nn->right)) + 1;
    
}


// return the difference of heights of a node's left and right
template <typename T>
int AVLSet<T>::heightDifference(Node* nn)
{
    // std::cout<<" --\t in heightDifference \t\n";
    return height(nn->left) - height(nn->right);
}

// balance the tree 
template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::makeBalance(Node* nn)
{
    // std::cout<<"  -- in makeBalance --  "<<std::endl;

    int difference = heightDifference(nn);

    if (difference > 1)
    {
        if(heightDifference(nn->left)>0)
            nn=llRotation(nn);
        else
            nn=lrRotation(nn);
    }
    else if (difference < -1)
    {
        if(heightDifference(nn->right)<0)
            nn=rrRotation(nn);
        else
            nn=rlRotation(nn);
    }
    // else if (difference > 1 && lsubtdiff <0)
    //     nn = lrRotation(nn);
    // else if (difference <-1 && rsubtdiff <0)
    //     nn = rrRotation(nn);
    // else if (difference <-1 && rsubtdiff >0)
    //     nn = rlRotation(nn);

    return nn;
}



template <typename T>
typename AVLSet<T>::Node* AVLSet<T>::insertNode(Node* nn, const T& element)
{
    // std::cout<<"start inserting::********************************************\n";

    if(nn == nullptr)
    {
        nn = new Node();
        nn->key = element;
        return nn;
    }
    else
    {
        if(element < nn->key)
        {
            // std::cout<<"element is less than key"<<std::endl;
            nn->left = insertNode(nn->left, element);
            nn = makeBalance(nn);
        }
        if(element > nn->key)
        {
            // std::cout<<"element is gret than key"<<std::endl;
            nn->right = insertNode(nn->right, element);
            nn = makeBalance(nn);
        }
        // std::cout<<"\t IN ELSE nn->key is: \t"<<nn->key<<std::endl;
        return nn;
    }
}



#endif // AVLSET_HPP

