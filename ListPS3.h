
// COS30008, List, Problem Set 3, 2022

#pragma once

#include "DoublyLinkedList.h"
#include "DoublyLinkedListIterator.h"

#include <stdexcept>

template<typename T>
class List
{
private:
    // auxiliary definition to simplify node usage
    using Node = DoublyLinkedList<T>;

    Node* fRoot;	// the first element in the list
    size_t fCount;	// number of elements in the list

public:
    // auxiliary definition to simplify iterator usage
    using Iterator = DoublyLinkedListIterator<T>;

    ~List()                                                             // destructor - frees all nodes
    {
        while (fRoot != nullptr)
        {
            if (fRoot != &fRoot->getPrevious())                       // more than one element
            {
                Node* lTemp = const_cast<Node*>(&fRoot->getPrevious()); // select last

                lTemp->isolate();                                       // remove from list
                delete lTemp;                                           // free
            }
            else
            {
                delete fRoot;                                           // free last
                break;                                                  // stop loop
            }
        }
    }

    void remove(const T& aElement)	                                // remove first match from list
    {
        Node* lNode = fRoot;                                            // start at first

        while (lNode != nullptr)                                      // Are there still nodes available?
        {
            if (**lNode == aElement)                                  // Have we found the node?
            {
                break;                                                  // stop the search
            }

            if (lNode != &fRoot->getPrevious())                       // not reached last
            {
                lNode = const_cast<Node*>(&lNode->getNext());           // go to next
            }
            else
            {
                lNode = nullptr;                                        // stop search
            }
        }

        // At this point we have either reached the end or found the node.
        if (lNode != nullptr)                                         // We have found the node.
        {
            if (fCount != 1)                                          // not the last element
            {
                if (lNode == fRoot)
                {
                    fRoot = const_cast<Node*>(&fRoot->getNext());       // make next root
                }
            }
            else
            {
                fRoot = nullptr;                                        // list becomes empty
            }

            lNode->isolate();                                           // isolate node
            delete lNode;                                               // release node's memory
            fCount--;                                                   // decrement count
        }
    }

    //////////////////////////////////////////////////////////////////
    //// PS3
    //////////////////////////////////////////////////////////////////

    // P1

    List() : fCount(0), fRoot(nullptr)                          // default constructor
    {}

    bool empty() const	                        // Is list empty?
    {
        return fRoot == nullptr;
    }

    size_t size() const                		// list size
    {
        return fCount;
    }

    void push_front(const T& aElement)	    // adds aElement at front
    {
        Node* lNode = new Node(aElement);

        if (fRoot == nullptr)
        {
            fRoot = lNode;
        }
        else
        {
            *fRoot->push_front(*lNode);
            fRoot = lNode;
        }
        fCount++;
    }

    Iterator begin() const                     // return a forward iterator
    {
        Iterator iter = fRoot;
        return iter.begin();
    }
    Iterator end() const                       // return a forward end iterator
    {
        Iterator iter = fRoot;
        return iter.end();
    }
    Iterator rbegin() const                    // return a backwards iterator
    {
        Iterator iter = fRoot;
        return iter.rbegin();
    }
    Iterator rend() const                      // return a backwards end iterator
    {
        Iterator iter = fRoot;
        return iter.rend();
    }

    // P2

    void push_back(const T& aElement)        // adds aElement at back
    {
        Node* lNode = new Node(aElement);

        if (fRoot == nullptr)
        {
            fRoot = lNode;
        }
        else
        {
            const_cast<Node*>(&fRoot->getPrevious())->push_back(*lNode);
        }
        fCount++;
    }

    // P3

    const T& operator[](size_t aIndex) const	// list indexer
    {
        Node* lNode = fRoot;

        if ((aIndex > fCount) || (aIndex < 0))
        {
            throw std::out_of_range("Index out of bounds.");
        }
        else
        {
            for (int i = 0; lNode != nullptr; i++)
            {
                if (i == aIndex)
                {
                    return lNode->getPayload();
                }
                lNode = const_cast<Node*>(&lNode->getNext());
            }
        }
    }

    // P4

    List(const List& aOtherList) : fCount(0), fRoot(0)         // copy constructor
    {
        for (int i = 0; i < aOtherList.size(); i++)
        {
            push_back(aOtherList[i]);
        }
    }

    List& operator=(const List& aOtherList) // assignment operator
    {
        if (&aOtherList != this)
        {
            this->~List();

            fCount = 0;
            fRoot = nullptr;

            for (int i = 0; i < aOtherList.size(); i++)
            {
                push_front(aOtherList[i]);
            }
        }

        return *this;
    }

    // P5

    List(List&& aOtherList) : fCount(aOtherList.fCount), fRoot(aOtherList.fRoot)  		// move constructor
    {
        aOtherList.fRoot = nullptr;
        aOtherList.fCount = 0;
    }

    // move assignment operator
    List& operator=(List&& aOtherList)       
    {
        if (&aOtherList != this) {
            this->~List();
            fRoot = aOtherList.fRoot;
            fCount = aOtherList.fCount;
            aOtherList.fCount = 0;
            aOtherList.fRoot = nullptr;
        }
        return *this;
    }

    // move push_front
    void push_front(T&& aElement)			
    {
        Node* lNode = new Node(std::move(aElement));

        if (fRoot == nullptr)
        {
            fRoot = lNode;
        }
        else
        {
            *fRoot->push_front(*lNode);
            fRoot = lNode;
        }
        fCount++;
    }

    // move push_back
    void push_back(T&& aElement)				
    {
        Node* lNode = new Node(std::move(aElement));

        if (fRoot == nullptr)
        {
            fRoot = lNode;
        }
        else
        {
            const_cast<Node*>(&fRoot->getPrevious())->push_back(*lNode);
        }
        fCount++;
    }

};

