#ifndef S4_DEQUEUE_H
#define S4_DEQUEUE_H

#include "BidirectionalList.h"

namespace bendryshev
{
  template< typename T >
  class Dequeue
  {
  public:

    void push(const T& rhs);
    const T& getTop() const;
    const T& getFront() const;
    void drop();
    bool isEmpty() const;

  private:
    BidirectionalList< T > items_;
  };

  template< typename T >
  void bendryshev::Dequeue< T >::push(const T& rhs)
  {
    items_.pushFront(rhs);
  }

  template< typename T >
  const T& bendryshev::Dequeue< T >::getTop() const
  {
    if (items_.isEmpty())
    {
      throw std::logic_error("Stack is empty");
    }
    return items_.getFrontData();
  }

  template< typename T >
  void bendryshev::Dequeue< T >::drop()
  {
    if (isEmpty())
    {
      throw std::logic_error("Stack is empty");
    }
    items_.popFront();
  }

  template< typename T >
  bool bendryshev::Dequeue< T >::isEmpty() const
  {
    return items_.isEmpty();
  }

  template< typename T >
  const T& Dequeue< T >::getFront() const
  {
    if (items_.isEmpty())
    {
      throw std::logic_error("Stack is empty");
    }
    return items_.getBackData();
  }
}
#endif
