#ifndef S3_BIDIRECTIONALLIST_H
#define S3_BIDIRECTIONALLIST_H

#include <stdexcept>
#include <cassert>
#include "DoubleLinkedNode.h"

namespace bendryshev
{
  template< typename T >
  class BidirectionalList
  {
  public:
    BidirectionalList();
    BidirectionalList(const BidirectionalList< T >&);
    BidirectionalList(std::initializer_list< T >);
    BidirectionalList(BidirectionalList< T >&&) noexcept;
    ~BidirectionalList();

    BidirectionalList< T >& operator=(const BidirectionalList< T >&);
    BidirectionalList< T >& operator=(BidirectionalList< T >&&) noexcept;

    void swap(BidirectionalList< T >&) noexcept;
    void clear();
    void pushFront(const T&);
    void pushBack(const T&);
    void popFront();
    void popBack();
    bool isEmpty() const noexcept;
    const T& getBackData() const;
    const T& getFrontData() const;

    struct ConstIterator
    {
      friend class BidirectionalList< T >;

      using this_t = bendryshev::BidirectionalList< T >::ConstIterator;
      ConstIterator() = default;
      ConstIterator(const this_t&) = default;
      ~ConstIterator() = default;
      this_t& operator=(const this_t&) = default;
      this_t& operator++();
      this_t operator--();
      this_t operator++(int);
      this_t operator--(int);
      const T& operator*() const;
      const T* operator->() const;
      bool operator!=(const this_t&) const;
      bool operator==(const this_t&) const;
    private:
      detail::DoubleLinkedNode< T >* node_;
      explicit ConstIterator(detail::DoubleLinkedNode< T >*);
    };

    struct Iterator
    {
      friend class BidirectionalList< T >;

      using this_t = Iterator;
      Iterator() = default;
      Iterator(const this_t&) = default;
      explicit Iterator(ConstIterator);
      ~Iterator() = default;
      this_t& operator=(const this_t&) = default;
      this_t& operator++();
      this_t operator--();
      this_t operator++(int);
      this_t operator--(int);
      T& operator*();
      T* operator->();
      bool operator!=(const this_t&) const;
      bool operator==(const this_t&) const;
    private:
      ConstIterator cit_;
    };

    Iterator insertBefore(const T&, BidirectionalList< T >::Iterator);
    Iterator erase(Iterator);
    Iterator find(const T& data);
    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

    bool operator==(BidirectionalList< T >&);
    bool operator!=(BidirectionalList< T >&);

    template< typename... Args >
    Iterator emplace(Iterator, Args&& ...);

    unsigned long getSize() const;
  private:
    detail::DoubleLinkedNode< T >* head_;
    detail::DoubleLinkedNode< T >* tail_;

  };

  template< typename T >
  BidirectionalList< T >::Iterator::Iterator(BidirectionalList::ConstIterator citer):
    cit_(citer)
  {}

  template< typename T >
  typename BidirectionalList< T >::Iterator::this_t& BidirectionalList< T >::Iterator::operator++()
  {
    ++cit_;
    return *this;
  }

  template< typename T >
  typename BidirectionalList< T >::Iterator::this_t BidirectionalList< T >::Iterator::operator++(int)
  {
    return Iterator(cit_++);
  }

  template< typename T >
  T& BidirectionalList< T >::Iterator::operator*()
  {
    return const_cast< T& >(*cit_);
  }

  template< typename T >
  T* BidirectionalList< T >::Iterator::operator->()
  {
    return const_cast< T* >(std::addressof(*cit_));
  }

  template< typename T >
  bool BidirectionalList< T >::Iterator::operator!=(const BidirectionalList::Iterator::this_t& rhs) const
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool BidirectionalList< T >::Iterator::operator==(const BidirectionalList::Iterator::this_t& rhs) const
  {
    return cit_ == rhs.cit_;
  }

  template< typename T >
  typename BidirectionalList< T >::Iterator::this_t BidirectionalList< T >::Iterator::operator--()
  {
    --cit_;
    return *this;
  }

  template< typename T >
  typename BidirectionalList< T >::Iterator::this_t BidirectionalList< T >::Iterator::operator--(int)
  {
    return Iterator(cit_--);
  }

  template< typename T >
  BidirectionalList< T >::ConstIterator::ConstIterator(detail::DoubleLinkedNode< T >* node):
    node_(node)
  {}

  template< typename T >
  typename BidirectionalList< T >::ConstIterator::this_t& BidirectionalList< T >::ConstIterator::operator++()
  {
    assert(node_ != nullptr);
    node_ = node_->pNext_;
    return *(this);
  }

