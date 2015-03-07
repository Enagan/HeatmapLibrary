///////////////////////////////////////////////////////////////////////////
// HashMap.hpp: A map-like class which stores it's KeyValue pairs through
//  The hashing of the key value into an indexable value for the internal
//  signed index vector. The Hash function is set through a Functor in 
//  as the third templated typename.
//  This hash map is still slightly unfinished, as it has some issues with
//  Hash functions that return values to big to index on a vector.
// Written by: Pedro Engana (http://pedroengana.com) 
///////////////////////////////////////////////////////////////////////////

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
  class SimpleHashmap
  {
  private:

    // Key-Value structure used internally to keep keys and values paired together while 
    // stored in the vector
    struct KeyValPair{
      // key and val are stored as pointers to greatly diminish the overhead of allocating
      // "defaultly" constructed KeyValPairs, if Key or Val happen to be complex types.
      // Since SignedIndexVector might fill in allocation gaps with default values
      // (when two objects are set in indexes with a lot of space in-between)
      // this was a performance necessity.
      std::shared_ptr<KeyT> key;
      std::shared_ptr<ValT> val;

      KeyValPair() {}
      KeyValPair(const KeyT& k, const ValT& v) : key(new KeyT(k)), val(new ValT(v)) {}
      KeyValPair(const KeyValPair& copy) : key(copy.key), val(copy.val) {}
      KeyValPair& operator=(const KeyValPair& copy){
        if (this != &copy) {
          key = copy.key;
          val = copy.val;
        }
        return *this;
      }

      // Boost serialization methods
      friend class boost::serialization::access;
      template<class Archive>
      void save(Archive & ar, const unsigned int version) const {
        ar & *key;
        ar & *val;
      }
      template<class Archive>
      void load(Archive & ar, const unsigned int version) {
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
    SimpleHashmap() {}
    SimpleHashmap(const SimpleHashmap& copy) : map_(copy.map_) {}
    SimpleHashmap& operator=(const SimpleHashmap& copy) {
      if (this != &copy)
        map_ = copy.map_;
      return *this;
    }

    // Returns an editable value for a certain key. Will allocate memory if key doesn't exist yet.
    ValT& operator[](const KeyT& key){
      return GetOrCreateValForKey(key);
    }

    // Returns a const value for a key, but throws out_of_range exception if key is non-existant
    const ValT& operator[] (const KeyT& key) const {
      return GetValForKey(key);
    }

    // Returns true if the key exists
    bool has_key(const KeyT& key) const {
      return map_.get_at(HashFunc()(key)).size() != 0;
    }

    void clear(){ map_.clear(); }

    void clean(){ map_.clean(); }
  private:

    // Finds the value that matches the given key in the hashmap. If the key is not yet present, a new Key Value Pair is created
    // and the default value from it is returned
    ValT& GetOrCreateValForKey(const KeyT& key){
      int hash_result = HashFunc()(key);
      //We get the hashbox, where all keys whose hashfunction collide with this key reside
      SignedIndexVector<KeyValPair>& hash_box = map_[hash_result];
      int hash_box_size = hash_box.size();

      if (hash_box_size == 0)
      {
        // Hashbox is empty, we got a default (empty) vector from the SignedIndexVector, Key is not present, so we create the new KvPair
        hash_box.push_back(KeyValPair(key, ValT()));
        return *(hash_box[0].val);
      }
      else {
        // Hashbox is not empty, one of the keys inside might match the one we're looking for
        for (SignedIndexVector<KeyValPair>::iterator iter = hash_box.begin(); iter != hash_box.end(); ++iter){
          if (*(iter->key) == key)
            return *(iter->val);
        }
        // If not, create a new KvPair
        hash_box.push_back(KeyValPair(key, ValT()));
        return *((hash_box.end() - 1)->val);
      }
    }

    const ValT& GetValForKey(const KeyT& key) const{
      if (!has_key(key))
        throw std::out_of_range("SimpleHashmap ERROR: Key does not exist in map");

      int hash_result = HashFunc()(key);
      const SignedIndexVector<KeyValPair>& hash_box = map_[hash_result];
      for (SignedIndexVector<KeyValPair>::const_iterator iter = hash_box.begin(); iter != hash_box.end(); ++iter){
        if (*(iter->key) == key)
          return *(iter->val);
      }
      throw std::out_of_range("SimpleHashmap ERROR: Key does not exist in map");
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