#pragma once
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "../include/book.h"

bool splitLine(std::string& line, std::vector<std::string> &lineSplit, char token);
bool parseBook(std::vector<std::string>& line, struct book& thisBook);
bool unparseBook(struct book& thisBook, std::string& line);
bool removeBook(std::vector<struct book>& books, struct book& delBook);
bool findBook(std::vector<struct book> &books, std::vector<struct book> &resultBooks, std::string query);