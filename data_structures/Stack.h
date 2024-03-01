#ifndef S1_STACK_H
#define S1_STACK_H

#include "data_structures/BidirectionalList.h"

namespace bendryshev
{
  template< typename T >
  class Stack
  {
  public:

    void push(const T& rhs);
    const T& getTop() const;
    void drop();
    bool isEmpty() const;

  private:
    BidirectionalList< T > items_;
  };

  template< typename T >
  void bendryshev::Stack< T >::push(const T& rhs)
  {
    items_.pushFront(rhs);
  }
  template< typename T >
  const T& bendryshev::Stack< T >::getTop() const
  {
    if (items_.isEmpty())
    {
      throw std::logic_error("Stack is empty");
    }
    return items_.getFrontData();
  }

  template< typename T >
  void bendryshev::Stack< T >::drop()
  {
    if (isEmpty())
    {
      throw std::logic_error("Stack is empty");
    }
    items_.popFront();
  }

  template< typename T >
  bool bendryshev::Stack< T >::isEmpty() const
  {
    return items_.isEmpty();
  }
}
#endif
