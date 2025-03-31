#include <iostream>
#include <Windows.h>
#include "../include/programSelect.h"

int main() {
    SetConsoleCP ( 1252 ) ;
    SetConsoleOutputCP ( 1252 ) ;
    std::cout << "Programmentwurf von Julius Clausen\n"<<
        "Matrikelnummer 3160705\n"<<
        "Ohne KI oder dergleichen geschrieben.\n";
    selectProgram();
}