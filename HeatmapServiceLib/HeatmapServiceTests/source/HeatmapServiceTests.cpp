// HeatmapServiceTests.cpp : Defines the entry point for the console application.
#include <iostream>
#include <ctime>
#include "HeatmapService.h"

using namespace std;

int main(int argc, char* argv[])
{
  double a = 3;
  double b = 2;

  clock_t init = clock();
  
  heatmap_service::HeatmapService* heatmap = new heatmap_service::HeatmapService(10,10);

  for (long int i = 0; i < 1000000; i++)
  {
    int randX = rand() % 10000 - 5000;
    int randY = rand() % 10000 - 5000;
    //cout << "Registerng { " << randX << " , " << randY << " }" << endl;
    heatmap->IncrementMapCounter({ randX, randY }, std::string("deaths"));
  }
  
  cout << "a + b = " << heatmap->getCounterAtPosition(1, 1, std::string("deaths")) << endl;
    

  clock_t end = clock();
  float diff((float)end - (float)init);
  cout << endl << "---------------------------------------" << endl << 
    "..... took " << (diff / CLOCKS_PER_SEC) << " seconds ....." << endl <<
    "... Press Enter to close ..." << endl;

  delete(heatmap);

  cin.get();

	return 0;
}

