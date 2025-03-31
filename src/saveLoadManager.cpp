#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#include "../include/fileManager.h"
#include "../include/bookManager.h"
#include "../include/book.h"


//Verwaltet das Laden und Parsen von Büchern aus einer Datei
//falls in einer der nachgestellten Dateien ein Fehler auftritt gibt auch diese Datei false zurück
std::exception* loadBooks(std::vector<struct book>& books, std::string fileLocation){
    std::vector<std::string> lines;
    std::vector<std::string>& linesRef = lines;
    std::vector<std::string> lineSplit;
    std::vector<std::string> lineSplitRef = lineSplit;

    try{
        std::exception* err = readFile(linesRef, fileLocation);
        if(err != nullptr){throw err;}
        std::vector<std::string>::iterator it;
        for(it = lines.begin() + 1; it != lines.end(); it ++){
            err = splitLine(*it, lineSplitRef, ';');
            if(err != nullptr){throw err;}
            struct book newBook;
            struct book& newBookRef = newBook;
            err = parseBook(lineSplitRef, newBookRef);
            if(err != nullptr){throw err;}
            books.push_back(newBook);
        }
    }
    catch(std::exception* re){
        return re;
    }
    return nullptr;
}

//speichert die Liste der Bücher in der angegebenen Datei
std::exception* saveBooks(std::vector<struct book>& books, std::string fileLocation){
    std::vector<std::string> lines;
    std::vector<std::string>& linesRef = lines;
    std::vector<struct book>::iterator it;
    try{
        lines.push_back("Autor;Erscheinungsjahr;Titel;ISBN;Aktueller Preis");
        for(it = books.begin(); it != books.end(); it++){
            std::string line;
            std::string& lineRef = line;
            std::exception* err = unparseBook(*it, lineRef);
            if(err != nullptr){throw err;}
            lines.push_back(line);
        }
        std::exception* err = writeFile(linesRef, fileLocation);
        if(err != nullptr){throw err;}
    }catch(std::runtime_error* re){
        return re;
    }
    return nullptr;
}


//Fügt das engegebene Buch der angegebenen Datei hinzu
std::exception* addBook(struct book& thisBook, std::string fileLocation){
    try{
        std::string bookString;
        std::string& bookStringRef = bookString;
        std::exception* err =  unparseBook(thisBook, bookStringRef);
        if(err != nullptr){throw err;}
        err = appendFile(bookString, fileLocation);
        if(err != nullptr){throw err;}
    }catch(std::runtime_error* re){
        return re;
    }
    return nullptr;
}


//lädt die Bücher aus der angegebenen Datei, löscht das gesuchte Buch, und schreibt die aktuelisierte Liste zurück in die Datei
std::exception* removeBookSL(struct book& thisBook, std::string fileLocation){
    std::vector<struct book> books;
    std::vector<struct book> booksRef = books;
    try{
        std::exception* err = loadBooks(booksRef, fileLocation);
        if(err != nullptr){throw err;}
        err = removeBook(booksRef, thisBook);
        if(err != nullptr){throw err;}
        err = saveBooks(booksRef, fileLocation);
        if(err != nullptr){throw err;}
        /*
        if(!loadBooks(booksRef, fileLocation)){
            throw std::runtime_error("ERROR WHILE LOADING BOOKS FOR DELETING\n");
        }
        if(!removeBook(booksRef, thisBook)){
            throw std::runtime_error("ERROR WHILE DELETING BOOK\n");
        }
        if(!saveBooks(booksRef, fileLocation)){
            throw std::runtime_error("ERROR WHILE SAVING UPDATED FILE\n");
        }
        */
    }catch(std::exception* re){
        return re;
    }
    return nullptr;
}

std::exception* loadOrigin(std::string fileLocationOrigin, std::string fielLocationNew){
    try{
        std::vector<std::string> lines;
        std::vector<std::string>& linesRef = lines;
        std::exception* err = readFile(linesRef, fileLocationOrigin);
        if(err != nullptr){
            throw err;
        }
        err = writeFile(linesRef, fielLocationNew);
        if(err != nullptr){
            throw err;
        }
    }catch(std::exception* re){
        return re;
    }
    return nullptr;
}