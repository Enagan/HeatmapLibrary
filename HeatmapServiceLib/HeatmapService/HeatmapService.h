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

    double single_unit_height();
    double single_unit_width();

    void IncrementMapCounter(double coord_x, double coord_y, std::string &counter_key);
    void IncrementMapCounter(HeatmapCoordinate coords, std::string &counter_key);

    void IncrementMapCounterByAmount(double coord_x, double coord_y, std::string &counter_key, int add_amount);
    void IncrementMapCounterByAmount(HeatmapCoordinate coords, std::string &counter_key, int add_amount);

    void IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, std::string counter_keys[], int amounts[], int counter_keys_length);

    bool SerializeHeatmap(char* &out_buffer, int &out_length);
    bool DeserializeHeatmap(const char* &in_buffer, int in_length);

    // LIB TEST
    double Add(double a, double b);

    unsigned int getCounterAtPosition(double coord_x, double coord_y, std::string &counter_key);
    unsigned int getCounterAtPosition(HeatmapCoordinate coords, std::string &counter_key);

    bool getCounterDataInsideRect(double lower_coord_x, double lower_coord_y, double upper_coord_x, double upper_coord_y, std::string &counter_key, HeatmapData &out_data);
    bool getCounterDataInsideRect(HeatmapCoordinate lower_left, HeatmapCoordinate upper_right, std::string &counter_key, HeatmapData &out_data);

    bool getAllCounterData(std::string &counter_key, HeatmapData &out_data);

  private:
    std::unique_ptr<Heatmap> pimpl_;
  };
}