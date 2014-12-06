#include "HeatmapPrivate.h"
#include <iostream>

namespace heatmap_service
{
  CoordinatesMap::CoordinatesMap()
  {
    map_columns = NULL;
    InitializeMap();
  }
  CoordinatesMap::~CoordinatesMap()
  {
    DestroyMap();
  }

  void CoordinatesMap::IncrementValueAt(int coord_x, int coord_y)
  {
    AddAmountAt(coord_x, coord_y, 1);
  }
  void CoordinatesMap::AddAmountAt(int coord_x, int coord_y, int amount)
  {
    while (coord_x + map_columns_negative_coord_padding >= map_columns_length_)
      AddColumns();

    while (coord_x + map_columns_negative_coord_padding < 0)
      InsertColumnsAtBeggining();

    int adjusted_coord_x = coord_x + map_columns_negative_coord_padding;

    while (coord_y + map_columns[adjusted_coord_x].col_negative_coord_padding >= map_columns[adjusted_coord_x].col_length)
      GrowColumnAtEnd(adjusted_coord_x);

    while (map_columns[adjusted_coord_x].col_negative_coord_padding + coord_y < 0)
      GrowColumnAtBeggining(adjusted_coord_x);

    int adjusted_coord_y = coord_y + map_columns[adjusted_coord_x].col_negative_coord_padding;
    map_columns[adjusted_coord_x].column[adjusted_coord_y] += amount;
  }

  uint32_t CoordinatesMap::getValueAt(int coord_x, int coord_y)
  {
    int adjusted_coord_x = coord_x + map_columns_negative_coord_padding;
    int adjusted_coord_y = coord_y + map_columns[adjusted_coord_x].col_negative_coord_padding;

    if (adjusted_coord_x < 0 || adjusted_coord_y < 0 ||
      adjusted_coord_x >= map_columns_length_ || adjusted_coord_y >= map_columns[adjusted_coord_x].col_length)
      return 0;

    return map_columns[adjusted_coord_x].column[adjusted_coord_y];
  }

  void CoordinatesMap::ClearMap()
  {
    DestroyMap();
    InitializeMap();
  }

  // --------- Private Methods ------------
  void CoordinatesMap::InitializeMap()
  {
    if (map_columns != NULL)
      DestroyMap();

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

  void CoordinatesMap::DestroyMap()
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

  void CoordinatesMap::AddColumns()
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

  void CoordinatesMap::InsertColumnsAtBeggining()
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

  void CoordinatesMap::GrowColumnAtEnd(unsigned int adjusted_coord_x)
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

  void CoordinatesMap::GrowColumnAtBeggining(unsigned int adjusted_coord_x)
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
}