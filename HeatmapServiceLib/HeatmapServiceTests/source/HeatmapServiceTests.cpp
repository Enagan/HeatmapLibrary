// HeatmapServiceTests.cpp : Defines the entry point for the console application.
#include <iostream>
#include "HeatmapService.h"

using namespace std;

int main(int argc, char* argv[])
{
  double a = 3;
  double b = 2;
  heatmap_service::HeatmapService* heatmap = new heatmap_service::HeatmapService();
  cout << "a + b = " << heatmap->Add(a,b) << endl;
    
  delete(heatmap);

  cout << endl << "... Press Enter to close ..." << endl;
  cin.get();

	return 0;
}

