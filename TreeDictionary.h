#ifndef S2_DICTIONARY_H
#define S2_DICTIONARY_H

#include <stdexcept>
#include "BinarySearchTree.hpp"

namespace bendryshev
{
  template< typename Key, typename Value, typename Compare >
  class TreeDictionary
  {
  public:
    using dict_iterator = typename bendryshev::BinarySearchTree< Key, Value, Compare >::Iterator;
    using const_dict_iterator = typename bendryshev::BinarySearchTree< Key, Value, Compare >::ConstIterator;
    TreeDictionary() = default;
    TreeDictionary(std::initializer_list< typename std::pair< Key, Value > >);
    TreeDictionary(const TreeDictionary< Key, Value, Compare >&) = default;
    TreeDictionary(TreeDictionary< Key, Value, Compare >&&) noexcept = default;
    ~TreeDictionary() = default;
    TreeDictionary& operator=(const TreeDictionary&) = default;
    void push(const Key&, const Value&);
    void drop(const Key&);
    Value& get(const Key&);
    const Value& get(const Key&) const;
    bool isEmpty() const noexcept;
    dict_iterator begin() noexcept;
    const_dict_iterator cbegin() const noexcept;
    dict_iterator end() noexcept;
    const_dict_iterator cend() const noexcept;
    dict_iterator find(const Key&);
  private:
    BinarySearchTree < Key, Value, Compare > tree_;
  };

  template< typename Key, typename Value, typename Compare >
  void TreeDictionary< Key, Value, Compare >::push(const Key& k, const Value& v)
  {
    auto item = std::make_pair(k, v);
    tree_.insert(item);
  }

  template< typename Key, typename Value, typename Compare >
  TreeDictionary< Key, Value, Compare >::TreeDictionary(std::initializer_list< std::pair< Key, Value > > list):
    tree_()
  {
    for (auto&& item: list)
    {
      push(item.first, item.second);
    }
  }

  template< typename Key, typename Value, typename Compare >
  Value& TreeDictionary< Key, Value, Compare >::get(const Key& k)
  {
    auto it = tree_.find(k);
    return it->second;
  }

  template< typename Key, typename Value, typename Compare >
  const Value& TreeDictionary< Key, Value, Compare >::get(const Key& k) const
  {
    return get(k);
  }

  template< typename Key, typename Value, typename Compare >
  bool TreeDictionary< Key, Value, Compare >::isEmpty() const noexcept
  {
    return tree_.isEmpty();
  }

  template< typename Key, typename Value, typename Compare >
  typename TreeDictionary< Key, Value, Compare >::dict_iterator TreeDictionary< Key, Value, Compare >::begin() noexcept
  {
    return tree_.begin();
  }

  template< typename Key, typename Value, typename Compare >
  typename TreeDictionary< Key, Value, Compare >::const_dict_iterator TreeDictionary< Key, Value, Compare >::cbegin() const noexcept
  {
    return tree_.cbegin();
  }

  template< typename Key, typename Value, typename Compare >
  typename TreeDictionary< Key, Value, Compare >::dict_iterator TreeDictionary< Key, Value, Compare >::end() noexcept
  {
    return tree_.end();
  }

  template< typename Key, typename Value, typename Compare >
  typename TreeDictionary< Key, Value, Compare >::const_dict_iterator TreeDictionary< Key, Value, Compare >::cend() const noexcept
  {
    return tree_.cend();
  }

  template< typename Key, typename Value, typename Compare >
  typename TreeDictionary< Key, Value, Compare >::dict_iterator TreeDictionary< Key, Value, Compare >::find(const Key& k)
  {
    return tree_.find(k);
  }

  template< typename Key, typename Value, typename Compare >
  void TreeDictionary< Key, Value, Compare >::drop(const Key& key)
  {
    if (find(key) != end())
    {
      tree_.remove(key);
    }
  }

}
#endif
