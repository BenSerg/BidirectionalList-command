  #ifndef S4_TREENODE_HPP
  #define S4_TREENODE_HPP

  namespace bendryshev
  {
    namespace detail
    {
      template< typename T, typename Compare >
      struct TreeNode
      {
      public:
        T data_;
        TreeNode< T, Compare >* left_;
        TreeNode< T, Compare >* right_;
        int balanceFactor_;
      };
    }
  }
  #endif
