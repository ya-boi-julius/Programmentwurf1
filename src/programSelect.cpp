#include <iostream>
#include <vector>
#include <string>
#include "../include/UIhandler.h"
#include "../include/book.h"
#include "../include/saveLoadManager.h"

std::vector<std::string> programs = {"Alle B�cher anzeigen", "Buch hinzuf�gen", "Buch l�schen", "Buch suchen", "Originalbestand herstellen", "Bibliothek verlassen"};
std::string fileLocation = "resources/buchliste_current.csv";
std::string fileLocationOriginal = "resources/buchliste_origin.csv";

//Fragt den Nutzer nacheinander die Daten eines Buches ab.
//Gibt true zur�ck wenn alles funktioniert hat, falls ein Fehler auftritt wird das Programm abgebrochen und false zur�ckgegeben.
bool PSaddBook(){
    try{
        clearScreen();
        std::string input;
        struct book thisBook;
        struct book& thisBookRef = thisBook;
        std::cin.ignore();
        std::cout << "Ah, ich freue mich, dass Sie der Bibliothek ein Buch spenden m�chten.\n"<<
                    "Ich m�sste nur zuerst ein paar Eckdaten abfragen. Wie lautet der Name des Buches?\n";
        std::getline(std::cin, input);
        std::cin.ignore();
        thisBookRef.title = input;
        input.clear();
        clearScreen();
        std::cout << "Vielen Dank. Wer ist der Autor?\n";
        std::getline(std::cin, input);
        std::cin.ignore();
        thisBookRef.author = input;
        input.clear();
        clearScreen();
        std::cout << "Und wissen Sie auch, wann das Buch erschienen ist?\nBitte geben Sie das Datum im Format dd.mm.yyyy an.\n";
        std::getline(std::cin, input);
        std::cin.ignore();
        thisBookRef.yearString = input;
        input.clear();
        clearScreen();
        std::cout << "K�nnten Sie mir nun noch die ISBN sagen?\nSie finde sie meist auf der R�ckseite, in der N�he des Barcodes.\n";
        std::getline(std::cin, input);
        std::cin.ignore();
        thisBookRef.isbn = stoll(input);
        input.clear();
        clearScreen();
        std::cout << "Nun br�chte ich nur noch den Preis.\nDen auf dem Buch bitte, nicht was Sie daf�r gezahlt haben.\n";
        std::getline(std::cin, input);
        std::cin.ignore();
        thisBookRef.price = stof(input);
        input.clear();
        if(!addBook(thisBookRef, fileLocation)){
            throw std::runtime_error("UNABLE TO ADD BOOK FROM USER INPUT\n");
        }
        std::cout << "Vielen Dank, das war es auch schon.\nIch werde Ihr Buch so schnell wie m�glich in unserer Datenbank anlegen.\nKann ich sonst noch etwas f�r Sie tun?\n\n";
    }catch(std::runtime_error re){
        std::cout << "Oh, entschuldigen Sie bitte, anscheinend gab es ein Problem beim anlegen Ihres Buches."
                    << "\nSind Sie sicher, dass Sie sich nicht vielleicht irgendwo vertippt haben?\nVersuchen Sie es bitte nocheinmal.\n\n";
        return false;
    }
    return true;
}

bool PSfindBook(struct book& foundBook){
    try{
        clearScreen();
        std::vector<struct book> foundBookList;
        std::vector<struct book>& foundBooks = foundBookList;
        std::cout << "Ich kann Ihnen helfen, ein bestimmtes Buch zu finden."
                    << "\nSagen Sie mir bitte den Titel, Autor, ISBN, Preis, oder das Erscheinungsdatum."
                    << "\nAllerdings bitte nur eins davon, und Ihre Eingabe muss genau mit den Daten\n auf dem Buch �bereinstimmen!\n";
        std::string input;
        std::cin.ignore();
        std::getline(std::cin, input);
        //nicht der effizienteste Weg die B�cher hier neu zu laden, aber muss erstmal reichen
        std::vector<struct book> books;
        std::vector<struct book>& booksRef = books;
        loadBooks(booksRef, fileLocation);
        if(!findBook(booksRef, foundBooks, input)){
            //08.03.2025 00:07
            //findBooks l�uft endlich ohne crashes durch. wer wei�, was jetzt noch kommt
            throw std::runtime_error("\nEs tut mir leid, ich konnte leider kein Buch finden, das zu Ihrer Beschreibung passt.\n");
        }
        if (foundBooks.size()>0){
            std::cout << "Ich habe folgende B�cher gefunden, die zu Ihrer Beschreibung passen:\n";
            printBooks(foundBooks);
            std::cout << "\nBitte w�hlen Sie anhand des Index eines dieser B�cher aus.\n";
            int index;
            std::cin.ignore();
            std::cin >> input;
            index = stoi(input);
            std::vector<struct book>::iterator it = foundBooks.begin();
            it += index - 1;
            foundBook = *it;
            std::cout << "\nAlles klar, " << foundBook.title << " von " << foundBook.author << ".\n";
            
            return true;
        }else{
            std::cout << "\nLeider haben wir kein Buch in unserer Sammlung das Ihrer Suche entspricht.\n";

            return false;
        }

    }catch(std::runtime_error re){
        std::cout << re.what();
        return false;
    }
}

