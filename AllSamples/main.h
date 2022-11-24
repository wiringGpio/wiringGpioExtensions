#pragma once
#include <map>
#include <string>

#define PWM_FREQUENCY 50.0
#define MAX_PWM 4096

//  global variables

//  Map of MCP expander chips, base address to number of pins
extern std::map<int, int> McpMap;

//  Map of PCA chips
extern std::map<int, int> PcaMap;





void PcaAllOff();

int calcTicks(float impulseMs, float hertz);


int GetIntegerFromKeyboard(std::string message);

float GetFloatFromKeyboard(std::string message);