  template< typename T >
  typename BidirectionalList< T >::ConstIterator::this_t BidirectionalList< T >::ConstIterator::operator--()
  {
    assert(node_ != nullptr);
    node_ = node_->pPrev_;
    return *(this);
  }

  template< typename T >
  typename BidirectionalList< T >::ConstIterator::this_t BidirectionalList< T >::ConstIterator::operator++(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    ++(*this);
    return result;
  }

  template< typename T >
  typename BidirectionalList< T >::ConstIterator::this_t BidirectionalList< T >::ConstIterator::operator--(int)
  {
    assert(node_ != nullptr);
    this_t result(*this);
    --(*this);
    return result;
  }

  template< typename T >
  const T& BidirectionalList< T >::ConstIterator::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< typename T >
  const T* BidirectionalList< T >::ConstIterator::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< typename T >
  bool BidirectionalList< T >::ConstIterator::operator!=(const BidirectionalList::ConstIterator::this_t& rhs) const
  {
    return !(*this == rhs);
  }

  template< typename T >
  bool BidirectionalList< T >::ConstIterator::operator==(const BidirectionalList::ConstIterator::this_t& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< typename T >
  BidirectionalList< T >::BidirectionalList():
    head_(nullptr),
    tail_(nullptr)
  {}

  template< typename T >
  BidirectionalList< T >::BidirectionalList(const BidirectionalList< T >& rhs):
    head_(nullptr),
    tail_(nullptr)
  {
    if (!rhs.isEmpty())
    {
      try
      {
        detail::DoubleLinkedNode< T >* rhs_node = rhs.head_->pNext_;
        head_ = new detail::DoubleLinkedNode< T > { rhs.head_->data_ };
        detail::DoubleLinkedNode< T >* this_node = head_;
        while (rhs_node)
        {
          this_node->pNext_ = new detail::DoubleLinkedNode< T > { rhs_node->data_, this_node };
          this_node = this_node->pNext_;
          rhs_node = rhs_node->pNext_;
        }
        tail_ = this_node;
      }
      catch (...)
      {
        clear();
      }
    }
  }

  template< typename T >
  BidirectionalList< T >::BidirectionalList(BidirectionalList< T >&& rhs) noexcept:
    head_(rhs.head_),
    tail_(rhs.tail_)
  {
    rhs.head_ = nullptr;
    rhs.tail_ = nullptr;
  }

  template< typename T >
  BidirectionalList< T >::~BidirectionalList()
  {
    clear();
  }

  template< typename T >
  BidirectionalList< T >& BidirectionalList< T >::operator=(const BidirectionalList< T >& rhs)
  {
    if (this != std::addressof(rhs))
    {
      BidirectionalList< T > temp(rhs);
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  BidirectionalList< T >& BidirectionalList< T >::operator=(BidirectionalList< T >&& rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      BidirectionalList< T > temp(std::move(rhs));
      swap(temp);
    }
    return *this;
  }

  template< typename T >
  void BidirectionalList< T >::swap(BidirectionalList< T >& rhs) noexcept
  {
    using std::swap;
    swap(head_, rhs.head_);
    swap(tail_, rhs.tail_);
  }

  template< typename T >
  void BidirectionalList< T >::clear()
  {
    while (!isEmpty())
    {
      popFront();
    }
  }

  template< typename T >
  void BidirectionalList< T >::pushFront(const T& data)
  {
    auto* node = new detail::DoubleLinkedNode< T > { data };
    if (head_ == nullptr)
    {
      head_ = node;
      tail_ = node;
    }
    else
    {
      node->pNext_ = head_;
      head_ = node;
      node->pNext_->pPrev_ = node;
    }
  }

  template< typename T >
  void BidirectionalList< T >::pushBack(const T& data)
  {
    auto* node = new detail::DoubleLinkedNode< T > { data };
    if (head_ == nullptr)
    {
      head_ = node;
      tail_ = head_;
    }
    else
    {
      node->pPrev_ = tail_;
      tail_->pNext_ = node;
      tail_ = node;
    }
  }

  template< typename T >
  void BidirectionalList< T >::popFront()
  {
    if (!isEmpty())
    {
      detail::DoubleLinkedNode< T >* temp = head_->pNext_;
      if (head_ == tail_)
      {
        tail_ = nullptr;
      }
      delete head_;
      head_ = temp;
      if (head_)
      {
        head_->pPrev_ = nullptr;
      }
    }
    else
    {
      throw std::logic_error("List is empty");
    }
  }

  template< typename T >
  void BidirectionalList< T >::popBack()
  {
    if (!isEmpty())
    {
      detail::DoubleLinkedNode< T >* temp = tail_->pPrev_;
      if (head_ == tail_)
      {
        head_ = nullptr;
      }
      delete tail_;
      tail_ = temp;
      if (tail_)
      {
        tail_->pNext_ = nullptr;
      }
    }
    else
    {
      throw std::logic_error("List is empty");
    }
  }

  template< typename T >
  bool BidirectionalList< T >::isEmpty() const noexcept
  {
    return head_ == nullptr;
  }

  template< typename T >
  typename BidirectionalList< T >::ConstIterator BidirectionalList< T >::cbegin() const noexcept
  {
    return ConstIterator(head_);
  }

  template< typename T >
  typename BidirectionalList< T >::ConstIterator BidirectionalList< T >::cend() const noexcept
  {
    return ConstIterator(nullptr);
  }

  template< typename T >
  typename BidirectionalList< T >::Iterator BidirectionalList< T >::begin() noexcept
  {
    return Iterator(cbegin());
  }

  template< typename T >
  typename BidirectionalList< T >::Iterator BidirectionalList< T >::end() noexcept
  {
    return BidirectionalList::Iterator(cend());
  }

  template< typename T >
  BidirectionalList< T >::BidirectionalList(std::initializer_list< T > il) :
    head_(nullptr),
    tail_(nullptr)
  {
    for (auto&& item: il)
    {
      pushBack(item);
    }
  }

  template< typename T >
  typename bendryshev::BidirectionalList< T >::Iterator BidirectionalList< T >::erase(Iterator it)
  {
    Iterator it_to_return = it;
    if (it == begin())
    {
      ++it_to_return;
      popFront();
    }
    else if (it == end())
    {
      popBack();
    }
    else
    {
      ++it_to_return;
      detail::DoubleLinkedNode< T >* temp_node = it.cit_.node_;
      temp_node->pPrev_->pNext_ = temp_node->pNext_;
      if (temp_node->pNext_)
      {
        temp_node->pNext_->pPrev_ = temp_node->pPrev_;
      }
      delete temp_node;
    }
    return it_to_return;
  }

  template< typename T >
  bool BidirectionalList< T >::operator==(BidirectionalList< T >& other)
  {
    BidirectionalList< T >::Iterator first = begin();
    BidirectionalList< T >::Iterator second = other.begin();
    while (first != end() && second != other.end())
    {
      if (*first != *second)
      {
        return false;
      }
      ++first;
      ++second;
    }
    if ((first == end() && second != other.end()) || (first != end() && second == other.end()))
    {
      return false;
    }
    return true;
  }

  template< typename T >
  typename BidirectionalList< T >::Iterator BidirectionalList< T >::insertBefore(const T& data, BidirectionalList< T >::Iterator it)
  {
    Iterator it_to_return = it;
    if (isEmpty())
    {
      ++it_to_return;
      pushFront(data);
    }
    else if (it == end())
    {
      pushBack(data);
    }
    else
    {
      ++it_to_return;
      detail::DoubleLinkedNode< T >* this_node = it.cit_.node_;
      auto* new_node = new detail::DoubleLinkedNode< T > { data, this_node->pPrev_, this_node, };
      if (this_node->pPrev_ != nullptr)
      {
        this_node->pPrev_->pNext_ = new_node;
      }
      this_node->pPrev_ = new_node;
      it.cit_.node_ = new_node;
    }
    return it_to_return;
  }

  template< typename T >
  typename BidirectionalList< T >::Iterator BidirectionalList< T >::find(const T& data)
  {
    BidirectionalList< T >::Iterator it = begin();
    while (it != end() && (*it != data))
    {
      ++it;
    }
    return it;
  }

  template< typename T >
  bool BidirectionalList< T >::operator!=(BidirectionalList< T >& rhs)
  {
    return !(*this == rhs);
  }

  template< typename T >
  template< typename... Args >
  typename BidirectionalList< T >::Iterator BidirectionalList< T >::emplace(BidirectionalList::Iterator position, Args&& ... args)
  {
    if (position == begin())
    {
      pushFront(T(std::move(args)...));
      return begin();
    }
    else if (position == end())
    {
      pushBack(T(std::move(args)...));
    }
    return insertBefore(T(std::move(args)...), position);
  }

  template< typename T >
  const T& BidirectionalList< T >::getBackData() const
  {
    if (isEmpty())
    {
      throw std::logic_error("List is empty");
    }
    return tail_->data_;
  }

  template< typename T >
  const T& BidirectionalList< T >::getFrontData() const
  {
    if (isEmpty())
    {
      throw std::logic_error("List is empty");
    }
    return head_->data_;
  }

  template< typename T >
  unsigned long BidirectionalList< T >::getSize() const
  {
    unsigned long size = 0;
    auto it = cbegin();
    while (it != cend())
    {
      ++size;
      ++it;
    }
    return size;
  }

}
#endif
