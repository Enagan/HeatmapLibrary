//////////////////////////////////////////////////////////////////////////////////////
// HeatmapServiceTests.cpp: Rough console application to test HeatmapServiceLib
// Written by: Pedro Engana (http://pedroengana.com) 
//////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <ctime>
#include "HeatmapService.h"

#include "SignedIndexVectorTests.h"
#include "SimpleHashmapTests.h"
#include "HeatmapTests.h"
#include "HeatmapStressTests.h"


using namespace std;
using namespace heatmap_service;

int main(int argc, char* argv[])
{
  TestSIVAll();
  TestHashMapAll();
  TestHeatmapAll();
  HeatmapStressTestAll();

  cout << endl << "... Tests Finished, Press Enter to close ..." << endl;
  cin.get();

	return 0;
}