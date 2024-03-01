#ifndef S1_QUEUE_H
#define S1_QUEUE_H

#include "data_structures/BidirectionalList.h"

namespace bendryshev
{
  template< typename T >
  class Queue
  {
  public:
    void push(const T& rhs);
    const T& getNext() const;
    void drop();
    bool isEmpty() const;
  private:
    BidirectionalList< T > items_;
  };

  template< typename T >
  void bendryshev::Queue< T >::push(const T& rhs)
  {
    items_.pushBack(rhs);
  }

  template< typename T >
  const T& bendryshev::Queue< T >::getNext() const
  {
    if (isEmpty())
    {
      throw std::logic_error("Queue is empty");
    }
    return items_.getFrontData();
  }

  template< typename T >
  void bendryshev::Queue< T >::drop()
  {
    if (isEmpty())
    {
      throw std::logic_error("Queue is empty");
    }
    items_.popFront();
  }

  template< typename T >
  bool bendryshev::Queue< T >::isEmpty() const
  {
    return items_.isEmpty();
  }
}
#endif
