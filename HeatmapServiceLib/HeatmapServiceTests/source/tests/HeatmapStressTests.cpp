#pragma once

#include "HeatmapService.h"
#include "HeatmapStressTests.h"
#include <iostream>
#include <ctime>

using namespace std;
using namespace heatmap_service;

const string kDeathsCounterKey = "deaths";
const string kGoldObtainedCounterKey = "gold_obtained";
const string kExperienceGainedCounterKey = "xp_gained";
const string kKillsCounterKey = "kills";
const string kSkillsUsedKey = "skills_used";
const string kDodgesKey = "dodge_rolls";

void HeatmapStressTestAll()
{
  cout << endl << "-------------- Heatmap Stress Tests -----------------" << endl;

  cout << endl << "Starting... StressTestMillionRegisters10kper10kCoords";
  StressTestMillionRegisters10kper10kCoords();
  cout << endl << "Starting... StressTestMillionRegisters10per5Coords";
  StressTestMillionRegisters10per5Coords();
  cout << endl << "Starting... StressTestMillionRegisters5kper5kOnlyNegativeCoords";
  StressTestMillionRegisters5kper5kOnlyNegativeCoords();
  cout << endl << "Starting... StressTestThousandRegistriesFractionalResolution";
  StressTestThousandRegistriesFractionalResolution();

  cout << endl;
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

