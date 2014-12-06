#include <cstdint>

namespace heatmap_service
{

  struct Column
  {
    int col_length;
    int col_negative_coord_padding;
    uint32_t* column;
  };


  class CoordinatesMap
  {
  private:
    const int kInitialMapSize = 1024;
    const int kInitialNegativeCoordsPadding = 0;

    int map_columns_length_;
    int map_columns_negative_coord_padding;
    Column* map_columns;

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

    void AddColumns();
    void InsertColumnsAtBeggining();

    void GrowColumnAtEnd(unsigned int adjusted_coord_x);
    void GrowColumnAtBeggining(unsigned int adjusted_coord_x);

  };
}