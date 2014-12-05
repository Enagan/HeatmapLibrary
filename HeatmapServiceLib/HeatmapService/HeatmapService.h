#pragma once
#include <string>
#include <memory>
#include "HeatmapServiceTypes.h"

namespace heatmap_service
{
  class Heatmap;

  HeatmapCoordinate MakeHeatmapCoordinate(double x, double y);

  class HeatmapService
  {
  public:
    HeatmapService();
    HeatmapService(double smallest_spatial_unit_size);
    HeatmapService(double smallest_spatial_unit_width, double smallest_spatial_unit_height);
    ~HeatmapService();

    void IncrementMapCounter(double coord_x, double coord_y, std::string &counter_key);
    void IncrementMapCounter(HeatmapCoordinate coords, std::string &counter_key);

    void IncrementMapCounterByAmount(double coord_x, double coord_y, std::string &counter_key, int add_amount);
    void IncrementMapCounterByAmount(HeatmapCoordinate coords, std::string &counter_key, int add_amount);

    void IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, std::string counter_keys[], int amounts[], int counter_keys_length);

    bool SerializeHeatmap(char* &out_buffer);
    bool DeserializeHeatmap(char* &in_buffer);

    // LIB TEST
    double Add(double a, double b);

    unsigned int getCounterAtPosition(double coord_x, double coord_y, std::string &counter_key);

  private:
    std::unique_ptr<Heatmap> pimpl_;
  };
}