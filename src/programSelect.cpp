#include <iostream>
#include <vector>
#include <string>
#include "../include/UIhandler.h"
#include "../include/book.h"
#include "../include/saveLoadManager.h"

std::vector<std::string> programs = {"Buch hinzufügen", "Buch löschen", "Buch suchen", "Originalbestand herstellen", "Bibliothek verlassen"};
std::string fileLocation = "../resources/buchliste_current.csv";
std::string fileLocationOriginal = "../resources/buchliste_origin.csv";

//Fragt den Nutzer nacheinander die Daten eines Buches ab.
//Gibt true zurück wenn alles funktioniert hat, falls ein Fehler auftritt wird das Programm abgebrochen und false zurückgegeben.
bool PSaddBook(){
    try{
        clearScreen();
        std::string input;
        struct book thisBook;
        struct book& thisBookRef = thisBook;
        std::cout << "Ah, ich freue mich, dass Sie der Bibliothek ein Buch spenden möchten.\n"<<
                    "Ich müsste nur zuerst ein paar Eckdaten abfragen. Wie lautet der Name des Buches?\n";
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
        std::cout << "Könnten Sie mir nun noch die ISBN sagen?\nSie finde sie meist auf der Rückseite, in der Nähe des Barcodes.\n";
        std::getline(std::cin, input);
        std::cin.ignore();
        thisBookRef.isbn = stoi(input);
        input.clear();
        clearScreen();
        std::cout << "Nun brächte ich nur noch den Preis.\nDen auf dem Buch bitte, nicht was Sie dafür gezahlt haben.\n";
        std::getline(std::cin, input);
        std::cin.ignore();
        thisBookRef.price = stof(input);
        input.clear();
        if(!addBook(thisBookRef, fileLocation)){
            throw std::runtime_error("UNABLE TO ADD BOOK FROM USER INPUT\n");
        }
        std::cout << "Vielen Dank, das war es auch schon.\nIch werde Ihr Buch so schnell wie möglich in unserer Datenbank anlegen.\nKann ich sonst noch etwas für Sie tun?\n\n";
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
                    << "\nAllerdings bitte nur eins davon, und Ihre Eingabe muss genau mit den Daten\n auf dem Buch übereinstimmen!\n";
        std::string input;
        std::cin >> input;
        std::cin.ignore();
        //nicht der effizienteste Weg die Bücher hier neu zu laden, aber muss erstmal reichen
        std::vector<struct book> books;
        std::vector<struct book>& booksRef = books;
        loadBooks(booksRef, fileLocation);
        if(!findBook(booksRef, foundBooks, input)){
            throw std::runtime_error("\nEs tut mir leid, ich konnte leider kein Buch finden, das zu Ihrer Beschreibung passt.\n");
        }
        if (foundBooks.size()>0){
            std::cout << "Ich habe folgende Bücher gefunden, die zu Ihrer Beschreibung passen:\n";
            printBooks(foundBooks);
            std::cout << "\nBitte wählen Sie anhand des Index eines dieser Bücher aus.\n";
            int index;
            std::cin >> input;
            index = stoi(input);
            struct book selected;
            std::vector<struct book>::iterator it = foundBooks.begin();
            it += index - 1;
            selected = *it;
            std::cout << "\nAlles klar, " << selected.title << " von " << selected.author << ".\n";
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
        std::cout << "Bitte wählen Sie aus, welches Buch Sie löschen möchten";
        struct book bookToDelete;
        struct book& bookToDeleteRef = bookToDelete;
        if(PSfindBook(bookToDeleteRef)){
            std::cout << "Das von Ihnen gewählte Buch wird aus unserer Sammlung entfernt.\n";
            return true;
        }else{
            throw std::runtime_error("\nEs tut mir leid, beim Suchen ist etwas schiefgegangen.");
        }
    }catch(std::runtime_error re){
        std::cerr << re.what();
    }
}

bool PSloadOriginal(){
    try{
        std::cout << "\nIch werde nun versuchen, unsere Datenbank wieder auf den Originalzustand zu setzen.\n";
        std::vector<struct book> booksOrigin;
        std::vector<struct book> booksOriginRef = booksOrigin;
        if(!loadBooks(booksOriginRef, fileLocationOriginal)){
            throw std::runtime_error("Etwas ist bei der Synchronisierung mit der Hauptdatenbank schiefgegangen. Es tut mir leid.");
            
        }
    }catch(std::runtime_error re){
        std::cerr << re.what();
    }
    return true;
}

//TODO: refactor functions to return errors instead of just bools
//Wählt eins der Oben stehenden Programme aus. Widerholt sich, bis der User das Programm beenden möchte.
bool selectProgram(){
    bool exit = true;
    while(!exit){
        try{
            std::vector<std::string>& programsRef = programs;
            drawMainMenu(programsRef);
            std::string input;
            std::cin >> input;
            int selection = std::stoi(input);
            switch (selection){
                case 1:{
                    clearScreen();
                    if(!PSaddBook()){
                        throw std::runtime_error("\nEtwas ist beim hinzufügen des Buches schiefgelaufen\n");
                    }
                }break;

                case 2:{
                    clearScreen();
                    if(!PSdeleteBook()){
                        throw std::runtime_error("\nEtwas ist beim löschen des Buches schiefgelaufen\n");
                        break;
                    }
                }break;

                case 3:{
                    clearScreen();
                    struct book tempBook;
                    struct book& tempBookRef = tempBook;
                    if(!PSfindBook(tempBookRef)){
                        throw std::runtime_error("\nEtwas ist beim durchsuchen der Bücher schiefgelaufen.\nEs tut mir leid.\n");
                        break;
                    }
                }break;
                case 4:{
                    clearScreen();
                    if(!loadOrigin(fileLocation, fileLocationOriginal)){
                        throw std::runtime_error("Ursprüngliche Datenbank konnte nicht wiederhergestellt werden");
                        break;
                    }
                }break;
                case 5:{
                    clearScreen();
                    exitMessage();
                    exit = true;
                }
            }
        }catch(std::runtime_error re){
            std::cerr << re.what();
        }
    }
}