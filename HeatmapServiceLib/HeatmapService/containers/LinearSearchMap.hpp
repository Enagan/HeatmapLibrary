#pragma once
#include "SignedIndexVector.hpp"

#include <boost\serialization\access.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\archive\binary_iarchive.hpp>

namespace heatmap_service
{
  template < typename KeyT, typename ValT>
  class LinearSearchMap
  {
  private:

    struct KeyValPair{
      KeyT key;
      ValT val;

      KeyValPair() {}
      KeyValPair(const KeyT& k, const ValT& v) : key(k), val(v) {}
      KeyValPair(const KeyValPair& copy) : key(copy.key), val(copy.val) {}
      KeyValPair& operator=(const KeyValPair& copy){
        if (this != &copy)
        {
          key = copy.key;
          val = copy.val;
        }
        return *this;
      }
      ~KeyValPair(){}

      // Boost serialization methods
      friend class boost::serialization::access;
      template<class Archive>
      void serialize(Archive &ar, const unsigned int version)
      {
        ar & key;
        ar & val;
      }
    };

    SignedIndexVector< KeyValPair > map_;

  public:
    LinearSearchMap() {}
    LinearSearchMap(const LinearSearchMap& copy) : map_(copy.map_) {}
    LinearSearchMap& operator=(const LinearSearchMap& copy) {
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
      for (const KeyValPair& key_val : map_){
        if (key_val.key == key) return true;
      }
      return false;
    }

    void clear(){ map_.clear(); }

    void clean(){ map_.clean(); }
  private:

    ValT& GetOrCreateValForKey(const KeyT& key){
      for (KeyValPair& key_val : map_){
        if (key_val.key == key) return key_val.val;
      }

      map_.push_back(KeyValPair(key, ValT()));
      return (map_.end() - 1)->val;
    }

    const ValT& GetValForKey(const KeyT& key) const{
      for (const KeyValPair& key_val : map_){
        if (key_val.key == key) return key_val.val;
      }
      throw std::out_of_range("LinearSearchMap ERROR: Call to const operator[] with inexistant key");
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