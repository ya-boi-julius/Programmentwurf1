#pragma once
#include <string>
#include <ctime>

struct book{
    std::string author;
    std::string yearString;
    std::string title;
    int isbn;
    float price;
    std::tm timestamp;
    std::time_t time;
};