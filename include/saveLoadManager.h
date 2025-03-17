#pragma once
#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#include "../include/fileManager.h"
#include "../include/bookManager.h"
#include "../include/book.h"

std::exception* loadBooks(std::vector<struct book>& books, std::string fileLocation);
std::exception* saveBooks(std::vector<struct book>& books, std::string fileLocation);
std::exception* addBook(struct book& thisBook, std::string fileLocation);
std::exception* loadOrigin(std::string fileLocationOrigin, std::string fileLocationNew);