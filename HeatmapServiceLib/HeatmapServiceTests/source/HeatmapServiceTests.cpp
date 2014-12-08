// HeatmapServiceTests.cpp : Defines the entry point for the console application.
#include <iostream>
#include <ctime>
#include "HeatmapService.h"


using namespace std;

void DumpHeatmapData(heatmap_service::HeatmapData &data)
{
  cout << endl << "---------------------------------------" << endl << endl;
  for (int y = data.data_size.height - 1; y >= 0; y--)
  {
    cout << (data.lower_left_coordinate.y + y)*data.spatial_resolution.height << "::";

    for (int x = 0; x < data.data_size.width; x++)
    {
      cout << "  " << data.heatmap_data[x][y];
    }
    cout << endl;
  }
  cout << endl << "---------------------------------------" << endl;
}

int main(int argc, char* argv[])
{
  double a = 3;
  double b = 2;

  clock_t init = clock();
  
  heatmap_service::HeatmapService* heatmap = new heatmap_service::HeatmapService(1);
  heatmap_service::HeatmapService* heatmap_2 = new heatmap_service::HeatmapService(1);

  heatmap->IncrementMapCounter({ 0, 0 }, "deaths");
  heatmap->IncrementMapCounter({ 0, 0 }, "deaths");
  heatmap->IncrementMapCounter({ -1, 0 }, "deaths");
  heatmap->IncrementMapCounter({ -1, -1 }, "deaths");

  heatmap->IncrementMapCounter({ 1, 0 }, "deaths");
  heatmap->IncrementMapCounter({ 1, 0 }, "deaths");

  heatmap->IncrementMapCounter({ 1, 2 }, "deaths");

  char* serialized_hm1; 
  const char* const_serialized;
  int buffer_size;

  if (heatmap->SerializeHeatmap(serialized_hm1, buffer_size))
  {
    delete(heatmap);
    const_serialized = serialized_hm1;
    //cout << serialized_hm1[0] << endl;
    heatmap_2->DeserializeHeatmap(const_serialized, buffer_size);
  }

  cout << "Results are : " << heatmap_2->single_unit_width() << " , " << heatmap_2->single_unit_height() << endl;
  cout << "probing deaths at 0,0 : " << heatmap_2->getCounterAtPosition({ 1, 2 },std::string("deaths")) << endl;
  /*for (long int i = 0; i < 1000000; i++)
  {
    int randX = rand() % 10000 - 5000;
    int randY = rand() % 10000 - 5000;
    //cout << "Registering { " << randX << " , " << randY << " }" << endl;
    heatmap->IncrementMapCounter({ randX, randY }, std::string("deaths"));
  }*/
 

  heatmap_service::HeatmapData data;

  if (heatmap_2->getAllCounterData(std::string("deaths"), data))
  {
    DumpHeatmapData(data);
  }
  //cout << "a + b = " << heatmap->getCounterAtPosition(1, 0, std::string("deaths")) << endl;
    

  clock_t end = clock();
  float diff((float)end - (float)init);
  cout << endl << "---------------------------------------" << endl << 
    "..... took " << (diff / CLOCKS_PER_SEC) << " seconds ....." << endl <<
    "... Press Enter to close ..." << endl;

  delete(heatmap_2);

  cin.get();

	return 0;
}

