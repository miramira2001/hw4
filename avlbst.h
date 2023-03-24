#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void spinToLeft( AVLNode<Key,Value>* node);
    void spinToRight( AVLNode<Key,Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  // TODO

  auto node_ptr = this->internalFind(new_item.first);
if (node_ptr != NULL) {
    node_ptr->setValue(new_item.second);
    return;
}

  if(this->root_ == NULL) {
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
      return;
  } // putting in an empty tree
  

  AVLNode<Key, Value>* currentVariable = dynamic_cast<AVLNode<Key, Value>*>(this->root_);
  AVLNode<Key, Value>* NowVariable = NULL;
  for (;;) {
if (new_item.first < currentVariable->getKey()) {
if (currentVariable->getLeft() == NULL) {
currentVariable->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, currentVariable));
NowVariable = currentVariable->getLeft();
break;
}
currentVariable = currentVariable->getLeft();
} else if (currentVariable->getKey() < new_item.first) {
if (currentVariable->getRight() == NULL) {
currentVariable->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, currentVariable));
NowVariable = currentVariable->getRight();
break;
}
currentVariable = currentVariable->getRight();
}
}


  AVLNode<Key, Value>* parent = NowVariable->getParent();
  while(parent != NULL) {
      parent->updateBalance(NowVariable == parent->getLeft() ? -1 : 1);
      if(parent->getBalance() == 0) break;
      else if(parent->getBalance() == 2) {
          if(NowVariable->getBalance() == -1) {
            spinToRight(NowVariable);
          }
          spinToLeft(parent);
          break;
      }
      else if(parent->getBalance() == -2) {
          if(NowVariable->getBalance() == 1) {
            spinToLeft(NowVariable);
          }
          spinToRight(parent);
          break;
      }

      NowVariable = parent;
      parent = NowVariable->getParent();

  }

}

/*
 * Recall: The writeup specifies that if a node has 2 kids you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* currentVariable = dynamic_cast<AVLNode<Key, Value>*>(this->internalFind(key));
if(currentVariable == NULL) return;

    (currentVariable->getLeft() != NULL && currentVariable->getRight() != NULL) ? nodeSwap(currentVariable, (AVLNode<Key, Value>*)this->predecessor(currentVariable)) : void();
 // 2 kids

    AVLNode<Key, Value>* kid = NULL;
    if(currentVariable->getLeft() != NULL) kid = currentVariable->getLeft();
    else if(currentVariable->getRight() != NULL) kid = currentVariable->getRight();

    AVLNode<Key, Value>* parent = currentVariable->getParent();
    if(parent != NULL) {
        if(currentVariable == parent->getLeft()) {
            parent->setLeft(kid);
            parent->updateBalance(1);
        }
        else {
            parent->setRight(kid);
            parent->updateBalance(-1);
        }
    }
    else {
        this->root_ = kid;
    }
    
    if(kid != NULL) {
        kid->setParent(currentVariable->getParent());
    }
    delete currentVariable;

    while(parent != NULL) {
      if(parent->getBalance() == 2) {
          if(parent->getRight()->getBalance() == -1) {
            spinToRight(parent->getRight());
          }
          spinToLeft(parent);
          parent = parent->getParent();
      }
      else if(parent->getBalance() == -2) {
          if(parent->getLeft()->getBalance() == 1) {
            spinToLeft(parent->getLeft());
          }
          spinToRight(parent);
          parent = parent->getParent();
      }
      if(abs(parent->getBalance()) == 1) break;
      currentVariable = parent;
      parent = parent->getParent();
      if(parent != NULL) {
          if(currentVariable == parent->getLeft()) {
              parent->updateBalance(1);
          }
          else {
              parent->updateBalance(-1);
          }
      }

    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t currentVariableB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(currentVariableB);
}



template<class Key, class Value>
void AVLTree<Key, Value>::spinToRight( AVLNode<Key,Value>* node)
{
    AVLNode<Key, Value>* n2 = node;
    AVLNode<Key, Value>* n1 = n2->getLeft();
    AVLNode<Key, Value>* g = n1->getLeft();
    AVLNode<Key, Value>* h = n1->getRight();
    AVLNode<Key, Value>* i = n2->getRight();
    
    AVLNode<Key, Value>* parent = n2->getParent();
switch(parent != NULL) {
    case true:
        switch(n2 == parent->getLeft()) {
            case true:
                parent->setLeft(n1);
                n1->setParent(parent);
                break;
            case false:
                parent->setRight(n1);
                n1->setParent(parent);
                break;
        }
        break;
    case false:
        this->root_ = n1;
        n1->setParent(NULL);
        break;
}

    switch (g != NULL) {
    case true:
        n1->setLeft(g);
        g->setParent(n1);
        break;
    case false:
        n1->setLeft(nullptr);
        break;
}

n1->setRight(n2);
n2->setParent(n1);

switch (h != NULL) {
    case true:
        n2->setLeft(h);
        h->setParent(n2);
        break;
    case false:
        n2->setLeft(nullptr);
        break;
}

switch (i != NULL) {
    case true:
        n2->setRight(i);
        i->setParent(n2);
        break;
    case false:
        n2->setRight(nullptr);
        break;
}

    int formern1 = n1->getBalance();
    int formern2 = n2->getBalance();
    n2->setBalance(formern2 + 1 + std::max(0, (int)formern1) - formern1);
    n1->setBalance(formern1 + 1 + std::max(0, (int)n2->getBalance()));
}

template<class Key, class Value>
void AVLTree<Key, Value>::spinToLeft( AVLNode<Key,Value>* node)
{
    AVLNode<Key, Value>* n1 = node;
    AVLNode<Key, Value>* n2 = n1->getRight();
    AVLNode<Key, Value>* g = n1->getLeft();
    AVLNode<Key, Value>* h = n2->getLeft();
    AVLNode<Key, Value>* i = n2->getRight();

    AVLNode<Key, Value>* parent = n1->getParent();

    switch (parent != NULL) {
    case true: 
        switch (n1 == parent->getLeft()) {
            case true:
                parent->setLeft(n2);
                n2->setParent(parent);
                break;
            case false:
                parent->setRight(n2);
                n2->setParent(parent);
                break;
        }
        break;
    case false:
        this->root_ = n2;
        n2->setParent(NULL);
        break;
}

    n2->setLeft(n1);
    n1->setParent(n2);
    n1->setLeft(g);
    if(g != NULL) g->setParent(n1);
    n1->setRight(h);
    if(h != NULL) h->setParent(n1);
    n2->setRight(i);
    if(i != NULL) i->setParent(n2);

    auto formern1 = n1->getBalance();
    auto formern2 = n2->getBalance();
    n1->setBalance(-1 * std::max(0, static_cast<int>(formern2)) - 1 + formern1);
    n2->setBalance(formern2 + std::min(0, static_cast<int>(n1->getBalance())) - 1);
}

#endif