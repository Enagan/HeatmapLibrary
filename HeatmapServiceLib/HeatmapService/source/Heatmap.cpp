#include "HeatmapPrivate.h"
#include <string.h>

// Boost headers for Serialization
#include <boost\iostreams\stream.hpp>
#include <boost\iostreams\device\back_inserter.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\archive\binary_iarchive.hpp>

namespace heatmap_service
{
  Heatmap::Heatmap() : single_unit_width_(1), single_unit_height_(1), amount_of_counters_(0)
  {
    counter_glossary_ = NULL;
  }

  Heatmap::Heatmap(double smallest_spatial_unit_size) : amount_of_counters_(0)
  {
    single_unit_width_ = single_unit_height_ = smallest_spatial_unit_size > 0 ? smallest_spatial_unit_size : 1;
    counter_glossary_ = NULL;
  }

  Heatmap::Heatmap(double smallest_spatial_unit_width,
    double smallest_spatial_unit_height) : amount_of_counters_(0)
  {
    single_unit_width_ = smallest_spatial_unit_width > 0 ? smallest_spatial_unit_width : 1;
    single_unit_height_ = smallest_spatial_unit_height > 0 ? smallest_spatial_unit_height : 1;
    counter_glossary_ = NULL;
  }

  Heatmap::Heatmap(HeatmapSize smallest_spatial_unit_size) : amount_of_counters_(0)
  {
    single_unit_width_ = smallest_spatial_unit_size.width > 0 ? smallest_spatial_unit_size.width : 1;
    single_unit_height_ = smallest_spatial_unit_size.height > 0 ? smallest_spatial_unit_size.height : 1;
    counter_glossary_ = NULL;
  }

  Heatmap::~Heatmap()
  {
    DestroyHeatmap();
  }

  bool Heatmap::hasMapForCounter(std::string& counter_key)
  {
    for (int i = 0; i < amount_of_counters_; i++)
    {
      if (counter_glossary_[i].counter_key->compare(counter_key) == 0)
        return true;
    }
    return false;
  }

  double Heatmap::single_unit_height()
  {
    return single_unit_height_;
  }

  double Heatmap::single_unit_width()
  {
    return single_unit_width_;
  }

  void Heatmap::IncrementMapCounter(double coord_x, double coord_y, std::string& counter_key)
  {
    IncrementMapCounter({ coord_x, coord_y }, counter_key);
  }

  void Heatmap::IncrementMapCounter(HeatmapCoordinate coords, std::string& counter_key)
  {
    IncrementMapCounterByAmount(coords, counter_key, 1);
  }

  void Heatmap::IncrementMapCounterByAmount(double coord_x, double coord_y, std::string& counter_key, int add_amount)
  {
    IncrementMapCounterByAmount({ coord_x, coord_y }, counter_key, add_amount);
  }

  void Heatmap::IncrementMapCounterByAmount(HeatmapCoordinate coords, std::string& counter_key, int add_amount)
  {
    CoordinatesMap* map_for_counter = getOrAddMapForCounter(counter_key);
    HeatmapCoordinate adjustedCoords = AdjustCoordsToSpatialResolution(coords);
    map_for_counter->AddAmountAt((int)adjustedCoords.x, (int)adjustedCoords.y, add_amount);
  }

