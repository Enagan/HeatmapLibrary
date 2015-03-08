#pragma once

#include "SignedIndexVector.hpp"
#include "SignedIndexVectorTests.h"
#include <iostream>

using namespace std;
using namespace heatmap_service;

void TestSIVAll()
{
  cout << "######## Signed Index Vector Tests ########" << endl << endl;

  cout << "TestSIVCreationPrefilledCreation: [" << (TestSIVCreationPrefilledCreation() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestSIVCopyAssignment: [" << (TestSIVCopyAssignment() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestSIVIterators: [" << (TestSIVIterators() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestSIVInsertionAndGetting: [" << (TestSIVInsertionAndGetting() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestSIVClearAndClean: [" << (TestSIVClearAndClean() ? "PASSED" : "FAILED") << "]" << endl;

  cout << endl;
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