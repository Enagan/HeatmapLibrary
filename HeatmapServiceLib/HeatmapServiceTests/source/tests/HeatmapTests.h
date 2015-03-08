//////////////////////////////////////////////////////////////////////////////////////
// HeatmapTests.h: Test functions for the HeatmapService
// Written by: Pedro Engana (http://pedroengana.com) 
//////////////////////////////////////////////////////////////////////////////////////

void TestHeatmapAll();

bool TestSimpleRegisterRead();
bool TestRegisterReadMultiple();
bool TestRegisterReadMultipleNonDefaultResolution();
bool TestRegisterReadMultipleCounters();
bool TestAllRegisteringMethods();

bool TestSimpleGetArea();
bool TestGetAreaUnitSizedRect();
bool TestGetAreaUpperLowerSwitched();
bool TestSimpleGetEntireArea();

bool TestSimpleSerializeDeserialize();
bool TestDeserializeIntoFilledHeatmap();
bool TestInvalidBufferForDeserialization();