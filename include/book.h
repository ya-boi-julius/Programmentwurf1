#pragma once
#include <string>
#include <ctime>

struct book{
    std::string author;
    std::string yearString;
    std::string title;
    long long int isbn;
    float price;
    std::tm timestamp;
    std::time_t time;
};