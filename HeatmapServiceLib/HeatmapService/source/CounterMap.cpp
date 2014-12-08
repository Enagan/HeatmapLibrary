#include "HeatmapPrivate.h"
#include <iostream>

namespace heatmap_service
{
  // --------- Public Methods ------------ 
  CounterMap::CounterMap()
  {
    map_columns = NULL;
    InitializeMap();
  }
  CounterMap::~CounterMap()
  {
    DestroyMap();
  }

  bool CounterMap::IncrementValueAt(int coord_x, int coord_y)
  {
    return AddAmountAt(coord_x, coord_y, 1);
  }

  bool CounterMap::AddAmountAt(int coord_x, int coord_y, int amount)
  {
    int adjusted_coord_x, adjusted_coord_y;
    try {
      while (coord_x + map_columns_negative_coord_padding >= map_columns_length_)
        AddColumns();
      while (coord_x + map_columns_negative_coord_padding < 0)
        InsertColumnsAtBeggining();

      adjusted_coord_x = coord_x + map_columns_negative_coord_padding;

      while (coord_y + map_columns[adjusted_coord_x].col_negative_coord_padding >= map_columns[adjusted_coord_x].col_length)
        GrowColumnAtEnd(adjusted_coord_x);
      while (coord_y + map_columns[adjusted_coord_x].col_negative_coord_padding < 0)
        GrowColumnAtBeggining(adjusted_coord_x);
    }
    catch (const std::bad_alloc& e) {
      std::cout << "[HEATMAP_SERVICE] ERROR: Could not register counter for coordinate { " << coord_x << " , " << coord_y << " }. Reason: \"" << e.what() << "\". Map may be too big to maintain" << std::endl;
      return false;
    }

    adjusted_coord_x = coord_x + map_columns_negative_coord_padding;
    adjusted_coord_y = coord_y + map_columns[adjusted_coord_x].col_negative_coord_padding;

    map_columns[adjusted_coord_x].column[adjusted_coord_y] += amount;

    CheckIfNewBoundary(coord_x, coord_y);
    return true;
  }

  uint32_t CounterMap::getValueAt(int coord_x, int coord_y)
  {
    int adjusted_coord_x = coord_x + map_columns_negative_coord_padding;
    int adjusted_coord_y = coord_y + map_columns[adjusted_coord_x].col_negative_coord_padding;

    if (adjusted_coord_x < 0 || adjusted_coord_y < 0 ||
      adjusted_coord_x >= map_columns_length_ || adjusted_coord_y >= map_columns[adjusted_coord_x].col_length)
      return 0;

    return map_columns[adjusted_coord_x].column[adjusted_coord_y];
  }

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

  void CounterMap::ClearMap()
  {
    DestroyMap();
    InitializeMap();
  }

  // --------- Private Methods ------------
  void CounterMap::InitializeMap()
  {
    if (map_columns != NULL)
      DestroyMap();

    lowest_coord_x_ = 0;
    highest_coord_x_ = 0;
    lowest_coord_y_ = 0;
    highest_coord_y_ = 0;

    map_columns_length_ = kInitialMapSize;
    map_columns_negative_coord_padding = kInitialNegativeCoordsPadding;
    map_columns = new Column[map_columns_length_];

    for (int i = 0; i < map_columns_length_; i++)
    {
      map_columns[i].col_length = kInitialMapSize;
      map_columns[i].col_negative_coord_padding = kInitialNegativeCoordsPadding;
      map_columns[i].column = new uint32_t[map_columns[i].col_length]();
    }
  }

  void CounterMap::DestroyMap()
  {
    if (map_columns != NULL)
    {
      for (int i = 0; i < map_columns_length_; i++)
      {
        delete[] map_columns[i].column;
        map_columns[i].column = NULL;
      }
    }
    delete[] map_columns;
    map_columns = NULL;
  }

  void CounterMap::AddColumns()
  {
    int new_length = map_columns_length_ * 2;
    Column* new_map_columns = new Column[new_length];

    for (int i = 0; i < map_columns_length_; i++)
    {
      new_map_columns[i] = map_columns[i];
    }
    for (int i = map_columns_length_; i < new_length; i++)
    {
      new_map_columns[i].col_length = kInitialMapSize;
      new_map_columns[i].col_negative_coord_padding = kInitialNegativeCoordsPadding;
      new_map_columns[i].column = new uint32_t[new_map_columns[i].col_length]();
    }

    delete[] map_columns;
    map_columns = new_map_columns;
    map_columns_length_ = new_length;
  }

  void CounterMap::InsertColumnsAtBeggining()
  {
    int new_length = map_columns_length_ * 2;
    Column* new_map_columns = new Column[new_length];

    for (int i = 0; i < map_columns_length_; i++)
    {
      new_map_columns[i].col_length = kInitialMapSize;
      new_map_columns[i].col_negative_coord_padding = kInitialNegativeCoordsPadding;
      new_map_columns[i].column = new uint32_t[new_map_columns[i].col_length]();
    }
    for (int i = map_columns_length_; i < new_length; i++)
    {
      new_map_columns[i] = map_columns[i - map_columns_length_];
    }

    delete[] map_columns;
    map_columns = new_map_columns;
    map_columns_negative_coord_padding += map_columns_length_;
    map_columns_length_ = new_length;
  }

  void CounterMap::GrowColumnAtEnd(unsigned int adjusted_coord_x)
  {
    while ((int)adjusted_coord_x >= map_columns_length_)
      AddColumns();

    int new_length = map_columns[adjusted_coord_x].col_length * 2;
    uint32_t* new_column = new uint32_t[new_length]();

    for (int i = 0; i < map_columns[adjusted_coord_x].col_length; i++)
    {
      new_column[i] = map_columns[adjusted_coord_x].column[i];
    }

    delete[] map_columns[adjusted_coord_x].column;
    map_columns[adjusted_coord_x].column = new_column;
    map_columns[adjusted_coord_x].col_length = new_length;
  }

  void CounterMap::GrowColumnAtBeggining(unsigned int adjusted_coord_x)
  {
    while ((int)adjusted_coord_x >= map_columns_length_)
      AddColumns();

    int new_length = map_columns[adjusted_coord_x].col_length * 2;
    uint32_t* new_column = new uint32_t[new_length]();

    for (int i = map_columns[adjusted_coord_x].col_length; i < new_length; i++)
    {
      new_column[i] = map_columns[adjusted_coord_x].column[i - map_columns[adjusted_coord_x].col_length];
    }

    delete[] map_columns[adjusted_coord_x].column;
    map_columns[adjusted_coord_x].column = new_column;

    map_columns[adjusted_coord_x].col_negative_coord_padding += map_columns[adjusted_coord_x].col_length;
    map_columns[adjusted_coord_x].col_length = new_length;
  }

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