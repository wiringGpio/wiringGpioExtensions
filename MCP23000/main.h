#pragma once
#include <map>

//  Map of MCP expander chips, base address to number of pins
extern std::map<int, int> McpMap;


//  Prompt user with message and accept input from keyboard
int GetIntegerFromKeyboard(std::string message);