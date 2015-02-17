#pragma once
#include "SignedIndexVector.hpp"
#include <algorithm>
#include <limits>
#include <functional>
#include <exception>

namespace heatmap_service
{
  template < typename KeyT, typename ValT, typename HashFunc>
  class Hashmap
  {
  private:

    struct KeyValPair{
      KeyT key;
      ValT val;

      KeyValPair() : key(KeyT()), val(ValT()) {}
      KeyValPair(KeyT k, ValT v) : key(k), val(v) {}
      KeyValPair(const KeyValPair& copy) : key(copy.key), val(copy.val) {}
      KeyValPair& operator=(const KeyValPair& copy){
        if (this != &copy)
        {
          key = copy.key;
          val = copy.val;
        }
        return *this;
      }
    };

    SignedIndexVector< SignedIndexVector<KeyValPair> > map_;

  public:
    Hashmap() {}
    Hashmap(const Hashmap& copy) : map_(copy.map_) {}
    Hashmap& operator=(const Hashmap& copy) {
      if (this != &copy)
        map_ = copy.map_;
      return *this;
    }

    ValT& operator[](const KeyT& key){
      return GetOrCreateValForKey(key);
    }

    const ValT& operator[] (const KeyT& key) const {
      return GetOrCreateValForKey(key);
    }

    bool has_key(const KeyT& key) const {
      return map_.get_at(HashFunc()(key)).size() != 0;
    }

    void clear(){ map_.clear() }

    void clean(){ map_.clean() }
  private:

    ValT& GetOrCreateValForKey(const KeyT& key){
      int hash_result = HashFunc()(key);
      if (map_[hash_result].size() == 0)
      {
        map_[hash_result].push_back(KeyValPair(key, ValT()));
        return map_[hash_result][0].val;
      }
      else
      {
        SignedIndexVector<KeyValPair>::iterator elem = std::find_if(map_[hash_result].begin(), 
                                                                    map_[hash_result].end(), [&key](const KeyValPair& pair) -> bool { return pair.key == key; });
        if (elem != map_[hash_result].end())
          return (*elem).val;

        map_[hash_result].push_back(KeyValPair(key, ValT()));
        return (map_[hash_result].end() - 1)->val;
      }
    }

  };
}