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
      cout << " " << data.heatmap_data[x][y];
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
  
  heatmap_service::HeatmapService* heatmap = new heatmap_service::HeatmapService(0.5);

  for (long int i = 0; i < 1000000; i++)
  {
    int randX = rand() % 10000 - 5000;
    int randY = rand() % 10000 - 5000;
    //cout << "Registerng { " << randX << " , " << randY << " }" << endl;
    heatmap->IncrementMapCounter({ randX, randY }, std::string("deaths"));
  }
 

  heatmap_service::HeatmapData data;

  if (heatmap->getCounterDataInsideRect({ 0, 0 }, { 100, 100 }, std::string("deaths"), data))
  {
    DumpHeatmapData(data);
  }
  //cout << "a + b = " << heatmap->getCounterAtPosition(1, 0, std::string("deaths")) << endl;
    

  clock_t end = clock();
  float diff((float)end - (float)init);
  cout << endl << "---------------------------------------" << endl << 
    "..... took " << (diff / CLOCKS_PER_SEC) << " seconds ....." << endl <<
    "... Press Enter to close ..." << endl;

  delete(heatmap);

  cin.get();

	return 0;
}

