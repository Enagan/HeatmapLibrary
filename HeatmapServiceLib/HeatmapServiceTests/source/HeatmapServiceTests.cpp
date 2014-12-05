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
  
  heatmap_service::HeatmapService* heatmap = new heatmap_service::HeatmapService();

  for (long int i = 0; i < 1000000000; i++)
  {
    heatmap->IncrementMapCounter({ rand() % 5000, rand() % 5000 }, std::string("deaths"));
  }

  cout << "a + b = " << heatmap->getCounterAtPosition(2, 5, std::string("lives")) << endl;
    

  clock_t end = clock();
  float diff((float)end - (float)init);
  cout << endl << "---------------------------------------" << endl << 
    "..... took " << (diff / CLOCKS_PER_SEC) << " seconds ....." << endl <<
    "... Press Enter to close ..." << endl;

  delete(heatmap);

  cin.get();

	return 0;
}

