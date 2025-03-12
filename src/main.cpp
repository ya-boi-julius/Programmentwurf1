/*
Programmentwurf von Julius Clausen
Ich versichere, dass dieses Programm ohne fremde Hilfe, KI,
oder anderen Quellen als dem Vorlesungsmaterial und meiner eigenen Erfahrung geschrieben wurde.
Begonnen 07.03.2025 15:20
Pause 18:20 - 18:40
*/

#include <iostream>
#include <Windows.h>
#include "../include/programSelect.h"

int main() {
    SetConsoleCP ( CP_UTF8 ) ;
    SetConsoleOutputCP ( CP_UTF8 ) ;
    if(!selectProgram()){
        return 0;
    }else{
        return 1;
    }
}