#ifndef S3_DOUBLELINKEDNODE_H
#define S3_DOUBLELINKEDNODE_H

namespace bendryshev
{
  namespace detail
  {
    template< typename T >
    struct DoubleLinkedNode
    {
      T data_;
      DoubleLinkedNode< T >* pPrev_;
      DoubleLinkedNode< T >* pNext_;
    };
  }
}
#endif
