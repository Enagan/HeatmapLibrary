////////////////////////////////////////////////////////////////////////
// CounterMap.cpp: Implementation of the CounterMap helper class
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////

#include "CounterMap.hpp"
#include <iostream>

namespace heatmap_service
{
  CounterMap::CounterMap() : map_columns_(NULL)
  {
    InitializeMap();
  }
  CounterMap::~CounterMap()
  {
    DestroyMap();
  }

  // -- Getters of current map limits
  int CounterMap::lowest_coord_x()
  {
    return lowest_coord_x_;
  }
  int CounterMap::highest_coord_x()
  {
    return highest_coord_x_;
  }
  int CounterMap::lowest_coord_y()
  {
    return lowest_coord_y_;
  }
  int CounterMap::highest_coord_y()
  {
    return highest_coord_y_;
  }

  // -- Map registering methods
  bool CounterMap::IncrementValueAt(int coord_x, int coord_y)
  {
    return AddAmountAt(coord_x, coord_y, 1);
  }

  bool CounterMap::AddAmountAt(int coord_x, int coord_y, int amount)
  {
    //If the amount is 0 or lesser, we don't need to register to do anything
    if (amount <= 0)
      return true;

    // Resize the map, in case coord_x and coord_y are outside the current scope
    try {
      ResizeMapIfNeededFor(coord_x, coord_y);
    }
    catch (const std::bad_alloc& e) {
      std::cout << "[HEATMAP_SERVICE] ERROR: Could not register counter for coordinate { " << coord_x << " , " << coord_y << " }. Reason: \"" << e.what() << "\". Map may be too big to maintain" << std::endl;
      return false;
    }

    // Adjust coordinates from heatmap coord space, to inner index space by adding the negative padding
    int index_coord_x = coord_x + map_columns_negative_coord_padding_;
    int index_coord_y = coord_y + map_columns_[index_coord_x].col_negative_coord_padding;

    // Increment counter
    map_columns_[index_coord_x].column[index_coord_y] += amount;
    return true;
  }

  // -- Map query methods
  uint32_t CounterMap::getValueAt(int coord_x, int coord_y)
  {
    // Adjust coordinates from heatmap coord space, to inner index space by adding the negative padding
    int index_coord_x = coord_x + map_columns_negative_coord_padding_;
    int index_coord_y = coord_y + map_columns_[index_coord_x].col_negative_coord_padding;

    // If outside the current scope of the map, then no relevant value would be present, so return 0
    if (index_coord_x < 0 || index_coord_y < 0 ||
      index_coord_x >= map_columns_length_ || index_coord_y >= map_columns_[index_coord_x].col_length)
      return 0;

    // Otherwise, access and return value
    return map_columns_[index_coord_x].column[index_coord_y];
  }

  // -- Map Clear
  void CounterMap::ClearMap()
  {
    DestroyMap();
    InitializeMap();
  }

  // -- Private Utility Functions

  // -- Map initial alocation and destruction
  void CounterMap::InitializeMap()
  {
    if (map_columns_ != NULL)
      DestroyMap();

    lowest_coord_x_ = 0;
    highest_coord_x_ = 0;
    lowest_coord_y_ = 0;
    highest_coord_y_ = 0;

    map_columns_length_ = kInitialMapSize;
    map_columns_negative_coord_padding_ = kInitialNegativeCoordsPadding;
    map_columns_ = new Column[map_columns_length_];

    // Initialize columns
    for (int i = 0; i < map_columns_length_; i++)
    {
      map_columns_[i].col_length = kInitialMapSize;
      map_columns_[i].col_negative_coord_padding = kInitialNegativeCoordsPadding;
      map_columns_[i].column = new uint32_t[map_columns_[i].col_length]();
    }
  }

  void CounterMap::DestroyMap()
  {
    if (map_columns_ != NULL)
    {
      for (int i = 0; i < map_columns_length_; i++)
      {
        delete[] map_columns_[i].column;
        map_columns_[i].column = NULL;
      }
    }
    delete[] map_columns_;
    map_columns_ = NULL;
  }

  // -- Resize Methods
  void CounterMap::ResizeMapIfNeededFor(int coord_x, int coord_y)
  {
    // Grow forward in case coord_x is larger than map
    while (coord_x + map_columns_negative_coord_padding_ >= map_columns_length_)
      AddColumns();
    // Grow at beggining in case coord_x is negative and not yet contemplated in map
    while (coord_x + map_columns_negative_coord_padding_ < 0)
      InsertColumnsAtBeggining();


    // As we're now sure that coord_x fits, we adjust it to index space to check if coord_y fits on the coord_x column
    int index_coord_x = coord_x + map_columns_negative_coord_padding_;

    // Grow forward in case coord_y is larger than column
    while (coord_y + map_columns_[index_coord_x].col_negative_coord_padding >= map_columns_[index_coord_x].col_length)
      GrowColumnAtEnd(index_coord_x);
    // Grow at beggining in case coord_y is negative and not yet contemplated in column
    while (coord_y + map_columns_[index_coord_x].col_negative_coord_padding < 0)
      GrowColumnAtBeggining(index_coord_x);


    // Define if coordinate is new boundary
    CheckIfNewBoundary(coord_x, coord_y);
  }