  void Heatmap::IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, std::string counter_keys[], int amounts[], int counter_keys_length)
  {
    for (int i = 0; i < counter_keys_length; i++)
    {
      IncrementMapCounterByAmount(coords, counter_keys[i], amounts[i]);
    }
  }

  bool Heatmap::SerializeHeatmap(char* &out_buffer, int &out_length)
  {
    // First we set a boost stream writting to a std::string
    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> buffer_destination(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > stream(buffer_destination);
    boost::archive::binary_oarchive oa(stream);

    // We write the class values to the string, through the stream
    oa << single_unit_width_;
    oa << single_unit_height_;
    oa << amount_of_counters_;

    for (int i = 0; i < amount_of_counters_; i++)
    {
      oa & *(counter_glossary_[i].counter_key);
      oa & counter_glossary_[i].counter_map;
    }

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

  bool Heatmap::DeserializeHeatmap(const char* &in_buffer, int in_length)
  {
    DestroyHeatmap();
    
    // wrap buffer inside a stream for deserialization
    boost::iostreams::basic_array_source<char> buffer_source(in_buffer, in_length);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > stream(buffer_source);
    boost::archive::binary_iarchive ia(stream);

    ia >> single_unit_width_;
    ia >> single_unit_height_;
    ia >> amount_of_counters_;

    counter_glossary_ = new CounterKeyValue[amount_of_counters_];
    std::string temp_string;
    for (int i = 0; i < amount_of_counters_; i++)
    {
      ia & temp_string;
      counter_glossary_[i].counter_key = new std::string(temp_string);
      ia & counter_glossary_[i].counter_map;
    }
    return true;
  }

  // LIB TEST
  double Heatmap::Add(double a, double b)
  {
    return a + b;
  }

  unsigned int Heatmap::getCounterAtPosition(double coord_x, double coord_y, std::string &counter_key)
  {
    return getCounterAtPosition({ coord_x, coord_y }, counter_key);
  }

  unsigned int Heatmap::getCounterAtPosition(HeatmapCoordinate coords, std::string &counter_key)
  {
    if (!ContainsCounterForKey(counter_key))
      return 0;

    CoordinatesMap* map_for_counter = getOrAddMapForCounter(counter_key);
    HeatmapCoordinate adjusted_coords = AdjustCoordsToSpatialResolution(coords);
    return map_for_counter->getValueAt((int)adjusted_coords.x, (int)adjusted_coords.y);
  }

  bool Heatmap::getCounterDataInsideRect(double lower_coord_x, double lower_coord_y, double upper_coord_x, double upper_coord_y, std::string &counter_key, HeatmapData &out_data)
  {
    return getCounterDataInsideRect({ lower_coord_x, lower_coord_y }, { upper_coord_x, upper_coord_y }, counter_key, out_data);
  }

  bool Heatmap::getCounterDataInsideRect(HeatmapCoordinate lower_left, HeatmapCoordinate upper_right, std::string &counter_key, HeatmapData &out_data)
  {
    if (!ContainsCounterForKey(counter_key))
      return false;

    CoordinatesMap* map_for_counter = getOrAddMapForCounter(counter_key);

    HeatmapCoordinate adjusted_lower_left = AdjustCoordsToSpatialResolution(lower_left);
    HeatmapCoordinate adjusted_upper_right = AdjustCoordsToSpatialResolution(upper_right);
    int width = (int)adjusted_upper_right.x - (int)adjusted_lower_left.x + 1;
    int height = (int)adjusted_upper_right.y - (int)adjusted_lower_left.y + 1;

    out_data.counter_name = std::string(counter_key);
    out_data.lower_left_coordinate = adjusted_lower_left;
    out_data.spatial_resolution = { single_unit_width_, single_unit_height_ };
    out_data.data_size = { width, height };
    out_data.heatmap_data = new uint32_t*[width];
    for (int i = 0; i < width; i++)
    {
      out_data.heatmap_data[i] = new uint32_t[height]();
    }

    for (int x = 0; x < width; x++)
    {
      for (int y = 0; y < height; y++)
      {
        out_data.heatmap_data[x][y] = map_for_counter->getValueAt((int)adjusted_lower_left.x + x, (int)adjusted_lower_left.y + y);
      }
    }

    return true;
  }

  bool Heatmap::getAllCounterData(std::string &counter_key, HeatmapData &out_data)
  {
    if (!ContainsCounterForKey(counter_key))
      return false;

    CoordinatesMap* map_for_counter = getOrAddMapForCounter(counter_key);

    return getCounterDataInsideRect(map_for_counter->lowest_coord_x(), map_for_counter->lowest_coord_y(),
      map_for_counter->highest_coord_x(), map_for_counter->highest_coord_y(), counter_key, out_data);
  }


  /// --------- Private ---------
  HeatmapCoordinate Heatmap::AdjustCoordsToSpatialResolution(HeatmapCoordinate coords)
  {
    HeatmapCoordinate adjusted_coords;
    adjusted_coords.x = floor(coords.x / single_unit_width_);
    adjusted_coords.y = floor(coords.y / single_unit_height_);
    return adjusted_coords;
  }

  CounterKeyValue Heatmap::addNewCounter(std::string &counter_key)
  {
    CounterKeyValue *old_glossary = counter_glossary_;
    counter_glossary_ = new CounterKeyValue[++amount_of_counters_];

    for (int i = 0; i < amount_of_counters_ - 1; i++)
    {
      counter_glossary_[i] = old_glossary[i];
    }
    delete(old_glossary);

    CounterKeyValue newCounterMap;
    newCounterMap.counter_key = new std::string(counter_key);
    newCounterMap.counter_map = new CoordinatesMap();
    counter_glossary_[amount_of_counters_ - 1] = newCounterMap;
    return newCounterMap;
  }

  CoordinatesMap* Heatmap::getOrAddMapForCounter(std::string &counter_key)
  {
    for (int i = 0; i < amount_of_counters_; i++)
    {
      if (counter_glossary_[i].counter_key->compare(counter_key) == 0)
        return counter_glossary_[i].counter_map;
    }

    return addNewCounter(counter_key).counter_map;
  }

  bool Heatmap::ContainsCounterForKey(std::string &counter_key)
  {
    for (int i = 0; i < amount_of_counters_; i++)
    {
      if (counter_glossary_[i].counter_key->compare(counter_key) == 0)
        return true;
    }

    return false;
  }

  void Heatmap::DestroyHeatmap()
  {
    for (int i = 0; i < amount_of_counters_; i++)
    {
      delete(counter_glossary_[i].counter_key);
      delete(counter_glossary_[i].counter_map);
    }

    delete(counter_glossary_);
    counter_glossary_ = NULL;
  }
}