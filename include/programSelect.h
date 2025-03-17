#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "../include/UIhandler.h"
#include "../include/book.h"
#include "../include/saveLoadManager.h"

std::exception* PSaddBook();
std::exception* PSfindBook(struct book& foundBook);
std::exception* PSdeleteBook();
std::exception* PSloadOriginal();
void selectProgram();