#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#include "../include/fileManager.h"
#include "../include/bookManager.h"
#include "../include/book.h"


//Verwaltet das Laden und Parsen von Büchern aus einer Datei
//falls in einer der nachgestellten Dateien ein Fehler auftritt gibt auch diese Datei false zurück
bool loadBooks(std::vector<struct book>& books, std::string fileLocation){
    std::vector<std::string> lines;
    std::vector<std::string>& linesRef = lines;
    std::vector<std::string> lineSplit;
    std::vector<std::string> lineSplitRef = lineSplit;

    try{
        if(!readFile(linesRef, fileLocation)){
            throw std::runtime_error("ERROR WHILE READING FILE\n");
        }
        std::vector<std::string>::iterator it;
        //it + 1 aus bekannten Gründen
        for(it = lines.begin(); it != lines.end(); it ++){
            if(!splitLine(*it, lineSplitRef, ';')){
                throw std::runtime_error("ERROR WHILE SPLITTING LINE\n");
            }
            struct book newBook;
            struct book& newBookRef = newBook;
            if(!parseBook(lineSplitRef, newBookRef)){
                throw std::runtime_error("ERROR WHILE PARSING BOOK\n");
            }
            books.push_back(newBook);
        }
    }
    catch(std::runtime_error re){
        std::cerr << re.what();
        return false;
    }
    return true;
}

//speichert die Liste der Bücher in der angegebenen Datei
bool saveBooks(std::vector<struct book>& books, std::string fileLocation){
    std::vector<std::string> lines;
    std::vector<std::string>& linesRef = lines;
    std::vector<struct book>::iterator it;
    try{
        for(it = books.begin(); it != books.end(); it++){
            std::string line;
            std::string& lineRef = line;
            if(!unparseBook(*it, lineRef)){
                throw std::runtime_error("ERROR WHILE UNPARSING BOOK\n");
            }
            lines.push_back(line);
        }
        if(!writeFile(linesRef, fileLocation)){
            throw std::runtime_error("UNABLE TO WRITE TO FILE\n");
        }
    }catch(std::runtime_error re){
        std::cerr << re.what();
    }
    return true;
}


//Fügt das engegebene Buch der angegebenen Datei hinzu
bool addBook(struct book& thisBook, std::string fileLocation){
    try{
        std::string bookString;
        std::string& bookStringRef = bookString;
        if(!unparseBook(thisBook, bookStringRef)){
            throw std::runtime_error("UNABLE TO UNPARSE BOOK\n");
        }
        if(!appendFile(bookString, fileLocation)){
            throw std::runtime_error("UNABLE TO APPEND FILE\n");
        }
    }catch(std::runtime_error re){
        std::cerr << re.what();
    }
    return true;
}


//lädt die Bücher aus der angegebenen Datei, löscht das gesuchte Buch, und schreibt die aktuelisierte Liste zurück in die Datei
bool removeBookSL(struct book& thisBook, std::string fileLocation){
    std::vector<struct book> books;
    std::vector<struct book> booksRef = books;
    try{
        if(!loadBooks(booksRef, fileLocation)){
            throw std::runtime_error("ERROR WHILE LOADING BOOKS FOR DELETING\n");
        }
        if(!removeBook(booksRef, thisBook)){
            throw std::runtime_error("ERROR WHILE DELETING BOOK\n");
        }
        if(!saveBooks(booksRef, fileLocation)){
            throw std::runtime_error("ERROR WHILE SAVING UPDATED FILE\n");
        }

    }catch(std::runtime_error re){
        std::cerr << re.what();
    }
    return true;
}

bool loadOrigin(std::string fileLocationOrigin, std::string fielLocationNew){
    try{
        std::vector<std::string> lines;
        std::vector<std::string>& linesRef = lines;
        if(!readFile(linesRef, fileLocationOrigin)){
            throw std::runtime_error("ERROR READING FILE");
            return false;
        }
        if(!writeFile(linesRef, fielLocationNew)){
            throw std::runtime_error("ERROR WRITING TO FILE");
            return false;
        }
    }catch(std::runtime_error re){
        std::cerr << re.what();
    }
    return true;
}