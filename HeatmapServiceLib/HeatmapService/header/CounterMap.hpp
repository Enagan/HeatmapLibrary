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

namespace heatmap_service
{

  // Defines a column in the CounterMap.
  // Each column has it's own independent size, only increased from the initial value if the specific column actually requires more space
  struct Column
  {
    int col_length;
    int col_negative_coord_padding;
    uint32_t* column;
  };

  // -- CounterMap Class is a helper class for the Heatmap, capable of holding the spatial counter data for the Heatmap it's part of.
  // It doesn't need to know map size at instantiation, instead dinamically resizing itself to fit the needs of the Heatmap.
  // It can resize specific columns independently of others, keeping memory footprint as low as possible.
  // All accesses to the map are O(1) complexity. Incrementing is also O(1) except on situations where a resize is needed.
  class CounterMap
  {
  private:
    // Initial values for the columns array, as well as each column inside
    // An initial size of 1 provided the best performance/space relation in tests
    const int kInitialMapSize = 1;
    const int kInitialNegativeCoordsPadding = 0;

    Column* map_columns_;
    int map_columns_length_;
    // Negative coordinates padding defines how many indexes, at the start of the array, are negative coordinates.
    // another way to see this, is that it holds the index of coordinate 0
    int map_columns_negative_coord_padding_;

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
    // Grows map horizontally and vertically as necessessary to accomodate new data
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

    // -- Map initial alocation and destruction
    void InitializeMap();
    void DestroyMap();

    // -- Resize Methods
    void ResizeMapIfNeededFor(int coord_x, int coord_y);

    // -- Resize column array, adding more columns at the end, in case of a simple resize
    // Or at the beggining, in case a new lowest negative X coordinate was sent
    // On resize, size is always double, in an attempt to more easily reach the full value of the map, in an attempt to save resizing calls.
    void AddColumns();
    void InsertColumnsAtBeggining();

    // -- Resize specific column at provided x index, adding more space at the end, in case of a simple resize
    // Or at the beggining, in case a new lowest negative Y coordinate was sent
    // On resize, size is always doubled, in an attempt to more easily reach the full value of the map, in an attempt to save resizing calls.
    void GrowColumnAtEnd(unsigned int index_coord_x);
    void GrowColumnAtBeggining(unsigned int index_coord_x);

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
      ar & map_columns_length_;
      ar & map_columns_negative_coord_padding_;
      ar & lowest_coord_x_;
      ar & lowest_coord_y_;
      ar & highest_coord_x_;
      ar & highest_coord_y_;

      // Serialize each column
      for (int i = 0; i < map_columns_length_; i++)
      {
        ar & map_columns_[i].col_length;
        ar & map_columns_[i].col_negative_coord_padding;

        // Serialize each value inside the column
        for (int n = 0; n < map_columns_[i].col_length; n++)
        {
          ar & map_columns_[i].column[n];
        }
      }
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
      // Ensures map is cleaned and deallocated before loading the serialized values
      DestroyMap();

      // Load all basic values
      ar & map_columns_length_;
      ar & map_columns_negative_coord_padding_;
      ar & lowest_coord_x_;
      ar & lowest_coord_y_;
      ar & highest_coord_x_;
      ar & highest_coord_y_;

      // Allocate column array and load all columns
      map_columns_ = new Column[map_columns_length_];
      for (int i = 0; i < map_columns_length_; i++)
      {
        ar & map_columns_[i].col_length;
        ar & map_columns_[i].col_negative_coord_padding;

        // Allocate data in column and all columns accumulators
        map_columns_[i].column = new uint32_t[map_columns_[i].col_length]();
        for (int n = 0; n < map_columns_[i].col_length; n++)
        {
          ar & map_columns_[i].column[n];
        }
      }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
  };
}