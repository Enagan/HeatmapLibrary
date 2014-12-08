////////////////////////////////////////////////////////////////////////
// HeatmapServiceTypes.h: Data types used by the HeatmapService API 
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////

#pragma once
namespace heatmap_service
{
  struct HeatmapCoordinate
  {
    double x;
    double y;
  };

  struct HeatmapSize
  {
    double width;
    double height;
  };

  struct HeatmapData
  {
    std::string counter_name;

    HeatmapCoordinate lower_left_coordinate;
    HeatmapSize spatial_resolution;

    HeatmapSize data_size;
    unsigned int **heatmap_data;
  };
}