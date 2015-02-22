////////////////////////////////////////////////////////////////////////
// Heatmap.cpp: Inner definition of the real Heatmap class
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////

#include "HeatmapPrivate.h"
#include <string.h>

// Boost headers for Serialization
#include <boost\iostreams\stream.hpp>
#include <boost\iostreams\device\back_inserter.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\archive\binary_iarchive.hpp>

namespace heatmap_service
{
  // Spatial resolution initialization
  HeatmapPrivate::HeatmapPrivate() : single_unit_width_(1), single_unit_height_(1) {}

  HeatmapPrivate::HeatmapPrivate(double smallest_spatial_unit_size) : single_unit_width_(smallest_spatial_unit_size > 0 ? smallest_spatial_unit_size : 1), 
    single_unit_height_(smallest_spatial_unit_size > 0 ? smallest_spatial_unit_size : 1){}

  HeatmapPrivate::HeatmapPrivate(double smallest_spatial_unit_width, double smallest_spatial_unit_height) : 
    single_unit_width_(smallest_spatial_unit_width > 0 ? smallest_spatial_unit_width : 1), single_unit_height_(smallest_spatial_unit_height > 0 ? smallest_spatial_unit_height : 1){}

  HeatmapPrivate::HeatmapPrivate(const HeatmapPrivate& copy) : single_unit_width_(copy.single_unit_width_), single_unit_height_(copy.single_unit_height_), 
    key_map_hash_(copy.key_map_hash_) {}

  HeatmapPrivate& HeatmapPrivate::operator=(const HeatmapPrivate& copy)
  {
    if (this != &copy)
      key_map_hash_ = copy.key_map_hash_;
    return *this;
  }

  HeatmapPrivate::~HeatmapPrivate() {}

  // -- Getters for the current spatial resolution
  double HeatmapPrivate::single_unit_height() const
  {
    return single_unit_height_;
  }

  double HeatmapPrivate::single_unit_width() const
  {
    return single_unit_width_;
  }

  // Queries if a certain counter has ever been added to the heatmap
  bool HeatmapPrivate::hasMapForCounter(const std::string& counter_key) const
  {
    return key_map_hash_.has_key(counter_key);
  }

  // -- Heatmap activity logging methods
  bool HeatmapPrivate::IncrementMapCounter(HeatmapCoordinate coords, const std::string &counter_key)
  {
    return IncrementMapCounterByAmount(coords, counter_key, 1);
  }

  bool HeatmapPrivate::IncrementMapCounterByAmount(HeatmapCoordinate coords, const std::string &counter_key, int add_amount)
  {
    HeatmapCoordinate adjusted_coords = AdjustCoordsToSpatialResolution(coords);
    return key_map_hash_[counter_key].AddAmountAt((int)adjusted_coords.x, (int)adjusted_coords.y, add_amount);
  }

