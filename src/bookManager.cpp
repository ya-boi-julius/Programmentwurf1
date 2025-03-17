#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "../include/book.h"
#include "../include/bookManager.h"


//teilt eine Zeile entsprechend dem mitgegebenen Trennzeichen auf, und speichert diese im mitgegebenen Vektor
//gibt true zur�k wenn erfolgreich falls (noch) nicht implementiert
//TODO: flase implementieren
std::exception* splitLine(std::string &line, std::vector<std::string> &lineSplit, char token){
    try{
        std::string linePart;
        lineSplit.clear();
        int quotationCounter = 0;
        for(char c : line){
            if(c == token && quotationCounter % 2 == 0){
                if(!linePart.empty()){
                    if(c != '"'){
                        lineSplit.push_back(linePart);
                    }
                }
                linePart.clear();
            }else{
                if(c == '"'){
                    quotationCounter++;
                }else{
                    linePart += c; 
                }         
            }
        }
        lineSplit.push_back(linePart);  
    }catch(std::exception* err){
        return err;
    }
    return nullptr;
}

//erh�lt eine aufgeteilte Zeile und parsed diese in ein element des book-structs (mir ist noch keine bessere Methode zu verteilen eingefallen)
//true falls erfolgreich, false falls nicht (falls ein vector mehr elemente hat als erwartet)
std::exception* parseBook(std::vector<std::string> &line, struct book& thisBook){
    try{
        std::vector<std::string>::iterator it;
        int index = 0;
        //it + 1 um erste Zeile mit Labels zu ignorieren
        //muss ich wahrscheinlich an anderen stellen im code auch noch �ndern
        //timestamp: 23:32
        //23:54 relativ genervt von invalid arguments in der Liste, und ich glaube auch, dass wir die von stoi/stof geworfenen errors nich in der VL hatten
        for(it = line.begin(); it != line.end(); it++){
            switch(index){
                case 0: 
                    {
                        thisBook.author = *it;
                    }break;
                case 1:
                    {
                        thisBook.yearString = *it;
                        struct std::tm timestamp = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
                        std::istringstream tempstream(*it);
                        tempstream >> std::get_time(&timestamp, "%d.%m.%Y" );
                        thisBook.timestamp = timestamp;
                        thisBook.time = std::mktime(&timestamp);
                    }break;
                case 2:
                    {
                    thisBook.title = *it;
                    }break;
                case 3:
                    {
                        //parsing crashes here for some reason
                        //it was the isbn being too long, now it's isbn being "unbekannt"
                        thisBook.isbnString = *it;
                        if((*it)[0] < 48 || 57 < (*it)[0]){
                            thisBook.isbn = -1;
                            break;
                        }else{
                            thisBook.isbn = std::stoll(*it);
                        }
                    }break;
                case 4:
                    {
                    std::string tempstring;
                    //kleine Split-Funktion, notwendig, damit C++ die Zahl als Float erkennt.
                    //02:14 c != '.' hinzugef�gt, damit preise erhalten bleiben
                    thisBook.priceString = *it;
                    for(char c : *it){
                        if((c < 48 || c > 57) && c != ',' && c != '.'){
                            tempstring = "-1";
                            break;
                        }
                        if(c != ','){
                            tempstring += c;
                        }else{
                            tempstring += '.';
                        }
                    }
                    thisBook.price = std::stof(tempstring);
                    }break;
                default:
                {
                    
                }
            }
            index++;
        }
    }catch(std::exception* err){
        return err;
    }
    return nullptr;
}

//wandelt ein Buch in einen CSV kompatiblen String um, der in eine Datei geschrieben werden kann
std::exception*  unparseBook(struct book &thisBook, std::string& line){
    try{
        long long isbn = std::stoll(thisBook.isbnString);
    }catch(std::invalid_argument& ia){
        thisBook.isbnString = "Unbekannt";
        thisBook.isbn = -1;
    }
    try{
        float isbn = std::stof(thisBook.priceString);
    }catch(std::invalid_argument& ia){
        thisBook.priceString = "Unbekannt";
        thisBook.price = -1;
    }
    try{
    line = {"\"" + thisBook.author + "\";" + thisBook.yearString + "; \"\"\"" + thisBook.title + "\"\"\";" + thisBook.isbnString + ";" + thisBook.priceString};
    return nullptr;
    }catch(std::exception* err){
        std::cerr << (*err).what();
        return err;
    }
}

//entfernt ein buch aus dem Vektor
//gibt true zur�ck falls erfolgreich, false falls das buch nicht gefunden wurde
std::exception* removeBook(std::vector<struct book>& books, struct book &delBook){
    try{
        std::vector<struct book>::iterator it;
        for(it = books.begin(); it != books.end(); it++){
            struct book thisBook = *it;
            //dereferencing the book into it's own object for debugging purposes
            if(thisBook.author == delBook.author &&
                thisBook.isbn == delBook.isbn &&
                thisBook.price == delBook.price &&
                thisBook.title == delBook.title &&
                thisBook.yearString == delBook.yearString){
                    books.erase(it);
                return nullptr;
            }
        }
    throw new std::runtime_error("Leider konnte Ihr gew�hltes Buch nicht entfernt werden.\n");
    }catch(std::runtime_error* err){
        return err;
    }
    return nullptr;
}

//gibt alle B�cher zur�ck, bei denen ein Feld dem Suchbegriff exakt entspricht. 
//nicht mit "find" implementiert, da ich nicht weiß, wie man das mit structs anwendet
std::exception* findBook(std::vector<struct book> &books, std::vector<struct book> &resultBooks, std::string query){
    try{
        std::vector<struct book>::iterator it;
        bool success = false;
        for(it = books.begin(); it != books.end(); it++){
            if(
                (*it).author == query ||
                (*it).yearString == query ||
                (*it).title == query ||
                (*it).isbnString == query ||
                (*it).priceString == query
            ){
                resultBooks.push_back(*it);
                success = true;
            }else if(query[0] > 46 && query[0] < 58){
                if(
                    (*it).isbn == stoll(query) ||
                    (*it).price == stof(query)
                ){
                    resultBooks.push_back(*it);
                    success = true;
                }
            }
        }
        if(!success){
            throw new std::runtime_error("Leider konnte ich kein Buch zu Ihrer Anfrage finden.\n");
        }
    }catch(std::runtime_error* err){
        return err;
    }
    return nullptr;
}

//Hab gerade gemerkt, dass das sortieren der B�cher nicht gefordert ist, eventuell mache ich diese Funktion fertig wenn ich mit dem Rest des programmes Fertig bin.
/*
void sortByAuthor(std::vector<struct book>& books, bool ascending, bool lastname){
    std::vector<struct book>::iterator it;
    std::vector<struct book>::iterator it2;
    std::vector<std::string> lineSplit1;
    std::vector<std::string> &lineSplitRef1 = lineSplit1;
    std::vector<std::string> lineSplit2;
    std::vector<std::string> &lineSplitRef1 = lineSplit2;
    //Bubble Sort. Ineffizient, aber eignet sich f�r den Vergleich zwischen Namen
    for(it = books.begin(); it != books.end(); it++){
        if(lastname){
            splitLine((*it).author, lineSplit1, ' ');
        }
        
    
    }
}
*/