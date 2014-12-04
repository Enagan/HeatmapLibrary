#include "HeatmapPrivate.h"

namespace heatmap_service
{
  Heatmap::Heatmap() : single_unit_width_(1), single_unit_height_(1), amount_of_counters_(0)
  {
    counter_glossary_ = NULL;
  }

  Heatmap::Heatmap(double smallest_spatial_unit_size) : single_unit_width_(smallest_spatial_unit_size), single_unit_height_(smallest_spatial_unit_size), amount_of_counters_(0)
  {
    counter_glossary_ = NULL;
  }

  Heatmap::Heatmap(double smallest_spatial_unit_width,
    double smallest_spatial_unit_height) : single_unit_width_(smallest_spatial_unit_width), single_unit_height_(smallest_spatial_unit_height), amount_of_counters_(0)
  {
    counter_glossary_ = NULL;
  }

  Heatmap::~Heatmap()
  {
    for (int i = 0; i < amount_of_counters_; i++)
    {
      delete(counter_glossary_[i].counter_key);
      delete(counter_glossary_[i].counter_map);
    }
    
    delete(counter_glossary_);
  }

  bool Heatmap::hasMapForCounter(std::string &counter_key)
  {
    for (int i = 0; i < amount_of_counters_; i++)
    {
      if (counter_glossary_[i].counter_key->compare(counter_key) == 0)
        return true;
    }
    return false;
  }

  void Heatmap::IncrementMapCounter(double coord_x, double coord_y, std::string &counter_key)
  {
    IncrementMapCounter({ coord_x, coord_y }, counter_key);
  }

  void Heatmap::IncrementMapCounter(HeatmapCoordinate coords, std::string &counter_key)
  {
    CoordinatesMap* map_for_counter = getOrAddMapForCounter(counter_key);
    map_for_counter->IncrementValueAt(coords);
  }

  void Heatmap::IncrementMapCounterByAmount(double coord_x, double coord_y, std::string &counter_key, int add_amount)
  {
    IncrementMapCounterByAmount({ coord_x, coord_y }, counter_key, add_amount);
  }

  void Heatmap::IncrementMapCounterByAmount(HeatmapCoordinate coords, std::string &counter_key, int add_amount)
  {
    CoordinatesMap* map_for_counter = getOrAddMapForCounter(counter_key);
    map_for_counter->AddAmountAt(coords, add_amount);
  }

  void Heatmap::IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, std::string counter_keys[], int amounts[], int counter_keys_length)
  {
    for (int i = 0; i < counter_keys_length; i++)
    {
      IncrementMapCounterByAmount(coords, counter_keys[i], amounts[i]);
    }
  }

  bool Heatmap::SerializeHeatmap(char* &out_buffer)
  {
    return true;
  }

  bool Heatmap::DeserializeHeatmap(char* &in_buffer)
  {
    return true;
  }

  // LIB TEST
  double Heatmap::Add(double a, double b)
  {
    return a + b;
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
}