////////////////////////////////////////////////////////////////////////
// CounterMap.cpp: Implementation of the CounterMap helper class
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////

#include "CounterMap.hpp"
#include <iostream>

namespace heatmap_service
{
  CounterMap::CounterMap() : lowest_coord_x_(0), highest_coord_x_(0), lowest_coord_y_(0), highest_coord_y_(0) { }
  CounterMap::CounterMap(const CounterMap& copy) : lowest_coord_x_(copy.lowest_coord_x_), highest_coord_x_(copy.highest_coord_x_),
    lowest_coord_y_(copy.lowest_coord_y_), highest_coord_y_(copy.highest_coord_y_), coord_matrix_(copy.coord_matrix_) { }
  CounterMap& CounterMap::operator=(const CounterMap& copy)
  {
    if (this != &copy)
      coord_matrix_ = copy.coord_matrix_;
    return *this;
  }
  CounterMap::~CounterMap(){ }

  // -- Getters of current map limits
  int CounterMap::lowest_coord_x() const
  {
    return lowest_coord_x_;
  }
  int CounterMap::highest_coord_x() const
  {
    return highest_coord_x_;
  }
  int CounterMap::lowest_coord_y() const
  {
    return lowest_coord_y_;
  }
  int CounterMap::highest_coord_y() const
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
    //If the amount is 0 or lesser, we don't need to do anything
    if (amount <= 0)
      return true;

    try {
      coord_matrix_[coord_x][coord_y] += amount;
    }
    catch (const std::bad_alloc& e) {
      std::cout << "[HEATMAP_SERVICE] ERROR: Could not register counter for coordinate { " << coord_x << " , " << coord_y << " }. Reason: \"" << e.what() << "\". Map may be too big to maintain" << std::endl;
      return false;
    }
    CheckIfNewBoundary(coord_x, coord_y);
    return true;
  }

  // -- Map query methods
  uint32_t CounterMap::getValueAt(int coord_x, int coord_y) const
  {
    // Get_at ensures that if coordinates fall outside the current scope of the vectors, 
    // then 0 will be returned as the default value of class uint_32_t, and no extra memory will be allocated
    return coord_matrix_.get_at(coord_x).get_at(coord_y);
  }

  // -- Map Clear
  void CounterMap::ClearMap()
  {
    coord_matrix_.clear();
  }

  // -- Private Utility Functions

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