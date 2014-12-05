#include "HeatmapPrivate.h"

namespace heatmap_service
{
  CoordinatesMap::CoordinatesMap()
  {
    accumulator_matrix_ = NULL;
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
    while (coord_x >= matrix_row_length_)
      ResizeRows();

    while (coord_y >= accumulator_matrix_[coord_x].column_length)
      ResizeColumn(coord_x);

    accumulator_matrix_[coord_x].column[coord_y] += amount;
  }

  uint32_t CoordinatesMap::getValueAt(int coord_x, int coord_y)
  {
    if (coord_x >= matrix_row_length_ || coord_y >= accumulator_matrix_[coord_x].column_length)
      return 0;

    return accumulator_matrix_[coord_x].column[coord_y];
  }

  void CoordinatesMap::ClearMap()
  {
    DestroyMap();
    InitializeMap();
  }

  void CoordinatesMap::InitializeMap()
  {
    if (accumulator_matrix_ != NULL)
      DestroyMap();

    matrix_row_length_ = kInitialMapSize;
    accumulator_matrix_ = new Column[matrix_row_length_];

    for (int i = 0; i < matrix_row_length_; i++)
    {
      accumulator_matrix_[i].column_length = kInitialMapSize;
      accumulator_matrix_[i].column = new uint32_t[accumulator_matrix_[i].column_length]();
    }
  }

  void CoordinatesMap::DestroyMap()
  {
    if (accumulator_matrix_ != NULL)
    {
      for (int i = 0; i < matrix_row_length_; i++)
      {
        delete(accumulator_matrix_[i].column);
      }
    }
    accumulator_matrix_ = NULL;
  }

  void CoordinatesMap::ResizeRows()
  {
    Column* new_matrix = new Column[matrix_row_length_*2];

    for (int i = 0; i < matrix_row_length_; i++)
    {
      new_matrix[i] = accumulator_matrix_[i];
    }
    for (int i = matrix_row_length_; i < matrix_row_length_ * 2; i++)
    {
      new_matrix[i].column_length = kInitialMapSize;
      new_matrix[i].column = new uint32_t[new_matrix[i].column_length]();
    }

    delete[] accumulator_matrix_;

    matrix_row_length_ *= 2;
    accumulator_matrix_ = new_matrix;
  }

  void CoordinatesMap::ResizeColumn(int coord_x)
  {
    while (coord_x >= matrix_row_length_)
      ResizeRows();

    uint32_t* new_column = new uint32_t[accumulator_matrix_[coord_x].column_length * 2]();

    for (int i = 0; i < accumulator_matrix_[coord_x].column_length; i++)
    {
      new_column[i] = accumulator_matrix_[coord_x].column[i];
    }

    delete[] accumulator_matrix_[coord_x].column;

    accumulator_matrix_[coord_x].column_length *= 2;
    accumulator_matrix_[coord_x].column = new_column;
  }
}