#pragma once
#include "SignedIndexVector.hpp"
#include <algorithm>
#include <limits>
#include <functional>
#include <exception>
#include <memory>

#include <boost\serialization\access.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\archive\binary_iarchive.hpp>

namespace heatmap_service
{
  template < typename KeyT, typename ValT, typename HashFunc>
  class Hashmap
  {
  private:

    struct KeyValPair{
      std::shared_ptr<KeyT> key;
      std::shared_ptr<ValT> val;

      KeyValPair() {}
      KeyValPair(KeyT k, ValT v) : key(new KeyT(k)), val(new ValT(v)) {}
      KeyValPair(const KeyValPair& copy) : key(copy.key), val(copy.val) {}
      KeyValPair& operator=(const KeyValPair& copy){
        if (this != &copy)
        {
          key = copy.key;
          val = copy.val;
        }
        return *this;
      }

      // Boost serialization methods
      friend class boost::serialization::access;
      template<class Archive>
      void save(Archive & ar, const unsigned int version) const
      {
        ar & *key;
        ar & *val;
      }
      template<class Archive>
      void load(Archive & ar, const unsigned int version)
      {
        KeyT read_key;
        ValT read_val;

        ar & read_key;
        ar & read_val;

        key = std::shared_ptr<KeyT>( new KeyT(read_key) );
        val = std::shared_ptr<ValT>( new ValT(read_val) );
      }
      BOOST_SERIALIZATION_SPLIT_MEMBER()
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
      return GetValForKey(key);
    }

    bool has_key(const KeyT& key) const {
      return map_.get_at(HashFunc()(key)).size() != 0;
    }

    void clear(){ map_.clear(); }

    void clean(){ map_.clean(); }
  private:

    ValT& GetOrCreateValForKey(const KeyT& key){
      int hash_result = HashFunc()(key);
      SignedIndexVector<KeyValPair>& hash_box = map_[hash_result];
      if (hash_box.size() == 0)
      {
        hash_box.push_back(KeyValPair(key, ValT()));
        return *(hash_box[0].val);
      }
      else if (hash_box.size() == 1){
        return *(hash_box[0].val);
      }
      else {
        SignedIndexVector<KeyValPair>::iterator elem = std::find_if(hash_box.begin(),
          hash_box.end(), [&key](const KeyValPair& pair) -> bool { return *(pair.key) == key; });
        if (elem != hash_box.end())
          return *(elem->val);

        hash_box.push_back(KeyValPair(key, ValT()));
        return *((hash_box.end() - 1)->val);
      }
    }

    const ValT& GetValForKey(const KeyT& key) const{
      if (!has_key(key))
        throw std::out_of_range("Hashmap ERROR: Call to const operator[] with inexistant key");

      int hash_result = HashFunc()(key);
      const SignedIndexVector<KeyValPair>& hash_box = map_[hash_result];
      if (hash_box.size() == 1)
        return *(hash_box[0].val);

      SignedIndexVector<KeyValPair>::const_iterator elem = std::find_if(hash_box.cbegin(),
        hash_box.cend(), [&key](const KeyValPair& pair) -> bool { return *(pair.key) == key; });
      if (elem != hash_box.cend())
        return *(elem->val);
      else
        throw std::out_of_range("Hashmap ERROR: Call to const operator[] with inexistant key");
    }


    // Boost serialization methods
    // Implement functionality on how to serialize and deserialize a CounterMap into a boost Archive
    // Used by master Heatmap class to serialize all it's instances of CounterMap
    // Versioning can be used in case implementation changes after deployment
    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
      ar & map_;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
      // Ensures the vector is cleaned and deallocated before loading the serialized values
      clean();

      // Load all basic values
      ar & map_;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
  };
}