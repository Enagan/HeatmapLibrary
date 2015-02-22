//////////////////////////////////////////////////////////////////////////////////////
// HeatmapServiceTests.cpp: Rough console application to test HeatmapServiceLib
// Written by: Pedro Engana (http://pedroengana.com) 
//////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <ctime>
#include "HeatmapService.h"
#include "SignedIndexVector.hpp"
#include "HashMap.hpp"


using namespace std;
using namespace heatmap_service;

const string kDeathsCounterKey = "deaths";
const string kGoldObtainedCounterKey = "gold_obtained";
const string kExperienceGainedCounterKey = "xp_gained";
const string kKillsCounterKey = "kills";
const string kSkillsUsedKey = "skills_used";
const string kDodgesKey = "dodge_rolls";

// Forward declaration of test methods

bool TestSIVCreationPrefilledCreation();
bool TestSIVCopyAssignment();
bool TestSIVIterators();
bool TestSIVInsertionAndGetting();
bool TestSIVClearAndClean();

bool TestHashMapCreation();
bool TestHashMapPlacementAndRetrieval();
bool TestHashMapCopyAssignment();
bool TestHashMapCollision();

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

void StressTestMillionRegisters10kper10kCoords();
void StressTestMillionRegisters10per5Coords();
void StressTestMillionRegisters5kper5kOnlyNegativeCoords();
void StressTestThousandRegistriesFractionalResolution();

