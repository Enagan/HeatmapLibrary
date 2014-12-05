#include <cstdint>

namespace heatmap_service
{

  struct Column
  {
    int column_length;
    uint32_t* column;
  };


  class CoordinatesMap
  {
  private:
    const int kInitialMapSize = 1024;

    int matrix_row_length_;
    Column* accumulator_matrix_;

  public:
    CoordinatesMap();
    ~CoordinatesMap();

    void IncrementValueAt(int coord_x, int coord_y);
    void AddAmountAt(int coord_x, int coord_y, int amount);

    uint32_t getValueAt(int coord_x, int coord_y);

    void ClearMap();

  private:
    void InitializeMap();
    void DestroyMap();

    void ResizeRows();
    void ResizeColumn(int coord_x);

  };
}