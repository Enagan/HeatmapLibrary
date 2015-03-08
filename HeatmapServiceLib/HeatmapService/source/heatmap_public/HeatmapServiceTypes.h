////////////////////////////////////////////////////////////////////////
// HeatmapServiceTypes.h: Data types used by the HeatmapService API 
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////////

#pragma once
namespace heatmap_service
{
  // Heatmap Service Types contains public structs used by the HeatmapService library API, to be used when calling its methods

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

  // Return data structure for area queries. Contains the name of the counter queried, The lower left point of the area queried and it's size
  // And a Matrix of unsigned ints containing the actual data from the query
  struct HeatmapData
  {
    std::string* counter_name;

    HeatmapCoordinate lower_left_coordinate;
    HeatmapSize spatial_resolution;

    HeatmapSize data_size;
    unsigned int **heatmap_data;
  };
}