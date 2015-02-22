////////////////////////////////////////////////////////////////////////
// Heatmap.h: Inner declaration of the real Heatmap class
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

#include "HeatmapServiceTypes.h"
#include "CounterMap.hpp"
#include "HashMap.hpp"

namespace heatmap_service
{
  struct CounterKeyMap
  {
    std::string *counter_key;
    CounterMap *counter_map;
  };

  struct StringHashFunctor{
    int operator()(const std::string& s) { 
      return s.length(); 
    }
  };

  class HeatmapPrivate
  {
  private:
    using CMHashMap = SimpleHashmap<std::string, CounterMap, StringHashFunctor>;

    // Spatial Resolution of heatmap
    double single_unit_width_;
    double single_unit_height_;

    // We keep each CounterMap in an unordered array of KeyValues
    // This method of storing CounterKeys isn't the most well performing structure requiring traversal for access
    // But I assume that on a regular use of the Heatmap, not too many different counters will be used (maybe 10, 20 at most?)
    // As such, I believe the performance isn't sufficiently significant to justify a more complex structure
    CMHashMap key_map_hash_;

  public:
    // Spatial resolution initialization
    HeatmapPrivate();
    explicit HeatmapPrivate(double smallest_spatial_unit_size);
    HeatmapPrivate(double smallest_spatial_unit_width, double smallest_spatial_unit_height);
    HeatmapPrivate(const HeatmapPrivate& copy);
    HeatmapPrivate& operator=(const HeatmapPrivate& copy);
    ~HeatmapPrivate();

    // -- Getters for the current spatial resolution
    double single_unit_height() const;
    double single_unit_width() const;

    // -- Queries if a certain counter has ever been added to the heatmap
    bool hasMapForCounter(const std::string &counter_key) const;

    // -- Heatmap activity logging methods
    bool IncrementMapCounter(HeatmapCoordinate coords, const std::string &counter_key);

    bool IncrementMapCounterByAmount(HeatmapCoordinate coords, const std::string &counter_key, int add_amount);

    bool IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, const std::string counter_keys[], int amounts[], int counter_keys_length);

    // -- Heatmap query methods
    unsigned int getCounterAtPosition(HeatmapCoordinate coords, const std::string &counter_key) const;

    bool getCounterDataInsideRect(HeatmapCoordinate lower_left, HeatmapCoordinate upper_right, const std::string &counter_key, HeatmapData &out_data) const;

    bool getAllCounterData(const std::string &counter_key, HeatmapData &out_data) const;

    // -- Heatmap serialization
    bool SerializeHeatmap(char* &out_buffer, int &out_length) const;
    bool DeserializeHeatmap(const char* &in_buffer, int in_length);

  private:
    // -- Private Utility Functions
    // Adjust regular world space coordinates to the inner spatial resolution
    HeatmapCoordinate AdjustCoordsToSpatialResolution(HeatmapCoordinate coords) const;

    // Inner implementation of get counter inside rect. Receives already adjusted coordinates, called by public methods
    bool getCounterDataInsideAdjustedRect(HeatmapCoordinate adjusted_lower_left, HeatmapCoordinate adjusted_upper_right, const std::string &counter_key, HeatmapData &out_data) const;
  };
}