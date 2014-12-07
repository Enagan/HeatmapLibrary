#include "HeatmapService.h"
#include "HeatmapPrivate.h"

namespace heatmap_service
{
  HeatmapService::HeatmapService() : pimpl_(new Heatmap()){}
  HeatmapService::HeatmapService(double smallest_spatial_unit_size) : pimpl_(new Heatmap(smallest_spatial_unit_size)){}
  HeatmapService::HeatmapService(double smallest_spatial_unit_width, 
                                 double smallest_spatial_unit_height) : pimpl_(new Heatmap(smallest_spatial_unit_width, smallest_spatial_unit_height)){}
  HeatmapService::~HeatmapService(){}

  void HeatmapService::IncrementMapCounter(double coord_x, double coord_y, std::string &counter_key)
  {
    pimpl_->IncrementMapCounter(coord_x, coord_y, counter_key);
  }

  void HeatmapService::IncrementMapCounter(HeatmapCoordinate coords, std::string &counter_key)
  {
    pimpl_->IncrementMapCounter(coords, counter_key);
  }

  void HeatmapService::IncrementMapCounterByAmount(double coord_x, double coord_y, std::string &counter_key, int add_amount)
  {
    pimpl_->IncrementMapCounterByAmount(coord_x, coord_y, counter_key, add_amount);
  }

  void HeatmapService::IncrementMapCounterByAmount(HeatmapCoordinate coords, std::string &counter_key, int add_amount)
  {
    pimpl_->IncrementMapCounterByAmount(coords, counter_key, add_amount);
  }

  void HeatmapService::IncrementMultipleMapCountersByAmount(HeatmapCoordinate coords, std::string counter_keys[], int amounts[], int counter_keys_length)
  {
    pimpl_->IncrementMultipleMapCountersByAmount(coords, counter_keys, amounts, counter_keys_length);
  }

  bool HeatmapService::SerializeHeatmap(char* &out_buffer)
  {
    return pimpl_->SerializeHeatmap(out_buffer);
  }
  bool HeatmapService::DeserializeHeatmap(char* &in_buffer)
  {
    return pimpl_->DeserializeHeatmap(in_buffer);
  }

    // LIB TEST
  double HeatmapService::Add(double a, double b)
  {
    return pimpl_->Add(a,b);
  }

  unsigned int HeatmapService::getCounterAtPosition(double coord_x, double coord_y, std::string &counter_key)
  {
    return pimpl_->getCounterAtPosition(coord_x, coord_y, counter_key);
  }

  unsigned int HeatmapService::getCounterAtPosition(HeatmapCoordinate coords, std::string &counter_key)
  {
    return pimpl_->getCounterAtPosition(coords, counter_key);
  }

  bool HeatmapService::getCounterDataInsideRect(double lower_coord_x, double lower_coord_y, double upper_coord_x, double upper_coord_y, std::string &counter_key, HeatmapData &out_data)
  {
    return pimpl_->getCounterDataInsideRect(lower_coord_x, lower_coord_y, upper_coord_x, upper_coord_y, counter_key, out_data);
  }

  bool HeatmapService::getCounterDataInsideRect(HeatmapCoordinate lower_left, HeatmapCoordinate upper_right, std::string &counter_key, HeatmapData &out_data)
  {
    return pimpl_->getCounterDataInsideRect(lower_left, upper_right, counter_key, out_data);
  }

  bool HeatmapService::getAllCounterData(std::string &counter_key, HeatmapData &out_data)
  {
    return pimpl_->getAllCounterData(counter_key, out_data);
  }
}