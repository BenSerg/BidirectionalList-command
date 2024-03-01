#ifndef S4_BINARYSEARCHTREE_HPP
#define S4_BINARYSEARCHTREE_HPP

#include <cassert>
#include <utility>
#include "TreeNode.hpp"
#include "Dequeue.h"
#include "Stack.h"
#include "Queue.h"

namespace bendryshev
{
  template< typename Key, typename Value, typename Compare >
  class BinarySearchTree
  {
  public:
    using data_t = std::pair< Key, Value >;
    using Node = detail::TreeNode< data_t, Compare >;

    BinarySearchTree();
    BinarySearchTree(const BinarySearchTree< Key, Value, Compare >&);
    BinarySearchTree(BinarySearchTree< Key, Value, Compare >&&) noexcept;
    BinarySearchTree& operator=(const BinarySearchTree< Key, Value, Compare >&);
    BinarySearchTree& operator=(BinarySearchTree< Key, Value, Compare >&&) noexcept;
    ~BinarySearchTree();
    void insert(const data_t&);
    void remove(const Key&);
    void clear();
    void swap(const BinarySearchTree< Key, Value, Compare >&) noexcept;
    bool isEmpty() const noexcept;

    template< typename F >
    F traverse_lnr(F f);
    template< typename F >
    F traverse_rnl(F f);
    template< typename F >
    F traverse_breadth(F f);

    struct ConstIterator
    {
      friend class BinarySearchTree< Key, Value, Compare >;

      using this_t = bendryshev::BinarySearchTree< Key, Value, Compare >::ConstIterator;
      ConstIterator() = default;
      ConstIterator(const this_t&) = default;
      ~ConstIterator() = default;
      this_t& operator=(const this_t&) = default;
      this_t& operator++();
      this_t operator++(int);
      const data_t& operator*() const;
      const data_t* operator->() const;
      bool operator!=(const this_t&) const;
      bool operator==(const this_t&) const;
    private:
      bendryshev::Dequeue< Node* > node_dequeue;
      bool hasNext() const;
      void pushAllLeftElements(Node*);
      explicit ConstIterator(Node*);
    };

    using CIterator = bendryshev::BinarySearchTree< Key, Value, Compare >::ConstIterator;
    CIterator cbegin() const noexcept;
    CIterator cend() const noexcept;

    struct Iterator
    {
      friend class BinarySearchTree< Key, Value, Compare >;

      using this_t = bendryshev::BinarySearchTree< Key, Value, Compare >::Iterator;
      Iterator() = default;
      Iterator(const this_t&) = default;
      explicit Iterator(ConstIterator);
      ~Iterator() = default;
      this_t& operator=(const this_t&) = default;
      this_t& operator++();
      this_t operator++(int);
      data_t& operator*();
      data_t* operator->();
      bool operator!=(const this_t&) const;
      bool operator==(const this_t&) const;
    private:
      ConstIterator cit_;
    };

    Iterator begin() noexcept;
    Iterator end() noexcept;
    CIterator cfind(const Key&);
    Iterator find(const Key&);
    CIterator cfindUpperBound(const Key&);
    CIterator cfindLowerBound(const Key&);
    Iterator findUpperBound(const Key&);
    Iterator findLowerBound(const Key&);
    void insert(Iterator, Iterator);
    void remove(Iterator, Iterator);
    void insert(std::initializer_list< data_t >);
    Iterator remove(Iterator);

