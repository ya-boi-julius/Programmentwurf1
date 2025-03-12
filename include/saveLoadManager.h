#pragma once
#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#include "../include/fileManager.h"
#include "../include/bookManager.h"
#include "../include/book.h"

bool loadBooks(std::vector<struct book>& books, std::string fileLocation);
bool saveBooks(std::vector<struct book>& books, std::string fileLocation);
bool addBook(struct book& thisBook, std::string fileLocation);
bool loadOrigin(std::string fileLocationOrigin, std::string fileLocationNew);