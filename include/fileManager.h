#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

std::exception* readFile(std::vector<std::string>& lines, std::string fileLocation);
std::exception* writeFile(std::vector<std::string>& lines, std::string fileLocation);
std::exception* appendFile(std::string line, std::string fileLocation);

