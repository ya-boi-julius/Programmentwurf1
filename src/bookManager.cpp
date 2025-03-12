#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "../include/book.h"



//teilt eine Zeile entsprechend dem mitgegebenen Trennzeichen auf, und speichert diese im mitgegebenen Vektor
//gibt true zurück wenn erfolgreich falls (noch) nicht implementiert
//TODO: flase implementieren
bool splitLine(std::string &line, std::vector<std::string> &lineSplit, char token){
    std::string linePart;
    lineSplit.clear();
    for(char c : line){
        if(c == token){
            if(!linePart.empty()){
                lineSplit.push_back(linePart);
            }
            line.clear();
        }else{
            linePart += c;
        }
    }
    lineSplit.push_back(linePart);
    return true;
}

//erhält eine aufgeteilte Zeile und parsed diese in ein element des book-structs (mir ist noch keine bessere Methode zu verteilen eingefallen)
//true falls erfolgreich, false falls nicht (falls ein vector mehr elemente hat als erwartet)
bool parseBook(std::vector<std::string> &line, struct book& thisBook){
    std::vector<std::string>::iterator it;
    int index = 0;
    for(it = line.begin(); it != line.end(); it++){
        switch(index){
            case 0: 
                {
                    thisBook.author = *it;
                }break;
            case 1:
                {
                    thisBook.yearString = *it;
                }break;
            case 2:
                {
                thisBook.title = *it;
                }break;
            case 3:
                {
                thisBook.isbn = std::stoi(*it);
                }break;
            case 4:
                {
                thisBook.price = std::stof(*it);
                }break;
            case 5: 
                {
                struct std::tm timestamp;
                std::istringstream tempstream(*it);
                tempstream >> std::get_time(&timestamp, "%d.%mm.%Y" );
                thisBook.timestamp = timestamp;
                thisBook.time = std::mktime(&timestamp);
                }
                break;
            default:
            {
                std::cerr << "UNEXPECTED ERROR WHILE PARSING BOOKS. MORE INDEXES THAN EXPECTED";
                return false;
            }
        }
        index++;
    }
    return true;
}

//wandelt ein Buch in einen CSV kompatiblen String um, der in eine Datei geschrieben werden kann
bool unparseBook(struct book &thisBook, std::string line){
    line = {thisBook.author + ";" + thisBook.yearString + ";" + thisBook.title + ";" + std::to_string(thisBook.isbn) + ";" + std::to_string(thisBook.price)};
    return true;
}

//entfernt ein buch aus dem Vektor
//gibt true zurück falls erfolgreich, false falls das buch nicht gefunden wurde
bool removeBook(std::vector<struct book>& books, struct book &delBook){
    std::vector<struct book>::iterator it;
    for(it = books.begin(); it != books.end(); it++){
        if((*it).author == delBook.author &&
            (*it).isbn == delBook.isbn &&
            (*it).price == delBook.price &&
            (*it).title == delBook.title &&
            (*it).yearString == delBook.yearString){
                books.erase(it);
                return true;
        }
    }
    std::cerr << "ERROR, BOOK NOT FOUND\n";
    return false;
}

//gibt alle Bücher zurück, bei denen ein Feld dem Suchbegriff exakt entspricht. 
//nicht mit "find" implementiert, da ich nicht weiß, wie man das mit structs anwendet
bool findBook(std::vector<struct book> &books, std::vector<struct book> &resultBooks, std::string query){
    std::vector<struct book>::iterator it;
    bool success = false;
    for(it = books.begin(); it != books.end(); it++){
        if(
            (*it).author == query ||
            (*it).yearString == query ||
            (*it).title == query ||
            std::to_string((*it).isbn) == query ||
            std::to_string((*it).price) == query
        ){
            resultBooks.push_back(*it);
            success = true;
        }
    }
    return success;
}

//Hab gerade gemerkt, dass das sortieren der Bücher nicht gefordert ist, eventuell mache ich diese Funktion fertig wenn ich mit dem Rest des programmes Fertig bin.
/*
void sortByAuthor(std::vector<struct book>& books, bool ascending, bool lastname){
    std::vector<struct book>::iterator it;
    std::vector<struct book>::iterator it2;
    std::vector<std::string> lineSplit1;
    std::vector<std::string> &lineSplitRef1 = lineSplit1;
    std::vector<std::string> lineSplit2;
    std::vector<std::string> &lineSplitRef1 = lineSplit2;
    //Bubble Sort. Ineffizient, aber eignet sich für den Vergleich zwischen Namen
    for(it = books.begin(); it != books.end(); it++){
        if(lastname){
            splitLine((*it).author, lineSplit1, ' ');
        }
        
    
    }
}
*/