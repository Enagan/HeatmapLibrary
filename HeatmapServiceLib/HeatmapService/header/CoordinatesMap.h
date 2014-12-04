
namespace heatmap_service
{
  class CoordinatesMap
  {
  public:
    CoordinatesMap();
    ~CoordinatesMap();

    void IncrementValueAt(HeatmapCoordinate coord);
    void AddAmountAt(HeatmapCoordinate coord, int amount);

  };
}