////////////////////////////////////////////////////////////////////////
// HeatmapService.h: API for use of the HeatmapService Library
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <memory>
#include "HeatmapServiceTypes.h"

namespace heatmap_service
{
  // Forward declaration of private Heatmap class
  class Heatmap;

  // The HeatmapService class provides a robust and easy to use tool to log player activity spatially organized over 2d space.
  // It can log any counters on a coordinate basis to measure activity, such as deaths, gold collected, materials gathered, etc...
  // The Heatmap can be queried for it's stored values, from the smallest unit of space to the entirety of the record.
  // It can also serialize itself to a char* buffer and can later be restored from the same buffer.
  class HeatmapService
  {
  public:
    // The Heatmap can be intialized with spatial resolution, meaning that the smalest unit of space, in the eyes of the heatmap will be the one provided. 
    // This means that, for example, if the spatial resolution is {10, 8} incrementing a counter at the coords { 1 , 2 } and { 5 , 7 } will result in an increment to the same unit
    // If the default constructor is used, or negative (and zero) values are provided in any of the other constructor, the spatial resolution will be set to a default of { 1 , 1 }, respectively.
    HeatmapService();
    HeatmapService(double smallest_spatial_unit_size);
    HeatmapService(double smallest_spatial_unit_width, double smallest_spatial_unit_height);
    HeatmapService(HeatmapSize smallest_spatial_unit_size);

    ~HeatmapService();

    // -- Getters for the current spatial resolution
    double single_unit_height();
    double single_unit_width();
    HeatmapSize single_unit_size();

    // -- Queries if a certain counter has ever been added to the heatmap
    bool hasMapForCounter(const std::string &counter_key);

    // -- Heatmap activity logging methods
    // Incrementing or adding values to a counter in the heatmap is a simple as providing the coordinates and the counter key used
    // The heatmap supports any values for coordinates, both positive and negative, as well as fractional. The map will grow as needed to accomodate.
    // In case the map growth reaches the limits of memory available for the process, these functions will write error logs to cout and return false. All previously logged data will still be available
    // Counter keys can be passed both as a c string or an std::string depending on what is most comfortable for the programmer at the point these methods are called.
    bool IncrementMapCounter(double coord_x, double coord_y, const std::string &counter_key);
    bool IncrementMapCounter(HeatmapCoordinate coords, const std::string &counter_key);

    // Instead of simply incrementing by one, these functions allow the addition of any value to the counter
    bool IncrementMapCounterByAmount(double coord_x, double coord_y, const std::string &counter_key, unsigned int add_amount);
    bool IncrementMapCounterByAmount(HeatmapCoordinate coords, const std::string &counter_key, unsigned int add_amount);

    // It can be convinient to log to several different counters at the same time for the same coordinate (deaths, gold lost, etc...), this function provides syntax sugar for those situations
    bool IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, const std::string counter_keys[], unsigned int amounts[], int counter_keys_length);


    // -- Heatmap query methods
    // Similar to the logging methods, these fetch the heatmap values for any given counter. If data is requested from a counter that doesn't yet exist, or
    // if the provided coordinate was never previously logged, the return will be 0.
    // Querying a single coordinate inside a counter map has O(1) complexity, 
    // (reaching the correct counter map is O(n), where n = amount of diferent counters (deaths, gold, etc...). It's not expected that enough counters need to be logged for this to be a significant bottleneck)
    // Querying an area, inside a counter map, will naturally have O(n) where n = width*height.
    unsigned int getCounterAtPosition(double coord_x, double coord_y, const std::string &counter_key);
    unsigned int getCounterAtPosition(HeatmapCoordinate coords, const std::string &counter_key);

    // These methods fetch an area of the heatmap instead of a single point. The data is returned via the HeatmapData output parameter and the funtion returns true if successfull.
    // Its the responsibility of the function that calls this to destroy HeatmapData when it's no longer needs to be used
    bool getCounterDataInsideRect(double lower_coord_x, double lower_coord_y, double upper_coord_x, double upper_coord_y, const std::string &counter_key, HeatmapData &out_data);
    bool getCounterDataInsideRect(HeatmapCoordinate lower_left, HeatmapCoordinate upper_right, const std::string &counter_key, HeatmapData &out_data);

    // These method behaves similarly to the area queries, but returns the entirety of the map data for the given counter
    bool getAllCounterData(const std::string &counter_key, HeatmapData &out_data);


    // -- Heatmap serialization
    // The Heatmap can be serialized into a char* buffer. This buffer can be saved to a file and later restored with the deserialize function
    // Serialization returns the buffer and it's size via the output parameters and returns true if successfull, false if any error occured.
    // Deserializing a char buffer into this map will firstly clean it of any data, and then load the serialized data in, 
    // make sure you saved your current data before you deserialize into a Heatmap
    // --- WARNING!: Serialization uses the boost serialization library
    // ---   More specifically, libboost_iostreams-vc120-mt-1_57.lib and libboost_serialization-vc120-mt-1_57.lib as well as the serialization and archive hpp headers.
    bool SerializeHeatmap(char* &out_buffer, int &out_length);
    bool DeserializeHeatmap(const char* &in_buffer, int in_length);

  private:
    // Internal instance of the Heatmap. Use of the pimpl idiom to hide private and internal methods from the library header
    // Use of a smart pointer to relieve concerns about deallocation internally
    std::unique_ptr<Heatmap> private_heatmap_;
  };
}