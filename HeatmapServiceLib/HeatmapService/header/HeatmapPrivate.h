////////////////////////////////////////////////////////////////////////
// Heatmap.h: Inner declaration of the real Heatmap class
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

#include "HeatmapServiceTypes.h"
#include "CounterMap.hpp"

namespace heatmap_service
{
  struct CounterKeyMap
  {
    std::string *counter_key;
    CounterMap *counter_map;
  };

  class Heatmap
  {
  private:
    // Spatial Resolution of heatmap
    double single_unit_width_;
    double single_unit_height_;

    // We keed each CounterMap in an unordered array of KeyValues
    // This method of storing CounterKeys isn't the most well performing structure requiring traversal for access
    // But I assume that on a regular use of the Heatmap, not too many different counters will be used (maybe 10, 20 at most?)
    // As such, I believe the performance isn't sufficiently significant to justify a more complex structure
    CounterKeyMap *key_map_dictionary_;
    int key_map_dictionary_length_;

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
    // Adjust regular world space coordinates to the inner spatial resolution
    HeatmapCoordinate AdjustCoordsToSpatialResolution(HeatmapCoordinate coords);

    // Adds new counter maps to the heatmap
    CounterKeyMap addNewCounter(const std::string &counter_key);
    // Looks if the counter exists, if yes, returns its Counter map, if not, a new Counter map is created and returned
    CounterMap* getOrAddMapForCounter(const std::string &counter_key);

    // Inner implementation of get counter inside rect. Receives already adjusted coordinates, called by public methods
    bool getCounterDataInsideAdjustedRect(HeatmapCoordinate adjusted_lower_left, HeatmapCoordinate adjusted_upper_right, const std::string &counter_key, HeatmapData &out_data);

    // Cleans the heatmap, deleting all counter maps and freeing memory
    void DestroyHeatmap();
  };
}