  private:
    Node* root_;
    void clear(Node**);
    void setBalance(Node**);
    void turnRight(Node**);
    void turnLeft(Node**);
    void insert(Node**, const data_t&);
    void removeHelper(Node**, Node**);
    void remove(Node**, const Key&);
    int getHeight(Node*);
    Node* balance(Node*);
    Node* find(Node*, const Key&);
    bool isEqual(const Key&, const Key&);
    template< typename F >
    F traverseAscending(Node*, F f);
    template< typename F >
    F traverseDescending(Node*, F f);
    template< typename F >
    F traverseBreadth(Node*, F f);
  };

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >::Iterator::Iterator(BinarySearchTree::ConstIterator cit):
    cit_(cit)
  {}

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Iterator::this_t& BinarySearchTree< Key, Value, Compare >::Iterator::operator++()
  {
    ++cit_;
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Iterator::this_t BinarySearchTree< Key, Value, Compare >::Iterator::operator++(int)
  {
    return Iterator(cit_++);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::data_t& BinarySearchTree< Key, Value, Compare >::Iterator::operator*()
  {
    return const_cast< data_t& >(*cit_);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::data_t* BinarySearchTree< Key, Value, Compare >::Iterator::operator->()
  {
    return const_cast< data_t* >(std::addressof(*cit_));
  }

  template< typename Key, typename Value, typename Compare >
  bool BinarySearchTree< Key, Value, Compare >::Iterator::operator==(const BinarySearchTree::Iterator::this_t& other) const
  {
    return cit_ == other.cit_;
  }

  template< typename Key, typename Value, typename Compare >
  bool BinarySearchTree< Key, Value, Compare >::Iterator::operator!=(const BinarySearchTree::Iterator::this_t& other) const
  {
    return !(*this == other);
  }

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >::ConstIterator::ConstIterator(Node* tree)
  {
    pushAllLeftElements(tree);
  }

  template< typename Key, typename Value, typename Compare >
  bool BinarySearchTree< Key, Value, Compare >::ConstIterator::hasNext() const
  {
    return !node_dequeue.isEmpty();
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::ConstIterator::pushAllLeftElements(Node* tree)
  {
    while (tree)
    {
      node_dequeue.push(tree);
      tree = tree->left_;
    }
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::ConstIterator& BinarySearchTree< Key, Value, Compare >::ConstIterator::operator++()
  {
    assert(hasNext());
    Node* tmpNode = node_dequeue.getTop();
    node_dequeue.drop();
    pushAllLeftElements(tmpNode->right_);
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::ConstIterator BinarySearchTree< Key, Value, Compare >::ConstIterator::operator++(int)
  {
    assert(hasNext());
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename Key, typename Value, typename Compare >
  const typename BinarySearchTree< Key, Value, Compare >::data_t& BinarySearchTree< Key, Value, Compare >::ConstIterator::operator*() const
  {
    assert(hasNext());
    Node* tmpNode = node_dequeue.getTop();
    return (tmpNode->data_);
  }

  template< typename Key, typename Value, typename Compare >
  const typename BinarySearchTree< Key, Value, Compare >::data_t* BinarySearchTree< Key, Value, Compare >::ConstIterator::operator->() const
  {
    assert(hasNext());
    Node* tmpNode = node_dequeue.getTop();
    return std::addressof(tmpNode->data_);
  }

  template< typename Key, typename Value, typename Compare >
  bool bendryshev::BinarySearchTree< Key, Value, Compare >::ConstIterator::operator==(const this_t& other) const
  {
    if (node_dequeue.isEmpty() && other.node_dequeue.isEmpty())
    {
      return true;
    }
    if ((node_dequeue.isEmpty() && !other.node_dequeue.isEmpty()) || (!node_dequeue.isEmpty() && other.node_dequeue.isEmpty()))
    {
      return false;
    }
    Node* tmpNode = node_dequeue.getTop();
    Node* other_tmpNode = other.node_dequeue.getTop();
    return (tmpNode == other_tmpNode);
  }

  template< typename Key, typename Value, typename Compare >
  bool bendryshev::BinarySearchTree< Key, Value, Compare >::ConstIterator::operator!=(const this_t& other) const
  {
    return !(*this == other);
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::setBalance(Node** node)
  {
    if (*node)
    {
      (*node)->balanceFactor_ = getHeight((*node)->right_) - getHeight((*node)->left_);
    }
  }

  template< typename Key, typename Value, typename Compare >
  int BinarySearchTree< Key, Value, Compare >::getHeight(Node* node)
  {
    if (!node)
    {
      return 0;
    }
    int hLeft = getHeight(node->left_);
    int hRight = getHeight(node->right_);
    if (hLeft > hRight)
    {
      return hLeft + 1;
    }
    else
    {
      return hRight + 1;
    }
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::turnRight(Node** node)
  {
    Node* leftSubtree, * leftSubtreeRightSubtree;
    leftSubtree = (*node)->left_;
    leftSubtreeRightSubtree = leftSubtree->right_;
    leftSubtree->right_ = *node;
    (*node)->left_ = leftSubtreeRightSubtree;
    (*node) = leftSubtree;
    setBalance(std::addressof((*node)->right_));
    setBalance(node);
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::turnLeft(Node** node)
  {
    Node* rightSubtree, * rightSubtreeLeftSubtree;
    rightSubtree = (*node)->right_;
    rightSubtreeLeftSubtree = rightSubtree->left_;
    rightSubtree->left_ = *node;
    (*node)->right_ = rightSubtreeLeftSubtree;
    *node = rightSubtree;
    setBalance(std::addressof((*node)->left_));
    setBalance(node);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Node* BinarySearchTree< Key, Value, Compare >::balance(Node* node)
  {
    if (getHeight(node->right_) - getHeight(node->left_) > 1)
    {
      if (getHeight(node->right_->right_) < getHeight(node->right_->left_))
      {
        turnRight(std::addressof(node->right_));
      }
      turnLeft(std::addressof(node));
    }
    else if (getHeight(node->left_) - getHeight(node->right_) > 1)
    {
      if (getHeight(node->left_->left_) < getHeight(node->left_->right_))
      {
        turnLeft(std::addressof(node->left_));
      }
      turnRight(std::addressof(node));
    }
    return node;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Node* BinarySearchTree< Key, Value, Compare >::find(Node* node, const Key& data)
  {
    if (!node)
    {
      return nullptr;
    }
    if (node->data_.first == data)
    {
      return node;
    }
    if (Compare()(data, node->data_.first))
    {
      return node->left_ ? find(node->left_, data) : nullptr;
    }
    else
    {
      return node->right_ ? find(node->right_, data) : nullptr;
    }
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::insert(Node** node, const data_t& data)
  {
    if (!(*node))
    {
      *node = new Node { data, nullptr, nullptr, 0 };
    }
    else
    {
      bool equal = isEqual((*node)->data_.first, data.first);
      if (Compare()((*node)->data_.first, data.first))
      {
        insert(std::addressof((*node)->right_), data);
        *node = balance(*node);
      }
      else if (Compare()(data.first, (*node)->data_.first) || equal)
      {
        insert(std::addressof((*node)->left_), data);
        *node = balance(*node);
      }
      setBalance(node);
    }
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::remove(Node** node, const Key& key)
  {
    bool equal = isEqual((*node)->data_.first, key);
    if (!(*node))
    {
      return;
    }
    if (Compare()(key, (*node)->data_.first))
    {
      remove(std::addressof((*node)->left_), key);
    }
    else if (!Compare()(key, (*node)->data_.first) && !equal)
    {
      remove(std::addressof((*node)->right_), key);
    }
    else
    {
      Node* pDel = *node;
      if (!(*node)->right_)
      {
        *node = (*node)->left_;
      }
      else if (!(*node)->left_)
      {
        *node = (*node)->right_;
      }
      else
      {
        removeHelper(std::addressof(pDel), std::addressof((*node)->left_));
      }
      delete pDel;
    }
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::removeHelper(Node** p, Node** q)
  {
    if ((*q)->right_)
    {
      removeHelper(p, std::addressof((*q)->right_));
    }
    else
    {
      (*p)->data_.first = (*q)->data_.first;
      *p = *q;
      (*q) = (*q)->left_;
    }
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::clear(Node** node)
  {
    if (*node)
    {
      clear(std::addressof((*node)->left_));
      clear(std::addressof((*node)->right_));
      delete *node;
      *node = nullptr;
    }
  }

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >::BinarySearchTree():
    root_(nullptr)
  {}

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >::~BinarySearchTree()
  {
    clear(std::addressof(root_));
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::insert(const data_t& data)
  {
    insert(std::addressof(root_), data);
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::remove(const Key& data)
  {
    remove(std::addressof(root_), data);
  }

  template< typename Key, typename Value, typename Compare >
  bool BinarySearchTree< Key, Value, Compare >::isEmpty() const noexcept
  {
    return !root_;
  }

  template< typename Key, typename Value, typename Compare >
  typename bendryshev::BinarySearchTree< Key, Value, Compare >::CIterator BinarySearchTree< Key, Value, Compare >::cbegin() const noexcept
  {
    return ConstIterator(root_);
  }

  template< typename Key, typename Value, typename Compare >
  typename bendryshev::BinarySearchTree< Key, Value, Compare >::CIterator BinarySearchTree< Key, Value, Compare >::cend() const noexcept
  {
    return ConstIterator(nullptr);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::begin() noexcept
  {
    return Iterator(cbegin());
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::end() noexcept
  {
    return Iterator(cend());
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::ConstIterator BinarySearchTree< Key, Value, Compare >::cfind(const Key& data)
  {
    Node* finder = find(root_, data);
    if (!finder)
    {
      return cend();
    }
    else
    {
      ConstIterator cit = ConstIterator(finder);
      auto value_back = cit.node_dequeue.getTop();
      auto value_front = cit.node_dequeue.getFront();
      while (Compare()(value_back, value_front) || Compare()(value_front, value_back))
      {
        cit.node_dequeue.drop();
        value_back = cit.node_dequeue.getTop();
        value_front = cit.node_dequeue.getFront();
      }
      return cit;
    }
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::find(const Key& k)
  {
    return Iterator(cfind(k));
  }

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >::BinarySearchTree(const BinarySearchTree& other):
    root_(nullptr)
  {
    BinarySearchTree< Key, Value, Compare >::ConstIterator it = other.cbegin();
    try
    {
      while (it != other.cend())
      {
        this->insert(*it);
        ++it;
      }
    }
    catch (...)
    {
      this->clear();
    }
  }

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >::BinarySearchTree(BinarySearchTree&& other) noexcept:
    root_(other.root_)
  {
    other.root_ = nullptr;
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::clear()
  {
    clear(std::addressof(root_));
  }

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >& BinarySearchTree< Key, Value, Compare >::operator=(const BinarySearchTree& rhs)
  {
    if (this != std::addressof(rhs))
    {
      BinarySearchTree< Key, Value, Compare > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::swap(const BinarySearchTree& other) noexcept
  {
    using std::swap;
    swap(root_, other.root_);
  }

  template< typename Key, typename Value, typename Compare >
  BinarySearchTree< Key, Value, Compare >& BinarySearchTree< Key, Value, Compare >::operator=(BinarySearchTree&& rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      BinarySearchTree< Key, Value, Compare > temp(std::move(rhs));
      swap(temp);
    }
    return *this;
  }

  template< typename Key, typename Value, typename Compare >
  bool BinarySearchTree< Key, Value, Compare >::isEqual(const Key& first, const Key& second)
  {
    return !Compare()(first, second) && !Compare()(first, second);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::CIterator BinarySearchTree< Key, Value, Compare >::cfindUpperBound(const Key& key)
  {
    auto temp = cfind(key);
    if (temp != cend())
    {
      ++temp;
    }
    return temp;
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::findUpperBound(const Key& key)
  {
    return BinarySearchTree::Iterator(cfindUpperBound(key));
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::CIterator BinarySearchTree< Key, Value, Compare >::cfindLowerBound(const Key& key)
  {
    return cfind(key);
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::findLowerBound(const Key& key)
  {
    return BinarySearchTree::Iterator(cfindLowerBound(key));
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::insert(Iterator first, Iterator second)
  {
    while (first != second)
    {
      insert(*(first++));
    }
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::remove(Iterator first, Iterator second)
  {
    while (first != second)
    {
      remove(first++);
    }
  }

  template< typename Key, typename Value, typename Compare >
  void BinarySearchTree< Key, Value, Compare >::insert(std::initializer_list< data_t > list)
  {
    for (auto&& item: list)
    {
      insert(item);
    }
  }

  template< typename Key, typename Value, typename Compare >
  typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::remove(Iterator it)
  {
    if (it == end())
    {
      return it;
    }
    auto new_iter = it;
    new_iter++;
    remove(it->first);
    return new_iter;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename F >
  F BinarySearchTree< Key, Value, Compare >::traverseAscending(Node* root, F f)
  {
    Node* node = root;
    Stack< Node* > stack;
    while (true)
    {
      if (node)
      {
        stack.push(node);
        node = node->left_;
      }
      else
      {
        if (stack.isEmpty())
        {
          break;
        }
        node = stack.getTop();
        stack.drop();
        f(node->data_);
        node = node->right_;
      }
    }
    return f;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename F >
  F BinarySearchTree< Key, Value, Compare >::traverseBreadth(Node* root, F f)
  {
    Queue< Node* > queue;
    queue.push(root);
    while (!queue.isEmpty())
    {
      Node* current = queue.getNext();
      f(current->data_);
      if (current->left_)
      {
        queue.push(current->left_);
      }
      if (current->right_)
      {
        queue.push(current->right_);
      }
      queue.drop();
    }
    return f;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename F >
  F BinarySearchTree< Key, Value, Compare >::traverseDescending(Node* root, F f)
  {
    Node* node = root;
    Stack< Node* > stack;
    while (true)
    {
      if (node)
      {
        stack.push(node);
        node = node->right_;
      }
      else
      {
        if (stack.isEmpty())
        {
          break;
        }
        node = stack.getTop();
        stack.drop();
        f(node->data_);
        node = node->left_;
      }
    }
    return f;
  }

  template< typename Key, typename Value, typename Compare >
  template< typename F >
  F BinarySearchTree< Key, Value, Compare >::traverse_lnr(F f)
  {
    return traverseAscending(root_, f);
  }

  template< typename Key, typename Value, typename Compare >
  template< typename F >
  F BinarySearchTree< Key, Value, Compare >::traverse_breadth(F f)
  {
    return traverseBreadth(root_, f);
  }

  template< typename Key, typename Value, typename Compare >
  template< typename F >
  F BinarySearchTree< Key, Value, Compare >::traverse_rnl(F f)
  {
    return traverseDescending(root_, f);
  }
}

#endif