void DumpHeatmapData(const heatmap_service::HeatmapData &data)
{
  cout << endl << "---------------------------------------" << endl << endl;
  for (int y = (int)data.data_size.height - 1; y >= 0; y--)
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

void RunUnitTests()
{
  cout << "-------------- Unit Tests -----------------" << endl << endl;

  cout << "######## Signed Index Vector ########" << endl << endl;
  cout << "TestSIVCreationPrefilledCreation: [" << (TestSIVCreationPrefilledCreation() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestSIVCopyAssignment: [" << (TestSIVCopyAssignment() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestSIVIterators: [" << (TestSIVIterators() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestSIVInsertionAndGetting: [" << (TestSIVInsertionAndGetting() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestSIVClearAndClean: [" << (TestSIVClearAndClean() ? "PASSED" : "FAILED") << "]" << endl;

  cout << endl;

  cout << "######## Hashmap ########" << endl << endl;
  cout << "TestHashMapCreation: [" << (TestHashMapCreation() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestHashMapPlacementAndRetrieval: [" << (TestHashMapPlacementAndRetrieval() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestHashMapCopyAssignment: [" << (TestHashMapCopyAssignment() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestHashMapCollision: [" << (TestHashMapCollision() ? "PASSED" : "FAILED") << "]" << endl;

  cout << endl;

  cout << "######## Heatmap ########" << endl << endl;
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
}

void RunStressTests()
{
  cout << endl << "-------------- Stress Tests -----------------" << endl;

  cout << endl << "Starting... StressTestMillionRegisters10kper10kCoords";
  StressTestMillionRegisters10kper10kCoords();
  cout << endl << "Starting... StressTestMillionRegisters10per5Coords";
  StressTestMillionRegisters10per5Coords();
  cout << endl << "Starting... StressTestMillionRegisters5kper5kOnlyNegativeCoords";
  StressTestMillionRegisters5kper5kOnlyNegativeCoords();
  cout << endl << "Starting... StressTestThousandRegistriesFractionalResolution";
  StressTestThousandRegistriesFractionalResolution();
}

int main(int argc, char* argv[])
{
  SignedIndexVector<int> testVec;
  testVec[10] = 5;


  //RunUnitTests();
  RunStressTests();
  
  cout << endl << "... Tests Finished, Press Enter to close ..." << endl;
  cin.get();

	return 0;
}

bool TestSIVCreationPrefilledCreation(){
  SignedIndexVector<int> vec;
  SignedIndexVector<int> vec2(20, 0);
  SignedIndexVector<int> vec3(vec);

  return vec.size() == 0 && vec2.size() == 20 && vec2.lowest_index() == -10 && vec3.size() == 0;
}
bool TestSIVCopyAssignment() {
  SignedIndexVector<string> vec(10, "Derp");
  SignedIndexVector<string> vec2(vec);
  SignedIndexVector<string> vec3;
  vec3 = vec;

  return vec.size() == 10 && vec2.size() == 10 && vec3.size() == 10 &&
    vec[0] == "Derp" && vec2[0] == "Derp" && vec3[0] == "Derp";
}
bool TestSIVIterators() {
  SignedIndexVector<int> vec(10, 1);
  int sum = 0;
  for (SignedIndexVector<int>::iterator iter = vec.begin(); iter != vec.end(); ++iter)
    sum += *iter;

  return sum == 10;
}
bool TestSIVInsertionAndGetting() {
  SignedIndexVector<int> vec;

  vec[0] = 0;
  vec[1] = 1; 
  vec.push_back(4);
  vec[-1] = -1; 
  vec[-2] = -2;

  int size = vec.size();
  int alloc_size = vec.allocation_size();

  return (vec[1] + vec[2] + vec[-1] + vec.get_at(-2)) == 2 && size == 5;
}
bool TestSIVClearAndClean() {
  SignedIndexVector<int> vec(10, 1);
  vec.clean();

  return vec.size() == 0;
}

struct StringLenHash{
  int operator()(const std::string& s) { return s.size(); }
};

bool TestHashMapCreation() {
  Hashmap<string, int, StringLenHash> hash1;
  return !hash1.has_key(kDeathsCounterKey);
}
bool TestHashMapPlacementAndRetrieval() {
  Hashmap<string, int, StringLenHash> hash1;

  hash1[kDeathsCounterKey] = 2;
  hash1[kDeathsCounterKey] = 0;

  hash1[kGoldObtainedCounterKey] = 4;
  hash1[kExperienceGainedCounterKey] = 10;
  bool test1 = hash1.has_key(kDeathsCounterKey);
  return hash1[kDeathsCounterKey] == 0 && hash1[kGoldObtainedCounterKey] == 4 && hash1[kExperienceGainedCounterKey] == 10;
}
bool TestHashMapCopyAssignment() {
  Hashmap<string, int, StringLenHash> hash1;
  Hashmap<string, int, StringLenHash> hash2(hash1);
  Hashmap<string, int, StringLenHash> hash3;

  hash1[kDeathsCounterKey] = 2;
  hash1[kDeathsCounterKey] = 0;
  hash3[kDeathsCounterKey] = 20;

  hash1[kGoldObtainedCounterKey] = 4;
  hash1[kExperienceGainedCounterKey] = 10;

  hash3 = hash1;

  Hashmap<string, int, StringLenHash> hash4(hash1);
  bool t1 = !hash2.has_key(kDeathsCounterKey);
  bool t2 = hash3[kDeathsCounterKey] == 0;
  bool t3 = hash4[kExperienceGainedCounterKey] == 10;
  return t1 && t2 && t3;
}
bool TestHashMapCollision(){
  // Ridiculous high collision hash function object
  struct StringHashFunc{
    int operator()(string s) { return s.size() % 2 - 1; }
  };

  Hashmap<string, string, StringHashFunc> hash1;

  hash1[kDeathsCounterKey] = "a";
  hash1[kGoldObtainedCounterKey] = "b";
  hash1[kExperienceGainedCounterKey] = "c";

  return hash1[kDeathsCounterKey] == "a" &&
    hash1[kGoldObtainedCounterKey] == "b" &&
    hash1[kExperienceGainedCounterKey] == "c";
}


// Test Methods Implementation
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
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService( 2, 2 );
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
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService( 2, 2 );
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
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService( 2, 2 );
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

  bool result = !heatmap.getCounterDataInsideRect({ 2, 2 },{ -2, -2 }, kDeathsCounterKey, out_data);

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
  heatmap_service::HeatmapService *heatmap = new heatmap_service::HeatmapService(1,1);
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

void StressTestMillionRegisters10per5Coords()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService(1);

  clock_t init = clock();
  for (long int i = 0; i < 1000000; i++)
  {
    int randX = rand() % 10;
    int randY = rand() % 5;
    heatmap.IncrementMapCounter({ randX, randY }, kDeathsCounterKey);
  }
  clock_t end = clock();
  float diff((float)end - (float)init);
  cout << " test took " << (diff / CLOCKS_PER_SEC) << " seconds " << endl;
}

void StressTestMillionRegisters10kper10kCoords()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService(10);

  clock_t init = clock();
  for (long int i = 0; i < 1000000; i++)
  {
    int randX = rand() % 10000 - 5000;
    int randY = rand() % 10000 - 5000;
    heatmap.IncrementMapCounter({ randX, randY }, kDeathsCounterKey);
  }
  clock_t end = clock();
  float diff((float)end - (float)init);
  cout << " test took " << (diff / CLOCKS_PER_SEC) << " seconds " << endl;
}

void StressTestMillionRegisters5kper5kOnlyNegativeCoords()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService(10);

  clock_t init = clock();
  for (long int i = 0; i < 1000000; i++)
  {
    int randX = rand() % 5000 - 5000;
    int randY = rand() % 5000 - 5000;
    heatmap.IncrementMapCounter({ randX, randY }, kDeathsCounterKey);
  }
  clock_t end = clock();
  float diff((float)end - (float)init);
  cout << " test took " << (diff / CLOCKS_PER_SEC) << " seconds " << endl;
}

void StressTestThousandRegistriesFractionalResolution()
{
  heatmap_service::HeatmapService heatmap = heatmap_service::HeatmapService(0.5);

  clock_t init = clock();
  for (long int i = 0; i < 1000; i++)
  {
    int randX = rand() % 100 - 50;
    int randY = rand() % 100 - 50;
    heatmap.IncrementMapCounter({ randX, randY }, kDeathsCounterKey);
  }
  clock_t end = clock();
  float diff((float)end - (float)init);
  cout << " test took " << (diff / CLOCKS_PER_SEC) << " seconds " << endl;
}


