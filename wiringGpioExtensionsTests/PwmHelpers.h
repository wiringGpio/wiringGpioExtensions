#pragma once
#include <map>

#define PWM_FREQUENCY 50.0
#define MAX_PWM 4096

void PcaAllOff();

int calcTicks(float impulseMs, float hertz);

void PrintPcaState();

//  Map of PCA chips
extern std::map<int, int> PcaMap;


