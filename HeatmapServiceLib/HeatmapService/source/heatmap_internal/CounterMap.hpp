////////////////////////////////////////////////////////////////////////////////////////////////////////
// CounterMap.h: Declaration of CounterMap helper class. 
// Contains data for a specific counter
// Includes Boost libraries to serialize itself, these require an hpp header with implementation
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// for uint_32
#include <cstdint>

// Boost headers for Serialization
#include <boost\serialization\access.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\archive\binary_iarchive.hpp>

#include "SignedIndexVector.hpp"

namespace heatmap_service
{
  // -- CounterMap Class is a helper class for the Heatmap, capable of holding the spatial counter data for the Heatmap it's part of.
  // It doesn't need to know map size at instantiation, instead using the dinamically resizeable SignedIndexVector container to fit the needs of the Heatmap.
  // All accesses to the map are O(1) complexity. Incrementing is also O(1) except on situations where a resize is needed.
  class CounterMap
  {
  private:
    // Signed index vector deals with most of our dynamic resizing needs as well as both positive and negative indexing
    SignedIndexVector< SignedIndexVector<uint32_t> > coord_matrix_;

    // Highest and lowest values currently present in the map. Usefull when querying about full size
    int lowest_coord_x_;
    int highest_coord_x_;
    int lowest_coord_y_;
    int highest_coord_y_;
  public:
    CounterMap();
    CounterMap(const CounterMap& copy);
    CounterMap& operator=(const CounterMap& copy);
    ~CounterMap();

    // -- Getters of current map limits
    int lowest_coord_x() const;
    int highest_coord_x() const;
    int lowest_coord_y() const;
    int highest_coord_y() const;

    // -- Map registering methods
    // Increment the counters at the specified coordinates
    // Map will grow horizontally and vertically as necessessary to accomodate new data
    bool IncrementValueAt(int coord_x, int coord_y);
    bool AddAmountAt(int coord_x, int coord_y, int amount);

    // -- Map query methods
    // Returns counter value at given coordinate
    // If coordinate lies outside the current scope of the map, 0 is returned.
    uint32_t getValueAt(int coord_x, int coord_y) const;

    // -- Map Clear
    void ClearMap();

  private:
    // -- Private Utility Functions

    // -- Checks if coordinate is a new boundary for the Map. If so, replace previous highest/lowest values
    void CheckIfNewBoundary(int coord_x, int coord_y);

    // Boost serialization methods
    // Implement functionality on how to serialize and deserialize a CounterMap into a boost Archive
    // Used by master Heatmap class to serialize all it's instances of CounterMap
    // Versioning can be used in case implementation changes after deployment
    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
      // Save all basic values
      ar & lowest_coord_x_;
      ar & lowest_coord_y_;
      ar & highest_coord_x_;
      ar & highest_coord_y_;
      ar & coord_matrix_;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
      // Ensures map is cleaned and deallocated before loading the serialized values
      ClearMap();

      // Load all basic values
      ar & lowest_coord_x_;
      ar & lowest_coord_y_;
      ar & highest_coord_x_;
      ar & highest_coord_y_;
      ar & coord_matrix_;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
  };
}