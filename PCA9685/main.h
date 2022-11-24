#pragma once
#include <map>
#include <string>

#define PWM_FREQUENCY 50.0
#define MAX_PWM 4096

//  Map of PCA chips
extern std::map<int, int> PcaMap;



//  Prompt user with message and accept input from keyboard
int GetIntegerFromKeyboard(std::string message);

//  Turn off all pca pins
void PcaAllOff();

//  Calculate ticks for given pulse length and frequency
int calcTicks(float impulseMs, float hertz);