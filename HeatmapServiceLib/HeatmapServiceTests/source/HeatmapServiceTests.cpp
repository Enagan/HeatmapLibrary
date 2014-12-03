// HeatmapServiceTests.cpp : Defines the entry point for the console application.


#include <iostream>
#include "HeatmapService.h"

using namespace std;

int main(int argc, char* argv[])
{
  double a = 3;
  double b = 2;

  cout << "a + b = " <<
    HeatmapServiceLib::Heatmap::Add(a, b) << endl;


  cout << endl << "... Press Enter to close ..." << endl;
  cin.get();

	return 0;
}

