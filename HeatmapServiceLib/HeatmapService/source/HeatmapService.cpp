////////////////////////////////////////////////////////////////////////
// HeatmapService.cpp: API for use of the HeatmapService Library
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////

#include "HeatmapService.h"
#include "HeatmapPrivate.h"

namespace heatmap_service
{
  // Implementation of the HeatmapService class serves only to pass the correct arguments to the inner private Heatmap

  // Allocates the private pointer with the respective arguments
  HeatmapService::HeatmapService() : private_heatmap_(new HeatmapPrivate()){}
  HeatmapService::HeatmapService(double smallest_spatial_unit_size) : private_heatmap_(new HeatmapPrivate(smallest_spatial_unit_size)){}
  HeatmapService::HeatmapService(double smallest_spatial_unit_width, 
                                 double smallest_spatial_unit_height) : private_heatmap_(new HeatmapPrivate(smallest_spatial_unit_width, smallest_spatial_unit_height)){}
  HeatmapService::HeatmapService(const HeatmapService& copy) : private_heatmap_(new HeatmapPrivate(*copy.private_heatmap_)){}

  HeatmapService& HeatmapService::operator=(const HeatmapService& copy)
  {
    if (this != &copy)
    {
      delete(private_heatmap_);
      private_heatmap_ = new HeatmapPrivate(*copy.private_heatmap_);
    }
    return *this;
  }

  HeatmapService::~HeatmapService()
  {
    delete(private_heatmap_);
  }

  // -- Getters for the current spatial resolution
  double HeatmapService::single_unit_height() const
  {
    return private_heatmap_->single_unit_height();
  }

  double HeatmapService::single_unit_width() const
  {
    return private_heatmap_->single_unit_width();
  }

  // -- Counter queries
  bool HeatmapService::hasMapForCounter(const std::string &counter_key) const
  {
    return private_heatmap_->hasMapForCounter(counter_key);
  }

  // -- Heatmap activity logging methods
  bool HeatmapService::IncrementMapCounter(HeatmapCoordinate coords, const std::string &counter_key)
  {
    return private_heatmap_->IncrementMapCounter(coords, counter_key);
  }

  bool HeatmapService::IncrementMapCounterByAmount(HeatmapCoordinate coords, const std::string &counter_key, int add_amount)
  {
    return private_heatmap_->IncrementMapCounterByAmount(coords, counter_key, add_amount);
  }

  bool HeatmapService::IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, const std::string counter_keys[], int amounts[], int counter_keys_length)
  {
    return private_heatmap_->IncrementMultipleMapCountersByAmount(coords, counter_keys, amounts, counter_keys_length);
  }

  // -- Heatmap query methods
  unsigned int HeatmapService::getCounterAtPosition(HeatmapCoordinate coords, const std::string &counter_key) const
  {
    return private_heatmap_->getCounterAtPosition(coords, counter_key);
  }

  bool HeatmapService::getCounterDataInsideRect(HeatmapCoordinate lower_left, HeatmapCoordinate upper_right, const std::string &counter_key, HeatmapData &out_data) const
  {
    return private_heatmap_->getCounterDataInsideRect(lower_left, upper_right, counter_key, out_data);
  }

  bool HeatmapService::getAllCounterData(const std::string &counter_key, HeatmapData &out_data) const
  {
    return private_heatmap_->getAllCounterData(counter_key, out_data);
  }

  // -- Heatmap serialization
  bool HeatmapService::SerializeHeatmap(char* &out_buffer, int &out_length) const
  {
    return private_heatmap_->SerializeHeatmap(out_buffer, out_length);
  }

  bool HeatmapService::DeserializeHeatmap(const char* &in_buffer, int in_length)
  {
    return private_heatmap_->DeserializeHeatmap(in_buffer, in_length);
  }
}