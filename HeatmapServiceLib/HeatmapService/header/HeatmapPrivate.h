#pragma once
#include <string>
#include "HeatmapServiceTypes.h"
#include "CoordinatesMap.h"

namespace heatmap_service
{
  struct CounterKeyValue
  {
    std::string *counter_key;
    CoordinatesMap *counter_map;
  };

  class Heatmap
  {
  private:
    double single_unit_width_;
    double single_unit_height_;

    int amount_of_counters_;
    CounterKeyValue *counter_glossary_;

  public:
    Heatmap();
    Heatmap(double smallest_spatial_unit_size);
    Heatmap(double smallest_spatial_unit_width, double smallest_spatial_unit_height);
    ~Heatmap();

    bool hasMapForCounter(std::string &counter_key);

    void IncrementMapCounter(double coord_x, double coord_y, std::string &counter_key);
    void IncrementMapCounter(HeatmapCoordinate coords, std::string &counter_key);

    void IncrementMapCounterByAmount(double coord_x, double coord_y, std::string &counter_key, int add_amount);
    void IncrementMapCounterByAmount(HeatmapCoordinate coords, std::string &counter_key, int add_amount);

    void IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, std::string counter_keys[], int amounts[], int counter_keys_length);

    bool SerializeHeatmap(char* &out_buffer);
    bool DeserializeHeatmap(char* &in_buffer);

    // LIB TEST
    double Add(double a, double b);

  private:
    CounterKeyValue addNewCounter(std::string &counter_key);
    CoordinatesMap* getOrAddMapForCounter(std::string &counter_key);
  };
}