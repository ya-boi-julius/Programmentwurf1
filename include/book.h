#pragma once
#include <string>
#include <ctime>

struct book{
    std::string author;
    std::string yearString;
    std::string title;
    long long int isbn;
    std::string isbnString;
    float price;
    std::string priceString;
    std::tm timestamp;
    std::time_t time;
};