  bool HeatmapPrivate::IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, const std::string counter_keys[], int amounts[], int counter_keys_length)
  {
    bool result = true;
    for (int i = 0; i < counter_keys_length; i++)
    {
      result = result && IncrementMapCounterByAmount(coords, counter_keys[i], amounts[i]);
    }
    return result;
  }

  // -- Heatmap query methods
  unsigned int HeatmapPrivate::getCounterAtPosition(HeatmapCoordinate coords, const std::string &counter_key) const
  {
    if (!hasMapForCounter(counter_key))
      return 0;

    HeatmapCoordinate adjusted_coords = AdjustCoordsToSpatialResolution(coords);
    return key_map_hash_[counter_key].getValueAt((int)adjusted_coords.x, (int)adjusted_coords.y);
  }

  bool HeatmapPrivate::getCounterDataInsideRect(HeatmapCoordinate lower_left, HeatmapCoordinate upper_right, const std::string &counter_key, HeatmapData &out_data) const
  {
    if (!hasMapForCounter(counter_key))
      return false;

    // First we adjust the rectangle to the inner resolution
    HeatmapCoordinate adjusted_lower_left = AdjustCoordsToSpatialResolution(lower_left);
    HeatmapCoordinate adjusted_upper_right = AdjustCoordsToSpatialResolution(upper_right);

    return getCounterDataInsideAdjustedRect(adjusted_lower_left, adjusted_upper_right, counter_key, out_data);
  }

  bool HeatmapPrivate::getAllCounterData(const std::string &counter_key, HeatmapData &out_data) const
  {
    if (!hasMapForCounter(counter_key))
      return false;

    const CounterMap& map_for_counter = key_map_hash_[counter_key];

    return getCounterDataInsideAdjustedRect({ map_for_counter.lowest_coord_x(), map_for_counter.lowest_coord_y() },
                                            { map_for_counter.highest_coord_x(), map_for_counter.highest_coord_y() }, counter_key, out_data);
  }

  // -- Heatmap serialization
  bool HeatmapPrivate::SerializeHeatmap(char* &out_buffer, int &out_length) const
  {
    // First we setup a boost stream, to write to a std::string
    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> buffer_destination(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > stream(buffer_destination);
    boost::archive::binary_oarchive oa(stream);

    // We write the class values to the string, through the stream. Boost knows how to serialize the values
    oa << single_unit_width_;
    oa << single_unit_height_;

    oa & key_map_hash_;

    // flush when done writting
    stream.flush();

    // Write the data contained in the std::string to a regular char* buffer
    char * writable = new char[serial_str.size()];
    memcpy_s(writable, serial_str.size(), serial_str.data(), serial_str.size());

    // Return the data
    out_buffer = writable;
    out_length = serial_str.size();

    return true;
  }

  bool HeatmapPrivate::DeserializeHeatmap(const char* &in_buffer, int in_length)
  {
    // Cleans current heatmap, so that the serialized data can be loaded while avoiding memory leaks
    key_map_hash_.clean();

    // Wrap char* buffer inside a stream to read from
    boost::iostreams::basic_array_source<char> buffer_source(in_buffer, in_length);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > stream(buffer_source);
    boost::archive::binary_iarchive ia(stream);

    // Read serialized values from buffer
    ia >> single_unit_width_;
    ia >> single_unit_height_;
    ia & key_map_hash_;

    return true;
  }

  // -- Private Utility Functions
  // Adjust regular world space coordinates to the inner spatial resolution
  HeatmapCoordinate HeatmapPrivate::AdjustCoordsToSpatialResolution(HeatmapCoordinate coords) const
  {
    return { floor(coords.x / single_unit_width_), floor(coords.y / single_unit_height_) };
  }

  // Inner implementation of get counter inside rect. Receives already adjusted coordinates
  bool HeatmapPrivate::getCounterDataInsideAdjustedRect(HeatmapCoordinate adjusted_lower_left, HeatmapCoordinate adjusted_upper_right, 
    const std::string &counter_key, HeatmapData &out_data) const
  {
    // If the countermap doesn't exist, or if the area is invalid, we return with a failure
    if (!hasMapForCounter(counter_key) || adjusted_lower_left.x > adjusted_upper_right.x || adjusted_lower_left.y > adjusted_upper_right.y)
      return false;

    const CounterMap& map_for_counter = key_map_hash_[counter_key];

    int width = (int)adjusted_upper_right.x - (int)adjusted_lower_left.x + 1;
    int height = (int)adjusted_upper_right.y - (int)adjusted_lower_left.y + 1;

    try {
      // We create a matrix of values, sized acording to the provided rectangle
      out_data.heatmap_data = new uint32_t*[width];
      for (int i = 0; i < width; i++)
      {
        out_data.heatmap_data[i] = new uint32_t[height]();
      }
      // We query the map for each value, using the adjusted coordinates, until we reach the expected width and height
      for (int x = 0; x < width; x++)
      {
        for (int y = 0; y < height; y++)
        {
          out_data.heatmap_data[x][y] = map_for_counter.getValueAt((int)adjusted_lower_left.x + x, (int)adjusted_lower_left.y + y);
        }
      }
    }
    catch (const std::bad_alloc& e) {
      std::cout << "[HEATMAP] ERROR: Could not build output for rect [ {" << adjusted_lower_left.x << "," << adjusted_lower_left.x << "} ] - [ {" <<
        adjusted_upper_right.x << "," << adjusted_upper_right.x << "} ] .Reason: \"" << e.what() << "\". Area may be too big to maintain in memory" << std::endl;
      return false;
    }

    // We fill the remaining values of HeatmapData if creation of the matrix was successfull
    out_data.counter_name = new std::string(counter_key);
    out_data.lower_left_coordinate = adjusted_lower_left;
    out_data.spatial_resolution = { single_unit_width_, single_unit_height_ };
    out_data.data_size = { width, height };

    return true;
  }
}