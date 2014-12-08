////////////////////////////////////////////////////////////////////
// CoordinatesMap.h: Declaration of CoordinatesMap helper class. 
// Contains data for a specific counter
// Includes Boost libraries to serialize itself
// Written by: Pedro Engana (http://pedroengana.com) 
////////////////////////////////////////////////////////////////////

#pragma once
#include <cstdint>
#include <boost\serialization\access.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\archive\binary_iarchive.hpp>

namespace heatmap_service
{

  struct Column
  {
    int col_length;
    int col_negative_coord_padding;
    uint32_t* column;
  };


  class CoordinatesMap
  {
  private:
    const int kInitialMapSize = 1;
    const int kInitialNegativeCoordsPadding = 0;

    int map_columns_length_;
    int map_columns_negative_coord_padding;
    Column* map_columns;

    int lowest_coord_x_;
    int highest_coord_x_;
    int lowest_coord_y_;
    int highest_coord_y_;
  public:
    CoordinatesMap();
    ~CoordinatesMap();

    void IncrementValueAt(int coord_x, int coord_y);
    void AddAmountAt(int coord_x, int coord_y, int amount);

    uint32_t getValueAt(int coord_x, int coord_y);
    int lowest_coord_x();
    int highest_coord_x();
    int lowest_coord_y();
    int highest_coord_y();

    void ClearMap();

  private:
    void InitializeMap();
    void DestroyMap();

    void AddColumns();
    void InsertColumnsAtBeggining();

    void GrowColumnAtEnd(unsigned int adjusted_coord_x);
    void GrowColumnAtBeggining(unsigned int adjusted_coord_x);

    void CheckIfNewBoundary(int coord_x, int coord_y);

    // Boost serialization method
    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
      ar & map_columns_length_;
      ar & map_columns_negative_coord_padding;
      ar & lowest_coord_x_;
      ar & lowest_coord_y_;
      ar & highest_coord_x_;
      ar & highest_coord_y_;

      for (int i = 0; i < map_columns_length_; i++)
      {
        ar & map_columns[i].col_length;
        ar & map_columns[i].col_negative_coord_padding;

        for (int n = 0; n < map_columns[i].col_length; n++)
        {
          ar & map_columns[i].column[n];
        }
      }
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
      DestroyMap();

      ar & map_columns_length_;
      ar & map_columns_negative_coord_padding;
      ar & lowest_coord_x_;
      ar & lowest_coord_y_;
      ar & highest_coord_x_;
      ar & highest_coord_y_;

      map_columns = new Column[map_columns_length_];
      for (int i = 0; i < map_columns_length_; i++)
      {
        ar & map_columns[i].col_length;
        ar & map_columns[i].col_negative_coord_padding;

        map_columns[i].column = new uint32_t[map_columns[i].col_length]();
        for (int n = 0; n < map_columns[i].col_length; n++)
        {
          ar & map_columns[i].column[n];
        }
      }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
  };
}