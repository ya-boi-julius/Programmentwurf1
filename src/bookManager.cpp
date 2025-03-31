#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "../include/book.h"
#include "../include/bookManager.h"


//teilt eine Zeile entsprechend dem mitgegebenen Trennzeichen auf, und speichert diese im mitgegebenen Vektor
//gibt true zurük wenn erfolgreich falls (noch) nicht implementiert
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

//erhült eine aufgeteilte Zeile und parsed diese in ein element des book-structs (mir ist noch keine bessere Methode zu verteilen eingefallen)
//true falls erfolgreich, false falls nicht (falls ein vector mehr elemente hat als erwartet)
std::exception* parseBook(std::vector<std::string> &line, struct book& thisBook){
    try{
        std::vector<std::string>::iterator it;
        int index = 0;
        //it + 1 um erste Zeile mit Labels zu ignorieren
        //muss ich wahrscheinlich an anderen stellen im code auch noch ündern
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
                        //kann mit Zeiten vor 1902 nicht wirklich umgehen, da time_t zu klein wird.
                        thisBook.yearString = *it;
                        struct std::tm timestamp = {0, 0, 0, 1, 0, 0, -1, -1, -1};
                        std::istringstream tempstream(*it);
                        if(std::count(thisBook.yearString.begin(), thisBook.yearString.end(), '.') == 2){
                            tempstream >> std::get_time(&timestamp, "%d.%m.%Y" );    
                        }else if(thisBook.yearString.length() == 4){
                            tempstream >> std::get_time(&timestamp, "%Y" );
                        }else{
                            thisBook.yearString = "Unbekannt";
                        }
                        thisBook.timestamp = timestamp;
                        thisBook.time = std::mktime(&timestamp);
                    }break;
                case 2:
                    {
                    thisBook.title = *it;
                    }break;
                case 3:
                    {
                        thisBook.isbnString = *it;
                        if((*it)[0] < 48 || 57 < (*it)[0]){
                            thisBook.isbn = -1;
                            thisBook.isbnString = "Unbekannt";
                            break;
                        }else{
                            thisBook.isbn = std::stoll(*it);
                        }
                    }break;
                case 4:
                    {
                    std::string tempstring;
                    //kleine Split-Funktion, notwendig, damit C++ die Zahl als Float erkennt.
                    //02:14 c != '.' hinzugefügt, damit preise erhalten bleiben
                    thisBook.priceString = *it;
                    for(char c : *it){
                        if((c < 48 || c > 57) && c != ',' && c != '.'){
                            tempstring = "-1";
                            break;
                        }
                        if(c != ','){
                            tempstring += c;
                        }else{
                            tempstring += ',';
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
        std::stoll(thisBook.isbnString);
    }catch(std::invalid_argument& ia){
        thisBook.isbnString = "Unbekannt";
        thisBook.isbn = -1;
    }
    try{
        std::stof(thisBook.priceString);
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
//gibt true zurück falls erfolgreich, false falls das buch nicht gefunden wurde
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
    throw new std::runtime_error("Leider konnte Ihr gewähltes Buch nicht entfernt werden.\n");
    }catch(std::runtime_error* err){
        return err;
    }
    return nullptr;
}

//gibt alle Bücher zurück, bei denen ein Feld dem Suchbegriff exakt entspricht. 
//nicht mit "find" implementiert, da ich nicht weiÃŸ, wie man das mit structs anwendet
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

//Durch die verschiedenen Arten von Einträgen, die im Autoren-Feld stehen können,
//scheint mir das sortieren nach Nachnamen nur mit größerem Aufwand möglich zu sein. 
//Deswegen hab ich es erstmal nicht gemacht.
std::exception* sortByAuthor(std::vector<struct book>& books, bool ascending){
    std::vector<struct book>::iterator it;
    std::vector<struct book>::iterator it2;
    //Bubble Sort. Ineffizient, aber eignet sich für den Vergleich zwischen Namen
    try {
        for(it = (books.begin() + 1); it != books.end(); it++){
            for(it2 = (it + 1); it2 != books.end(); it2++){
                for (int i = 0; (i  < (*it).author.length()) && (i < (*it2).author.length()); i++){
                    if((*it).author[i] != (*it2).author[i]){
                        if(ascending == (std::tolower((*it).author[i]) < std::tolower((*it2).author[i]))){
                            struct book temp = *it;
                            *it = *it2;
                            *it2 = temp;
                            break;
                        }else if(ascending == (std::tolower((*it).author[i]) > std::tolower((*it2).author[i]))){
                            break;
                        }
                    }
                }
            }
        }  
        return nullptr;
    }catch(std::exception* err){
        return err;
    }
}

//quasi exakt wie sortByAuthor, nur eben auf dem title Feld
std::exception* sortByTitle(std::vector<struct book>& books, bool ascending){
    std::vector<struct book>::iterator it;
    std::vector<struct book>::iterator it2;
    //Bubble Sort. Ineffizient, aber eignet sich für den Vergleich zwischen Namen
    try {
        for(it = (books.begin() + 1); it != books.end(); it++){
            for(it2 = (it + 1); it2 != books.end(); it2++){
                for (int i = 0; (i  < (*it).title.length()) && (i < (*it2).title.length()); i++){
                    if(std::tolower((*it).title[i]) != std::tolower((*it2).title[i])){
                        if(ascending == (std::tolower((*it).title[i]) < std::tolower((*it2).title[i]))){
                            struct book temp = *it;
                            *it = *it2;
                            *it2 = temp;
                            break;
                        }else if(ascending == (std::tolower((*it).title[i]) > std::tolower((*it2).title[i]))){
                            break;
                        }
                    }
                }
            }
        }  
        return nullptr;
    }catch(std::exception* err){
        return err;
    }
}


//TODO: Sortierung repariere, if else Blöcke überdenken.
std::exception* sortByDate(std::vector<struct book> &books, bool ascending){
    std::vector<struct book>::iterator it;
    std::vector<struct book>::iterator it2;
    try{
        for(it = (books.begin() + 1); it != books.end(); it++){
            for(it2 = (it + 1); it2 != books.end(); it2++){
                struct book temp;
                if (ascending == ((*it).time > (*it2).time)){//sortiert Zeiten fall gültige Zeiteinträge vorhanden sind.
                    temp = *it;
                    *it = *it2;
                    *it2 = temp;
                }
                if((*it).time == -1 && (*it2).time == -1 && (*it).yearString.length() == 4 && (*it2).yearString.length() == 4 ){
                    //Falls beide Zeiten keinen gültigen Zeiteintrag haben, jedoch eine Jahreszahl
                    if (ascending == ((*it).timestamp.tm_year > (*it2).timestamp.tm_year)){
                        temp = *it;
                        *it = *it2;
                        *it2 = temp;
                    }
                }
                if((*it).yearString == "Unbekannt" && (*it2).yearString != "Unbekannt"){
                    temp = *it;
                        *it = *it2;
                        *it2 = temp;
                }
            }
        }
    }catch(std::exception& err){
        return new std::runtime_error("Leider ist beim sortieren der Bücher nach Erscheinungsdatum ein Fehler aufgetreten.\n");
    }
    return nullptr;
}
