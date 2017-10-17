// HashSet.hpp
//
// ICS 46 Spring 2017
// Project #3: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::vector, std::list, or std::array).  Instead, you'll need
// to use a dynamically-allocated array and your own linked list
// implementation; the linked list doesn't have to be its own class,
// though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename T>
class HashSet : public Set<T>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction 
    typedef std::function<unsigned int(const T&)> HashFunction;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet();

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s);

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s);


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const T& element);


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const T& element) const;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const;


private:
    HashFunction hashFunction;      // return an int
    
    struct Node
    {
        T key;
        Node* next = nullptr;
    };

    Node** table;   // an array of Node pointers

    int tablecapacity;// = DEFAULT_CAPACITY;
    int tablesize = 0;
    int num =0;
    void destroyAll(Node* nn);
    void initTable(Node** table);
    
    Node** resizedTable(Node** old, const T& element);
    Node** addNode(Node** table, const T& element);
    // Node** copyAll(Node** tb, int capacity);

};



template <typename T>
HashSet<T>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}, tablecapacity{DEFAULT_CAPACITY}, tablesize{0}
{
    // initialize table as an array of Node ptrs
    table = new Node* [tablecapacity];
    initTable(table);
}


template <typename T>
HashSet<T>::~HashSet()
{
    for(int i=0; i<tablecapacity; i++)
        destroyAll(table[i]);
    delete table;
}



template <typename T>
HashSet<T>::HashSet(const HashSet& s)
    : hashFunction{nullptr}
{
    // need to copy capacity before initialize table !!!!!
    tablecapacity = s.tablecapacity;
    hashFunction = s.hashFunction;
    // std::swap(hashFunction, s.hashFunction);
    // tablesize = s.tablesize;

    table = new Node* [s.tablecapacity];     // create new table with the size of s
    initTable(table);
    
    for(int i=0; i<s.tablecapacity; i++)
    {
        Node* nn = table[i];
        for(Node* temp = s.table[i]; temp != nullptr; temp=temp->next)
        {
            nn= new Node();
            nn->key = temp->key;
            nn->next = temp-> next;
            nn = temp;
        }
    }
}


template <typename T>
HashSet<T>::HashSet(HashSet&& s)
    : hashFunction{nullptr}
{
    tablecapacity = s.tablecapacity;
    hashFunction = s.hashFunction;
    // tablesize = s.tablesize;

    table = new Node* [s.tablecapacity];
    initTable(table); 
    
    for(int i=0; i<s.tablecapacity; i++)
    {
        Node* nn = table[i];
        for(Node* temp = s.table[i]; temp != nullptr; temp=temp->next)
        {
            nn = new Node();
            nn->key = temp->key;        // table[i]->key = s.table[i]->key
            nn->next = temp-> next;
            nn = temp;
        }
    }
}


template <typename T>
HashSet<T>& HashSet<T>::operator=(const HashSet& s)
{
    // destroy old table
    for(int i=0; i<tablecapacity; i++)
    {
        destroyAll(table[i]);
    }
    delete table;

    // create new table
    hashFunction = s.hashFunction;
    tablecapacity = s.tablecapacity;

    table = new Node* [s.tablecapacity];
    initTable(table);

    for(int i=0; i<s.tablecapacity; i++)
    {
        Node* nn = table[i];
        for(Node* temp=s.table[i]; temp != nullptr; temp=temp->next)
        {
            nn= new Node();
            nn->key = temp->key;
            nn->next = temp-> next;
            nn = temp;
        }
    }

    return *this;
}


template <typename T>
HashSet<T>& HashSet<T>::operator=(HashSet&& s)
{
    std::swap(table, s.table);
    return *this;
}


template <typename T>
bool HashSet<T>::isImplemented() const
{
    return true;
}


template <typename T>
void HashSet<T>::add(const T& element)
{
    double rate = static_cast<double>(tablesize) / static_cast<double>(tablecapacity);

    if(rate < 0.8)
    {   
        table = addNode(table, element);
        tablesize++;
    }
    else
    {
        table = resizedTable(table, element);
        table=addNode(table,element);
        tablesize++;
    }
}


template <typename T>
bool HashSet<T>::contains(const T& element) const
{
    unsigned int index = static_cast<unsigned int>(hashFunction(element)) % tablecapacity;

    Node* nn = table[index];
    while(nn != nullptr)
    {
        if(nn->key == element)
            return true;
        nn = nn->next;
    }
    return false;
}


template <typename T>
unsigned int HashSet<T>::size() const
{
    return tablesize;
}



template <typename T>
typename HashSet<T>::Node** HashSet<T>::addNode(Node** table, const T& element)
{
    unsigned int index = static_cast<unsigned int>(hashFunction(element)) % tablecapacity;

    Node* nnd = new Node();
    nnd->key = element;
    nnd->next = table[index];
    table[index] = nnd;

    return table;
}


template <typename T>
typename HashSet<T>::Node** HashSet<T>::resizedTable(Node** table, const T& element)
{
    int halfcap = tablecapacity;
    tablecapacity *= 2;

    Node** cptable;
    cptable = new Node* [tablecapacity];
    initTable(cptable);

    for(int i=0; i< halfcap; i++)
        for(Node* n = table[i]; n != nullptr; n=n->next)
            cptable = addNode(cptable, n->key);

    for(int i=0; i<halfcap; i++)
        destroyAll(table[i]);
    delete table;
    
    return cptable;
}



template <typename T>
void HashSet<T>::initTable(Node** table)
{
    for(int i=0; i<tablecapacity; i++)
        table[i]=nullptr; 
}



template <typename T>
void HashSet<T>::destroyAll(Node* nn)
{
    if(nn != nullptr)
    {
        destroyAll(nn->next);
        delete nn;
    }
    // else
    //     delete nn;
}



#endif // HASHSET_HPP