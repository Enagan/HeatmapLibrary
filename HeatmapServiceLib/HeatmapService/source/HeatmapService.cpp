#include "HeatmapService.h"
#include "HeatmapPrivate.h"

namespace heatmap_service
{
  // Implementation of the HeatmapService class serves only to pass the correct arguments to the inner private Heatmap

  // Allocates the smart pointer with the respective arguments
  HeatmapService::HeatmapService() : private_heatmap_(new Heatmap()){}
  HeatmapService::HeatmapService(double smallest_spatial_unit_size) : private_heatmap_(new Heatmap(smallest_spatial_unit_size)){}
  HeatmapService::HeatmapService(double smallest_spatial_unit_width, 
                                 double smallest_spatial_unit_height) : private_heatmap_(new Heatmap(smallest_spatial_unit_width, smallest_spatial_unit_height)){}
  HeatmapService::HeatmapService(HeatmapSize smallest_spatial_unit_size) : private_heatmap_(new Heatmap(smallest_spatial_unit_size.width, smallest_spatial_unit_size.height)){}

  // The smart pointer will deallocate itself as the HeatmapService class is destroyed
  HeatmapService::~HeatmapService(){}

  // -- Getters for the current spatial resolution
  double HeatmapService::single_unit_height()
  {
    return private_heatmap_->single_unit_height();
  }

  double HeatmapService::single_unit_width()
  {
    return private_heatmap_->single_unit_width();
  }

  // -- Counter queries
  bool HeatmapService::hasMapForCounter(const char* &counter_key)
  {
    return private_heatmap_->hasMapForCounter(counter_key);
  }
  bool HeatmapService::hasMapForCounter(const std::string &counter_key)
  {
    return private_heatmap_->hasMapForCounter(counter_key);
  }

  // -- Heatmap activity logging methods
  bool HeatmapService::IncrementMapCounter(double coord_x, double coord_y, const char* &counter_key)
  {
    return private_heatmap_->IncrementMapCounter(coord_x, coord_y, counter_key);
  }
  bool HeatmapService::IncrementMapCounter(double coord_x, double coord_y, const std::string &counter_key)
  {
    return private_heatmap_->IncrementMapCounter(coord_x, coord_y, counter_key);
  }

  bool HeatmapService::IncrementMapCounter(HeatmapCoordinate coords, const char* &counter_key)
  {
    return private_heatmap_->IncrementMapCounter(coords, counter_key);
  }
  bool HeatmapService::IncrementMapCounter(HeatmapCoordinate coords, const std::string &counter_key)
  {
    return private_heatmap_->IncrementMapCounter(coords, counter_key);
  }

  bool HeatmapService::IncrementMapCounterByAmount(double coord_x, double coord_y, const char* &counter_key, unsigned int add_amount)
  {
    return private_heatmap_->IncrementMapCounterByAmount(coord_x, coord_y, counter_key, add_amount);
  }
  bool HeatmapService::IncrementMapCounterByAmount(double coord_x, double coord_y, const std::string &counter_key, unsigned int add_amount)
  {
    return private_heatmap_->IncrementMapCounterByAmount(coord_x, coord_y, counter_key, add_amount);
  }

  bool HeatmapService::IncrementMapCounterByAmount(HeatmapCoordinate coords, const char* &counter_key, unsigned int add_amount)
  {
    return private_heatmap_->IncrementMapCounterByAmount(coords, counter_key, add_amount);
  }
  bool HeatmapService::IncrementMapCounterByAmount(HeatmapCoordinate coords, const std::string &counter_key, unsigned int add_amount)
  {
    return private_heatmap_->IncrementMapCounterByAmount(coords, counter_key, add_amount);
  }

  bool HeatmapService::IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, const char* counter_keys[], unsigned int amounts[], int counter_keys_length)
  {
    return private_heatmap_->IncrementMultipleMapCountersByAmount(coords, counter_keys, amounts, counter_keys_length);
  }

  // -- Heatmap query methods
  unsigned int HeatmapService::getCounterAtPosition(double coord_x, double coord_y, const std::string &counter_key)
  {
    return private_heatmap_->getCounterAtPosition(coord_x, coord_y, counter_key);
  }

  unsigned int HeatmapService::getCounterAtPosition(HeatmapCoordinate coords, const std::string &counter_key)
  {
    return private_heatmap_->getCounterAtPosition(coords, counter_key);
  }

  bool HeatmapService::getCounterDataInsideRect(double lower_coord_x, double lower_coord_y, double upper_coord_x, double upper_coord_y, const std::string &counter_key, HeatmapData &out_data)
  {
    return private_heatmap_->getCounterDataInsideRect(lower_coord_x, lower_coord_y, upper_coord_x, upper_coord_y, counter_key, out_data);
  }

  bool HeatmapService::getCounterDataInsideRect(HeatmapCoordinate lower_left, HeatmapCoordinate upper_right, const std::string &counter_key, HeatmapData &out_data)
  {
    return private_heatmap_->getCounterDataInsideRect(lower_left, upper_right, counter_key, out_data);
  }

  bool HeatmapService::getAllCounterData(const std::string &counter_key, HeatmapData &out_data)
  {
    return private_heatmap_->getAllCounterData(counter_key, out_data);
  }

  // -- Heatmap serialization
  bool HeatmapService::SerializeHeatmap(char* &out_buffer, int &out_length)
  {
    return private_heatmap_->SerializeHeatmap(out_buffer, out_length);
  }

  bool HeatmapService::DeserializeHeatmap(const char* &in_buffer, int in_length)
  {
    return private_heatmap_->DeserializeHeatmap(in_buffer, in_length);
  }
}