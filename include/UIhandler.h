#pragma once
#include <iostream>
#include <vector>
#include "../include/book.h"

void printBooks(std::vector<struct book> books);
void drawMainMenu(std::vector<std::string>& menuItems);
void exitMessage();
void clearScreen();