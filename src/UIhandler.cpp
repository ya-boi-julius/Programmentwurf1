#include <iostream>
#include <vector>
#include "../include/book.h"

//leert die Konsole
//TODO: Funktion finden Die tats√§chlich funktioniert.
void clearScreen(){
    std::cout << "\x1B[2J\x1B[H";
    system("cls");
}

//druckt alle B√ºcher aus dem Vektor auf die Konsole.
//Die Reihenfolge entspricht nicht der Reihenfolge der Daten in der Datei,
//sondern einer Reihenfolge dich ich persˆnlich intuitiver finde.
void printBooks(std::vector<struct book> books){
    std::vector<struct book>::iterator it;
    std::cout << "Index:\tAutor:\t\t\tTitel:\t\t\tErschienen:\t\tISBN:\t\tPreis\n";
    std::cout << "-------------------------------------------------------------------\n";
    int index = 0;
    for(it = books.begin(); it != books.end(); it++){
        index++;
        std::cout
            << index << "\t"
            << (*it).author << "\t"
            << (*it).title << "\t"
            << (*it).yearString << "\t"
            << (*it).isbnString << "\t"
            << (*it).priceString << "\n"
            ;
    }
}

//Dynamisch gestaltet, damit ich die Funktion nicht immer √§ndern muss, wenn ich ein neues Programm hinzuf√ºge.
//gibt einfach nur die mˆglichen Programme aus, mit einem Index damit man wei√ü, wie man welches w√§hlen kann.
void drawMainMenu(std::vector<std::string>& menuItems){
    int index = 1;
    std::vector<std::string>::iterator it;
    clearScreen();
    std::cout << "Willkommen in der Digital Bibliothek. Ich bin Julius, was kann ich f¸r Sie tun?\nBitte w‰hlen Sie ein Programm anhand seiner Nummer aus.\n\n";
    for(it = menuItems.begin(); it != menuItems.end(); it++){
        std::cout << index << ": " << *it << "\n";
        index++;
    } 
}

//Nur eine kleine Abschiedsnachricht
void exitMessage(){
    std::cout << "Vielen Dank f¸r Ihren Besuch. Bis bald.\n\nDr¸cken Sie \"Enter\" um das Programm zu verlassen\n";
    std::string n;
    std::getline(std::cin, n);
}