  // -- Resize column array
  void CounterMap::AddColumns()
  {
    // Double the size
    int new_length = map_columns_length_ * 2;
    Column* new_map_columns = new Column[new_length];

    // Copy values from old at first half
    for (int i = 0; i < map_columns_length_; i++)
    {
      new_map_columns[i] = map_columns_[i];
    }
    // Initialize new columns at second half
    for (int i = map_columns_length_; i < new_length; i++)
    {
      new_map_columns[i].col_length = kInitialMapSize;
      new_map_columns[i].col_negative_coord_padding = kInitialNegativeCoordsPadding;
      new_map_columns[i].column = new uint32_t[new_map_columns[i].col_length]();
    }

    delete[] map_columns_;
    map_columns_ = new_map_columns;
    map_columns_length_ = new_length;
  }

  void CounterMap::InsertColumnsAtBeggining()
  {
    // Double the size
    int new_length = map_columns_length_ * 2;
    Column* new_map_columns = new Column[new_length];

    // Initialize new columns at first half
    for (int i = 0; i < map_columns_length_; i++)
    {
      new_map_columns[i].col_length = kInitialMapSize;
      new_map_columns[i].col_negative_coord_padding = kInitialNegativeCoordsPadding;
      new_map_columns[i].column = new uint32_t[new_map_columns[i].col_length]();
    }
    // Copy values from old at second half
    for (int i = map_columns_length_; i < new_length; i++)
    {
      new_map_columns[i] = map_columns_[i - map_columns_length_];
    }

    delete[] map_columns_;
    map_columns_ = new_map_columns;
    // Update negative coord padding. Since the size was doubled, we always add the old size to the previous padding
    map_columns_negative_coord_padding_ += map_columns_length_;
    map_columns_length_ = new_length;
  }

  // -- Resize specific column at provided x index
  void CounterMap::GrowColumnAtEnd(unsigned int index_coord_x)
  {
    // Just in case the respective column to resize doesn't exist yet.
    // Should never happen though
    while ((int)index_coord_x >= map_columns_length_)
      AddColumns();

    int new_length = map_columns_[index_coord_x].col_length * 2;
    // Initialize whole array
    uint32_t* new_column = new uint32_t[new_length]();

    // Copy values from old column
    for (int i = 0; i < map_columns_[index_coord_x].col_length; i++)
    {
      new_column[i] = map_columns_[index_coord_x].column[i];
    }

    delete[] map_columns_[index_coord_x].column;
    map_columns_[index_coord_x].column = new_column;
    map_columns_[index_coord_x].col_length = new_length;
  }

  void CounterMap::GrowColumnAtBeggining(unsigned int index_coord_x)
  {
    // Just in case the respective column to resize doesn't exist yet.
    // Should never happen though
    while ((int)index_coord_x >= map_columns_length_)
      AddColumns();

    int new_length = map_columns_[index_coord_x].col_length * 2;
    // Initialize whole array
    uint32_t* new_column = new uint32_t[new_length]();

    // Copy values from old column, to the second half of the new column
    for (int i = map_columns_[index_coord_x].col_length; i < new_length; i++)
    {
      new_column[i] = map_columns_[index_coord_x].column[i - map_columns_[index_coord_x].col_length];
    }

    delete[] map_columns_[index_coord_x].column;
    map_columns_[index_coord_x].column = new_column;
    // Update negative coord padding. Since the size was doubled, we always add the old size to the previous padding
    map_columns_[index_coord_x].col_negative_coord_padding += map_columns_[index_coord_x].col_length;
    map_columns_[index_coord_x].col_length = new_length;
  }

  // -- Checks if coordinate is a new boundary for the Map. If so, replace previous highest/lowest values
  void CounterMap::CheckIfNewBoundary(int coord_x, int coord_y)
  {
    if (coord_x < lowest_coord_x_)
      lowest_coord_x_ = coord_x;

    if (coord_y < lowest_coord_y_)
      lowest_coord_y_ = coord_y;

    if (coord_x > highest_coord_x_)
      highest_coord_x_ = coord_x;

    if (coord_y > highest_coord_y_)
      highest_coord_y_ = coord_y;
  }
}