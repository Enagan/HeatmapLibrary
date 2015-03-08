//////////////////////////////////////////////////////////////////////////////////////
// HeatmapStressTests.h: Test functions to test performance of the Heatmap Service
// Written by: Pedro Engana (http://pedroengana.com) 
//////////////////////////////////////////////////////////////////////////////////////

void HeatmapStressTestAll();

void StressTestMillionRegisters10kper10kCoords();
void StressTestMillionRegisters10per5Coords();
void StressTestMillionRegisters5kper5kOnlyNegativeCoords();
void StressTestThousandRegistriesFractionalResolution();