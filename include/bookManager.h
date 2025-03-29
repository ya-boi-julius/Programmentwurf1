#pragma once
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "../include/book.h"

std::exception* splitLine(std::string& line, std::vector<std::string> &lineSplit, char token);
std::exception* parseBook(std::vector<std::string>& line, struct book& thisBook);
std::exception* unparseBook(struct book& thisBook, std::string& line);
std::exception* removeBook(std::vector<struct book>& books, struct book& delBook);
std::exception* findBook(std::vector<struct book> &books, std::vector<struct book> &resultBooks, std::string query);
std::exception* findBook(std::vector<struct book>& books);
std::exception* sortByDate(std::vector<struct book> &books, bool ascending);
std::exception* sortByAuthor(std::vector<struct book>& books, bool ascending);