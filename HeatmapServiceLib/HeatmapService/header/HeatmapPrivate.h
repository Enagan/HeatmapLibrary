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
    // Spatial resolution initialization
    Heatmap();
    Heatmap(double smallest_spatial_unit_size);
    Heatmap(double smallest_spatial_unit_width, double smallest_spatial_unit_height);
    Heatmap(HeatmapSize smallest_spatial_unit_size);
    ~Heatmap();

    // -- Getters for the current spatial resolution
    double single_unit_height();
    double single_unit_width();

    // -- Queries if a certain counter has ever been added to the heatmap
    bool hasMapForCounter(const std::string &counter_key);

    // -- Heatmap activity logging methods
    bool IncrementMapCounter(double coord_x, double coord_y, const std::string &counter_key);
    bool IncrementMapCounter(HeatmapCoordinate coords, const std::string &counter_key);

    bool IncrementMapCounterByAmount(double coord_x, double coord_y, const  std::string &counter_key, unsigned int add_amount);
    bool IncrementMapCounterByAmount(HeatmapCoordinate coords, const std::string &counter_key, unsigned int add_amount);

    bool IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, const std::string counter_keys[], unsigned int amounts[], int counter_keys_length);

    // -- Heatmap query methods
    unsigned int getCounterAtPosition(double coord_x, double coord_y, const std::string &counter_key);
    unsigned int getCounterAtPosition(HeatmapCoordinate coords, const std::string &counter_key);

    bool getCounterDataInsideRect(double lower_coord_x, double lower_coord_y, double upper_coord_x, double upper_coord_y, const std::string &counter_key, HeatmapData &out_data);
    bool getCounterDataInsideRect(HeatmapCoordinate lower_left, HeatmapCoordinate upper_right, const std::string &counter_key, HeatmapData &out_data);

    bool getAllCounterData(const std::string &counter_key, HeatmapData &out_data);

    // -- Heatmap serialization
    bool SerializeHeatmap(char* &out_buffer, int &out_length);
    bool DeserializeHeatmap(const char* &in_buffer, int in_length);

  private:
    // -- Private Utility Functions
    HeatmapCoordinate AdjustCoordsToSpatialResolution(HeatmapCoordinate coords);

    CounterKeyValue addNewCounter(const std::string &counter_key);
    CoordinatesMap* getOrAddMapForCounter(const std::string &counter_key);

    bool ContainsCounterForKey(const std::string &counter_key);

    void DestroyHeatmap();
  };
}