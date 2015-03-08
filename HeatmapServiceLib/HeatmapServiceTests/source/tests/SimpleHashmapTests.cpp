#pragma once

#include "SimpleHashmapTests.h"
#include "SimpleHashmap.hpp"
#include <iostream>

using namespace std;
using namespace heatmap_service;

const string kDeathsCounterKey = "deaths";
const string kGoldObtainedCounterKey = "gold_obtained";
const string kExperienceGainedCounterKey = "xp_gained";
const string kKillsCounterKey = "kills";
const string kSkillsUsedKey = "skills_used";
const string kDodgesKey = "dodge_rolls";

void TestHashMapAll()
{
  cout << "######## Simple Hashmap Tests ########" << endl << endl;

  cout << "TestHashMapCreation: [" << (TestHashMapCreation() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestHashMapPlacementAndRetrieval: [" << (TestHashMapPlacementAndRetrieval() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestHashMapCopyAssignment: [" << (TestHashMapCopyAssignment() ? "PASSED" : "FAILED") << "]" << endl;
  cout << "TestHashMapCollision: [" << (TestHashMapCollision() ? "PASSED" : "FAILED") << "]" << endl;

  cout << endl;
}

struct HashFunctor
{
  int operator()(const std::string& s){
    int prime = 7;
    for (const char& c : s)
      prime = prime >> (int)c;
    return prime;
  }
};

bool TestHashMapCreation() {
  SimpleHashmap<string, int, HashFunctor> hash1;
  return !hash1.has_key(kDeathsCounterKey);
}
bool TestHashMapPlacementAndRetrieval() {
  SimpleHashmap<string, int, HashFunctor> hash1;

  hash1[kDeathsCounterKey] = 2;
  hash1[kDeathsCounterKey] = 0;

  hash1[kGoldObtainedCounterKey] = 4;
  hash1[kExperienceGainedCounterKey] = 10;
  bool test1 = hash1.has_key(kDeathsCounterKey);
  return hash1[kDeathsCounterKey] == 0 && hash1[kGoldObtainedCounterKey] == 4 && hash1[kExperienceGainedCounterKey] == 10;
}
bool TestHashMapCopyAssignment() {
  SimpleHashmap<string, int, HashFunctor> hash1;
  SimpleHashmap<string, int, HashFunctor> hash2(hash1);
  SimpleHashmap<string, int, HashFunctor> hash3;

  hash1[kDeathsCounterKey] = 2;
  hash1[kDeathsCounterKey] = 0;
  hash3[kDeathsCounterKey] = 20;

  hash1[kGoldObtainedCounterKey] = 4;
  hash1[kExperienceGainedCounterKey] = 10;

  hash3 = hash1;

  SimpleHashmap<string, int, HashFunctor> hash4(hash1);
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

  SimpleHashmap<string, string, StringHashFunc> hash1;

  hash1[kDeathsCounterKey] = "a";
  hash1[kGoldObtainedCounterKey] = "b";
  hash1[kExperienceGainedCounterKey] = "c";

  std::string a = hash1[kDeathsCounterKey];
  std::string b = hash1[kGoldObtainedCounterKey];
  std::string c = hash1[kExperienceGainedCounterKey];

  return  a == "a" &&
    b == "b" &&
    c == "c";
}