#ifndef PROGRAMSELECT_H
#define PROGRAMSELECT_H
#include <iostream>
#include <vector>
#include <string>
#include "../include/UIhandler.h"
#include "../include/book.h"
#include "../include/saveLoadManager.h"

bool PSaddBook();
bool PSfindBook(struct book& foundBook);
bool PSdeleteBook();
bool PSloadOriginal();
bool selectProgram();
#endif