////////////////////////////////////////////////////////////////////////
// HeatmapService.h: API for use of the HeatmapService Library
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include "HeatmapServiceTypes.h"

namespace heatmap_service
{
  // Forward declaration of private Heatmap class
  class HeatmapPrivate;

  // The HeatmapService class provides a robust and easy to use tool to log player activity spatially organized over 2d space.
  // It can log any counters on a coordinate basis to measure activity, such as deaths, gold collected, materials gathered, etc...
  // The Heatmap can be queried for it's stored values, from the smallest unit of space to the entirety of the record.
  // It can also serialize itself to a char* buffer and can later be restored from the same buffer.
  class HeatmapService
  {
  public:
    // The Heatmap can be initialized with spatial resolution, meaning that the smallest unit of space, in the eyes of the heatmap will be the one provided. 
    // This means that, for example, if the spatial resolution is {10, 8} incrementing a counter at the coords { 1 , 2 } and { 5 , 7 } will result in an increment to the same unit
    // If the default constructor is used, or negative (and zero) values are provided in any of the other constructor, the spatial resolution will be set to a default of { 1 , 1 }, respectively.
    HeatmapService();
    explicit HeatmapService(double smallest_spatial_unit_size);
    HeatmapService(double smallest_spatial_unit_width, double smallest_spatial_unit_height);
    HeatmapService(const HeatmapService& copy);
    HeatmapService& operator=(const HeatmapService& copy);

    ~HeatmapService();

    // -- Getters for the current spatial resolution
    double single_unit_height() const;
    double single_unit_width() const;
    HeatmapSize single_unit_size() const;

    // -- Queries if a certain counter has ever been added to the heatmap
    bool hasMapForCounter(const std::string &counter_key) const;

    // -- Heatmap activity logging methods
    // Incrementing or adding values to a counter in the heatmap is as simple as providing the coordinates and the counter key used (deaths, gold lost, etc...)
    // The heatmap supports any values for coordinates, both positive and negative, as well as fractional. The map will grow as needed to accommodate.
    // In case the map growth reaches the limits of memory available for the process, these functions will write error logs to cout and return false. All previously logged data will still be available
    bool IncrementMapCounter(HeatmapCoordinate coords, const std::string &counter_key);

    // Instead of simply incrementing by one, these functions allow the addition of any value to the counter, in case the provided value is zero or negative the counter won't be incremented
    bool IncrementMapCounterByAmount(HeatmapCoordinate coords, const std::string &counter_key, int add_amount);

    // It can be convenient to log to several different counters at the same time for the same coordinate (deaths, gold lost, etc...), this function provides syntax sugar for those situations
    bool IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, const std::string counter_keys[], int amounts[], int counter_keys_length);


    // -- Heatmap query methods
    // Similar to the logging methods, these fetch the heatmap values for any given counter. If data is requested from a counter that doesn't yet exist, or
    // if the provided coordinate was never previously logged, the return will be 0.
    // Querying a single coordinate inside a counter map has O(1) complexity, 
    // Querying an area, inside a counter map, will naturally have O(n) where n = width*height.
    unsigned int getCounterAtPosition(HeatmapCoordinate coords, const std::string &counter_key) const;

    // These methods fetch an area of the heatmap instead of a single point. The data is returned via the HeatmapData output parameter and the function returns true if successful.
    // Its the responsibility of the function that calls this to destroy HeatmapData when it no longer needs to be used
    bool getCounterDataInsideRect(HeatmapCoordinate lower_left, HeatmapCoordinate upper_right, const std::string &counter_key, HeatmapData &out_data) const;

    // This method behaves similarly to the area queries, but returns the entirety of the currently registered map data for the given counter
    // The counter value for any coordinate outside the area returned by this function is 0
    bool getAllCounterData(const std::string &counter_key, HeatmapData &out_data) const;


    // -- Heatmap serialization
    // The Heatmap can be serialized into a char* buffer. This buffer can be saved to a file and later restored with the serialize function
    // Serialization returns the buffer and it's size via the output parameters and returns true if successful, false if any error occurred.
    // Deserializing a char buffer into this map will firstly clean it of any data, and then load the serialized data in, 
    // make sure you saved your current data before you deserialize into a Heatmap
    // --- WARNING!: Serialization uses the boost serialization library
    // ---   More specifically, libboost_iostreams-vc120-mt-1_57.lib and libboost_serialization-vc120-mt-1_57.lib as well as the serialization and archive hpp headers.
    // ---   As such, boost exceptions will be launched upon errors or invalid input data
    bool SerializeHeatmap(char* &out_buffer, int &out_length) const;
    bool DeserializeHeatmap(const char* &in_buffer, int in_length);


    // -- Utility Functions
    // PrintHeatmapData is a static method that receives a heatmap data object and prints it's contents to the standard output.
    // Usefull to visually debug heatmap contents
    static void PrintHeatmapData(const heatmap_service::HeatmapData &data);

  private:
    // Internal instance of the Heatmap. Use of the pimpl idiom to hide private and internal methods from the library header
    HeatmapPrivate* private_heatmap_;
  };
}