#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

bool readFile(std::vector<std::string>& lines, std::string fileLocation);
bool writeFile(std::vector<std::string>& lines, std::string fileLocation);
bool appendFile(std::string line, std::string fileLocation);

