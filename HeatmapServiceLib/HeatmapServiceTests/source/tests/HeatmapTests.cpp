#pragma once

#include "HeatmapService.h"
#include "HeatmapTests.h"
#include <iostream>

using namespace std;
using namespace heatmap_service;

const string kDeathsCounterKey = "deaths";
const string kGoldObtainedCounterKey = "gold_obtained";
const string kExperienceGainedCounterKey = "xp_gained";
const string kKillsCounterKey = "kills";
const string kSkillsUsedKey = "skills_used";
const string kDodgesKey = "dodge_rolls";

void TestHeatmapAll()
{
  cout << "######## Heatmap Tests ########" << endl << endl;

  cout << "TestSimpleRegisterRead: [" << (TestSimpleRegisterRead() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestRegisterReadMultiple: [" << (TestRegisterReadMultiple() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestRegisterReadMultipleNonDefaultResolution: [" << (TestRegisterReadMultipleNonDefaultResolution() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestRegisterReadMultipleCounters: [" << (TestRegisterReadMultipleCounters() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestAllRegisteringMethods: [" << (TestAllRegisteringMethods() ? "PASSED" : "FAILED") << "]" << endl;

  cout << endl;

  cout << "TestSimpleGetArea: [" << (TestSimpleGetArea() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestGetAreaUnitSizedRect: [" << (TestGetAreaUnitSizedRect() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestGetAreaUpperLowerSwitched: [" << (TestGetAreaUpperLowerSwitched() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestSimpleGetEntireArea: [" << (TestSimpleGetEntireArea() ? "PASSED" : "FAILED") << "]" << endl;

  cout << endl;

  cout << "TestSimpleSerializeDeserialize: [" << (TestSimpleSerializeDeserialize() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestDeserializeIntoFilledHeatmap: [" << (TestDeserializeIntoFilledHeatmap() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestInvalidBufferForDeserialization: [" << (TestInvalidBufferForDeserialization() ? "PASSED" : "FAILED") << "]" << endl;

  cout << endl;
}

bool TestSimpleRegisterRead()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService();
  heatmap.IncrementMapCounter({ 0, 0 }, kDeathsCounterKey);

  return 1 == heatmap.getCounterAtPosition({ 0, 0 }, kDeathsCounterKey);
}

bool TestRegisterReadMultiple()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService();
  heatmap.IncrementMapCounter({ 0, 0 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 1, 1 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 2, 2 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 3, 3 }, kDeathsCounterKey);

  return 1 == heatmap.getCounterAtPosition({ 0, 0 }, kDeathsCounterKey) &&
    1 == heatmap.getCounterAtPosition({ 1, 1 }, kDeathsCounterKey) &&
    1 == heatmap.getCounterAtPosition({ 2, 2 }, kDeathsCounterKey) &&
    1 == heatmap.getCounterAtPosition({ 3, 3 }, kDeathsCounterKey);
}

bool TestRegisterReadMultipleNonDefaultResolution()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService(2, 2);
  heatmap.IncrementMapCounter({ -1, -1 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 0, 0 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 1, 1 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 2, 2 }, kDeathsCounterKey);

  return 1 == heatmap.getCounterAtPosition({ -1, -1 }, kDeathsCounterKey) &&
    2 == heatmap.getCounterAtPosition({ 0, 0 }, kDeathsCounterKey) &&
    2 == heatmap.getCounterAtPosition({ 1, 1 }, kDeathsCounterKey) &&
    1 == heatmap.getCounterAtPosition({ 2, 2 }, kDeathsCounterKey) &&
    0 == heatmap.getCounterAtPosition({ 10, 10 }, kDeathsCounterKey);
}

bool TestRegisterReadMultipleCounters()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService(2, 2);
  heatmap.IncrementMapCounter({ 0, 0 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 0, 0 }, kDodgesKey);
  heatmap.IncrementMapCounter({ 0, 0 }, kExperienceGainedCounterKey);
  heatmap.IncrementMapCounter({ 0, 0 }, kKillsCounterKey);

  return 1 == heatmap.getCounterAtPosition({ 0, 0 }, kDeathsCounterKey) &&
    1 == heatmap.getCounterAtPosition({ 0, 0 }, kDodgesKey) &&
    1 == heatmap.getCounterAtPosition({ 0, 0 }, kExperienceGainedCounterKey) &&
    1 == heatmap.getCounterAtPosition({ 0, 0 }, kKillsCounterKey);
}

bool TestAllRegisteringMethods()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService(2, 2);
  heatmap.IncrementMapCounter({ 0, 0 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 0, 0 }, kDeathsCounterKey);

  heatmap.IncrementMapCounterByAmount({ 0, 0 }, kDeathsCounterKey, 1);
  heatmap.IncrementMapCounterByAmount({ 0, 0 }, kDeathsCounterKey, 1);
  heatmap.IncrementMapCounterByAmount({ 0, 0 }, kDeathsCounterKey, -1);
  heatmap.IncrementMapCounterByAmount({ 0, 0 }, kDeathsCounterKey, -1);

  const std::string counters[3] = { kDeathsCounterKey, kDodgesKey, kGoldObtainedCounterKey };
  int amounts[3] = { 1, 3, 4 };
  heatmap.IncrementMultipleMapCountersByAmount({ 0, 0 }, counters, amounts, 3);

  return 5 == heatmap.getCounterAtPosition({ 0, 0 }, kDeathsCounterKey) &&
    3 == heatmap.getCounterAtPosition({ 0, 0 }, kDodgesKey) &&
    4 == heatmap.getCounterAtPosition({ 0, 0 }, kGoldObtainedCounterKey);
}


bool TestSimpleGetArea()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService(2, 2);
  heatmap.IncrementMapCounter({ -1, -1 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 0, 0 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 1, 1 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 2, 2 }, kDeathsCounterKey);

  heatmap_service::HeatmapData out_data;
  if (!heatmap.getCounterDataInsideRect({ -2, -2 }, { 1, 1 }, kDeathsCounterKey, out_data))
    return false;

  bool result = out_data.counter_name->compare(kDeathsCounterKey) == 0 &&
    out_data.spatial_resolution.width == 2 && out_data.spatial_resolution.height == 2 &&
    out_data.data_size.width == 2 && out_data.data_size.height == 2 &&
    2 == out_data.heatmap_data[1][1];

  delete(out_data.counter_name);
  delete[] out_data.heatmap_data;

  return result;
}

bool TestGetAreaUnitSizedRect()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService(2, 2);
  heatmap.IncrementMapCounter({ -1, -1 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 0, 0 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 1, 1 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 2, 2 }, kDeathsCounterKey);

  heatmap_service::HeatmapData out_data;
  if (!heatmap.getCounterDataInsideRect({ 0, 0 }, { 0, 0 }, kDeathsCounterKey, out_data))
    return false;

  bool result = out_data.counter_name->compare(kDeathsCounterKey) == 0 &&
    out_data.spatial_resolution.width == 2 && out_data.spatial_resolution.height == 2 &&
    out_data.data_size.width == 1 && out_data.data_size.height == 1 &&
    2 == out_data.heatmap_data[0][0];

  delete(out_data.counter_name);
  delete[] out_data.heatmap_data;

  return result;
}

bool TestGetAreaUpperLowerSwitched()
{
  // Should fail at getting data. Invalid rect
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService(2, 2);
  heatmap.IncrementMapCounter({ -1, -1 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 0, 0 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 1, 1 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 2, 2 }, kDeathsCounterKey);

  heatmap_service::HeatmapData out_data;

  bool result = !heatmap.getCounterDataInsideRect({ 2, 2 }, { -2, -2 }, kDeathsCounterKey, out_data);

  if (result == false)
  {
    delete(out_data.counter_name);
    delete[] out_data.heatmap_data;
  }

  return result;
}

bool TestSimpleGetEntireArea()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService(2, 2);
  heatmap.IncrementMapCounter({ -1, -1 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 0, 0 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 1, 1 }, kDeathsCounterKey);
  heatmap.IncrementMapCounter({ 2, 2 }, kDeathsCounterKey);

  heatmap_service::HeatmapData out_data;
  if (!heatmap.getAllCounterData(kDeathsCounterKey, out_data))
    return false;

  bool result = out_data.counter_name->compare(kDeathsCounterKey) == 0 &&
    out_data.spatial_resolution.width == 2 && out_data.spatial_resolution.height == 2 &&
    out_data.data_size.width == 3 && out_data.data_size.height == 3 &&
    2 == out_data.heatmap_data[1][1];

  delete(out_data.counter_name);
  delete[] out_data.heatmap_data;

  return result;
}


bool TestSimpleSerializeDeserialize()
{
  heatmap_service::HeatmapService *heatmap = new heatmap_service::HeatmapService(1, 1);
  heatmap_service::HeatmapService *heatmap_2 = new heatmap_service::HeatmapService(10);

  const std::string counters[3] = { kDeathsCounterKey, kDodgesKey, kGoldObtainedCounterKey };
  int amounts[3] = { 1, 1, 1 };
  heatmap->IncrementMultipleMapCountersByAmount({ 0, 0 }, counters, amounts, 3);
  heatmap->IncrementMultipleMapCountersByAmount({ -1, -1 }, counters, amounts, 3);
  heatmap->IncrementMultipleMapCountersByAmount({ 1, 1 }, counters, amounts, 3);

  char* serialized_hm1;
  int buffer_size;

  if (!heatmap->SerializeHeatmap(serialized_hm1, buffer_size))
  {
    delete(heatmap);
    delete(heatmap_2);
    return false;
  }

  delete(heatmap);
  heatmap = NULL;

  const char* const_serialized_hm1 = serialized_hm1;
  heatmap_2->DeserializeHeatmap(const_serialized_hm1, buffer_size);

  bool result = 1 == heatmap_2->single_unit_height() &&
    1 == heatmap_2->getCounterAtPosition({ 0, 0 }, kDeathsCounterKey) &&
    1 == heatmap_2->getCounterAtPosition({ 0, 0 }, kDodgesKey) &&
    1 == heatmap_2->getCounterAtPosition({ 0, 0 }, kGoldObtainedCounterKey) &&

    1 == heatmap_2->getCounterAtPosition({ -1, -1 }, kDeathsCounterKey) &&
    1 == heatmap_2->getCounterAtPosition({ -1, -1 }, kDodgesKey) &&
    1 == heatmap_2->getCounterAtPosition({ -1, -1 }, kGoldObtainedCounterKey) &&

    1 == heatmap_2->getCounterAtPosition({ 1, 1 }, kDeathsCounterKey) &&
    1 == heatmap_2->getCounterAtPosition({ 1, 1 }, kDodgesKey) &&
    1 == heatmap_2->getCounterAtPosition({ 1, 1 }, kGoldObtainedCounterKey);

  delete(heatmap_2);

  return result;
}

bool TestDeserializeIntoFilledHeatmap()
{
  heatmap_service::HeatmapService *heatmap = new heatmap_service::HeatmapService(1, 1);
  heatmap_service::HeatmapService *heatmap_2 = new heatmap_service::HeatmapService(10);

  const std::string counters[3] = { kDeathsCounterKey, kDodgesKey, kGoldObtainedCounterKey };
  int amounts[3] = { 1, 1, 1 };
  heatmap->IncrementMultipleMapCountersByAmount({ 0, 0 }, counters, amounts, 3);
  heatmap->IncrementMultipleMapCountersByAmount({ -1, -1 }, counters, amounts, 3);
  heatmap->IncrementMultipleMapCountersByAmount({ 1, 1 }, counters, amounts, 3);

  heatmap_2->IncrementMultipleMapCountersByAmount({ 0, 0 }, counters, amounts, 3);
  heatmap_2->IncrementMultipleMapCountersByAmount({ 2, 2 }, counters, amounts, 3);
  heatmap_2->IncrementMultipleMapCountersByAmount({ 1, 1 }, counters, amounts, 3);

  char* serialized_hm1;
  int buffer_size;

  if (!heatmap->SerializeHeatmap(serialized_hm1, buffer_size))
  {
    delete(heatmap);
    delete(heatmap_2);
    return false;
  }

  delete(heatmap);
  heatmap = NULL;

  const char* const_serialized_hm1 = serialized_hm1;
  if (!heatmap_2->DeserializeHeatmap(const_serialized_hm1, buffer_size))
  {
    delete(heatmap);
    delete(heatmap_2);
    return false;
  }

  bool result = 1 == heatmap_2->single_unit_height() &&
    1 == heatmap_2->getCounterAtPosition({ 0, 0 }, kDeathsCounterKey) &&
    1 == heatmap_2->getCounterAtPosition({ 0, 0 }, kDodgesKey) &&
    1 == heatmap_2->getCounterAtPosition({ 0, 0 }, kGoldObtainedCounterKey) &&

    1 == heatmap_2->getCounterAtPosition({ -1, -1 }, kDeathsCounterKey) &&
    1 == heatmap_2->getCounterAtPosition({ -1, -1 }, kDodgesKey) &&
    1 == heatmap_2->getCounterAtPosition({ -1, -1 }, kGoldObtainedCounterKey) &&

    1 == heatmap_2->getCounterAtPosition({ 1, 1 }, kDeathsCounterKey) &&
    1 == heatmap_2->getCounterAtPosition({ 1, 1 }, kDodgesKey) &&
    1 == heatmap_2->getCounterAtPosition({ 1, 1 }, kGoldObtainedCounterKey) &&

    0 == heatmap_2->getCounterAtPosition({ 2, 2 }, kDeathsCounterKey) &&
    0 == heatmap_2->getCounterAtPosition({ 2, 2 }, kDodgesKey) &&
    0 == heatmap_2->getCounterAtPosition({ 2, 2 }, kGoldObtainedCounterKey);

  delete(heatmap_2);

  return result;
}


bool TestInvalidBufferForDeserialization()
{
  heatmap_service::HeatmapService *heatmap = new heatmap_service::HeatmapService(10, 10);

  const char* const_serialized_hm1 = "invalid";
  try
  {
    heatmap->DeserializeHeatmap(const_serialized_hm1, 7);
  }
  // Boost exception should be caught here. Generalized only for testing purposes
  catch (exception &e)
  {

    return true;
  }
  delete(heatmap);
  return false;
}