bool PSdeleteBook(){
    try{
        std::cout << "Bitte w�hlen Sie aus, welches Buch Sie l�schen m�chten";
        struct book bookToDelete;
        struct book& bookToDeleteRef = bookToDelete;
        if(PSfindBook(bookToDeleteRef)){
            std::cout << "Das von Ihnen gew�hlte Buch wird aus unserer Sammlung entfernt.\n";
        }else{
            throw std::runtime_error("\nEs tut mir leid, beim Suchen ist etwas schiefgegangen.");
        }
        std::vector<struct book> books;
        std::vector<struct book>& booksRef = books;
        if(!loadBooks(booksRef, fileLocation)){
            throw std::runtime_error("ERROR WHILE LOADING BOOKS FOR DELETING");
        }
        if(!removeBook(booksRef, bookToDeleteRef)){
            throw std::runtime_error("ERROR WHILE DELETING BOOK");
        }else{
            saveBooks(booksRef, fileLocation);
        }
    }catch(std::runtime_error re){
        std::cerr << re.what();
    }
    return true;
}

bool PSloadOriginal(){
    try{
        std::cout << "\nIch werde nun versuchen, unsere Datenbank wieder auf den Originalzustand zu setzen.\n";
        if(!loadOrigin(fileLocationOriginal, fileLocation)){
            throw std::runtime_error("Etwas ist bei der Synchronisierung mit der Hauptdatenbank schiefgegangen. Es tut mir leid.");
            
        }
    }catch(std::runtime_error re){
        std::cerr << re.what();
    }
    return true;
}

std::runtime_error* PSallBooks(){
    std::vector<struct book> books;
    std::vector<struct book>& booksRef = books;
    try{
        if(!loadBooks(booksRef, fileLocation)){
            throw std::runtime_error("Ich wahr leider nicht in der Lage die B�cher zu laden, �berpr�fen Sie bitte, ob die lokale Buchliste angelegt wurde.\n");
        }
        printBooks(booksRef);
        std::string n;
        std::cout << "\nBeliebige Taste + Enter um zur�ck zum Hauptmen� zu gelangen\n";
        std::cin >> n;
    }catch(std::runtime_error re){
        std::runtime_error* err = &re;
        return err;
    }
    return nullptr;
}

//TODO: refactor functions to return errors instead of just bools
//W�hlt eins der Oben stehenden Programme aus. Widerholt sich, bis der User das Programm beenden m�chte.
bool selectProgram(){
    bool exit = false;
    while(!exit){
        try{
            std::vector<std::string>& programsRef = programs;
            drawMainMenu(programsRef);
            std::string input;
            std::cin >> input;
            int selection = std::stoi(input);
            switch (selection){
                case 1:{
                    std::runtime_error* err = PSallBooks();
                    if (err != nullptr){
                        std::cerr << (*err).what();
                    }
                }break;
                case 2:{
                    clearScreen();
                    if(!PSaddBook()){
                        throw std::runtime_error("\nEtwas ist beim hinzuf�gen des Buches schiefgelaufen\n");
                    }
                }break;

                case 3:{
                    clearScreen();
                    if(!PSdeleteBook()){
                        throw std::runtime_error("\nEtwas ist beim l�schen des Buches schiefgelaufen\n");
                        break;
                    }
                }break;

                case 4:{
                    clearScreen();
                    struct book tempBook;
                    struct book& tempBookRef = tempBook;
                    if(!PSfindBook(tempBookRef)){
                        throw std::runtime_error("\nEtwas ist beim durchsuchen der B�cher schiefgelaufen.\nEs tut mir leid.\n");
                        break;
                    }
                }break;
                case 5:{
                    clearScreen();
                    if(!PSloadOriginal()){
                        throw std::runtime_error("Urspr�ngliche Datenbank konnte nicht wiederhergestellt werden");
                        break;
                    }
                }break;
                case 6:{
                    clearScreen();
                    exitMessage();
                    exit = true;
                }
            }
        }catch(std::runtime_error re){
            std::cerr << re.what();
        }
    }
    return true;
}