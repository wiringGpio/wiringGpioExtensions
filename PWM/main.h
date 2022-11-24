#pragma once
#include <map>

//  Map of MCP expander chips, base address to number of pins
extern std::map<int, int> McpMap;

//  Map of PCA chips
extern std::map<int, int> PcaMap;


//  Prompt user with message and accept input from keyboard
int GetIntegerFromKeyboard(std::string message);