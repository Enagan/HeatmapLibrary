////////////////////////////////////////////////////////////////////////
// Heatmap.h: Inner declaration of real Heatmap class
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include "HeatmapServiceTypes.h"
#include "CoordinatesMap.hpp"

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
    Heatmap(HeatmapSize smallest_spatial_unit_size);
    ~Heatmap();

    bool hasMapForCounter(std::string &counter_key);
    double single_unit_height();
    double single_unit_width();

    void IncrementMapCounter(double coord_x, double coord_y, std::string &counter_key);
    void IncrementMapCounter(HeatmapCoordinate coords, std::string &counter_key);

    void IncrementMapCounterByAmount(double coord_x, double coord_y, std::string &counter_key, int add_amount);
    void IncrementMapCounterByAmount(HeatmapCoordinate coords, std::string &counter_key, int add_amount);

    void IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, std::string counter_keys[], int amounts[], int counter_keys_length);

    bool SerializeHeatmap(char* &out_buffer, int &out_length);
    bool DeserializeHeatmap(const char* &in_buffer, int in_length);

    unsigned int getCounterAtPosition(double coord_x, double coord_y, std::string &counter_key);
    unsigned int getCounterAtPosition(HeatmapCoordinate coords, std::string &counter_key);

    bool getCounterDataInsideRect(double lower_coord_x, double lower_coord_y, double upper_coord_x, double upper_coord_y, std::string &counter_key, HeatmapData &out_data);
    bool getCounterDataInsideRect(HeatmapCoordinate lower_left, HeatmapCoordinate upper_right, std::string &counter_key, HeatmapData &out_data);

    bool getAllCounterData(std::string &counter_key, HeatmapData &out_data);

  private:
    HeatmapCoordinate AdjustCoordsToSpatialResolution(HeatmapCoordinate coords);

    CounterKeyValue addNewCounter(std::string &counter_key);
    CoordinatesMap* getOrAddMapForCounter(std::string &counter_key);
    bool ContainsCounterForKey(std::string &counter_key);

    void DestroyHeatmap();
  };
}