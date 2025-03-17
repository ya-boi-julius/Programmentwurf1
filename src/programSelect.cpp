#include <iostream>
#include <vector>
#include <string>
#include "../include/UIhandler.h"
#include "../include/book.h"
#include "../include/saveLoadManager.h"

std::vector<std::string> programs = {"Alle Bücher anzeigen", "Buch hinzufügen", "Buch löschen", "Buch suchen", "Originalbestand herstellen", "Bibliothek verlassen"};
std::string fileLocation = "resources/buchliste_current.csv";
std::string fileLocationOriginal = "resources/buchliste_origin.csv";

//Fragt den Nutzer nacheinander die Daten eines Buches ab.
//Gibt true zurück wenn alles funktioniert hat, falls ein Fehler auftritt wird das Programm abgebrochen und false zurückgegeben.
std::exception* PSaddBook(){
    try{
        clearScreen();
        std::string input;
        struct book thisBook;
        struct book& thisBookRef = thisBook;
        std::cout << "Ah, ich freue mich, dass Sie der Bibliothek ein Buch spenden müchten.\n"<<
                    "Ich müsste nur zuerst ein paar Eckdaten abfragen. Falls Sie etwas nicht wissen geben Sie bitte \"Unbekannt\" an.\n"
                    <<"Wie lautet der Name des Buches?\n";
        std::getline(std::cin, input);
        if(input.size() == 0){
            throw new std::invalid_argument("Ihre Eingabe war leider ungültig. Bitte versuchen Sie es erneut.\n");
        }
        thisBookRef.title = input;
        input.clear();
        clearScreen();
        std::cout << "Vielen Dank. Wer ist der Autor?\n";
        std::getline(std::cin, input);
        if(input.size() == 0){
            throw new std::invalid_argument("Ihre Eingabe war leider ungültig. Bitte versuchen Sie es erneut.\n");
        }
        thisBookRef.author = input;
        input.clear();
        clearScreen();
        std::cout << "Und wissen Sie auch, wann das Buch erschienen ist?\nBitte geben Sie das Datum im Format dd.mm.yyyy an.\n";
        std::getline(std::cin, input);
        if(input.size() == 0){
            throw new std::invalid_argument("Ihre Eingabe war leider ungültig. Bitte versuchen Sie es erneut.\n");
        }
        thisBookRef.yearString = input;
        input.clear();
        clearScreen();
        std::cout << "Künnten Sie mir nun noch die ISBN sagen?\nSie finde sie meist auf der Rückseite, in der Nühe des Barcodes.\n";
        std::getline(std::cin, input);
        if(input.size() == 0){
            throw new std::invalid_argument("Ihre Eingabe war leider ungültig. Bitte versuchen Sie es erneut.\n");
        }
        thisBookRef.isbnString = input;
        input.clear();
        clearScreen();
        std::cout << "Nun bräuchte ich nur noch den Preis.\nDen auf dem Buch bitte, nicht was Sie dafür gezahlt haben.\n";
        std::getline(std::cin, input);
        if(input.size() == 0){
            throw new std::invalid_argument("Ihre Eingabe war leider ungültig. Bitte versuchen Sie es erneut.\n");
        }
        thisBookRef.priceString = input;
        input.clear();
        std::exception* err = addBook(thisBookRef, fileLocation);
        if(err != nullptr){
            throw new std::runtime_error("Oh, entschuldigen Sie bitte, anscheinend gab es ein Problem beim anlegen Ihres Buches."
                    "\nSind Sie sicher, dass Sie sich nicht vielleicht irgendwo vertippt haben?\n"
                    "Versuchen Sie es bitte nocheinmal.\n\n");
        }
        std::cout << "Vielen Dank, das war es auch schon.\n"
            << "Ich werde Ihr Buch so schnell wie möglich in unserer Datenbank anlegen.\n"
            << "Kann ich sonst noch etwas für Sie tun?\n";
            pauseForEnter();
    }catch(std::exception* re){
        return re;
    }
    return nullptr;
}

std::exception* PSfindBook(struct book& foundBook, bool toDelete){
    try{
        clearScreen();
        std::vector<struct book> foundBookList;
        std::vector<struct book>& foundBooks = foundBookList;
        std::cout << "Ich kann Ihnen helfen, ein bestimmtes Buch zu finden."
                    << "\nSagen Sie mir bitte den Titel, Autor, ISBN, Preis, oder das Erscheinungsdatum."
                    << "\nAllerdings bitte nur eins davon, und Ihre Eingabe muss genau mit den Daten\n auf dem Buch übereinstimmen!\n";
        std::string input;
        std::getline(std::cin, input);
        //nicht der effizienteste Weg die Bücher hier neu zu laden, aber muss erstmal reichen
        std::vector<struct book> books;
        std::vector<struct book>& booksRef = books;
        std::exception* err = loadBooks(booksRef, fileLocation);
        if(err != nullptr){throw err;}
        err = findBook(booksRef, foundBooks, input);
        if(err != nullptr){
            throw err;
        }
            //08.03.2025 00:07
            //findBooks läuft endlich ohne crashes durch. wer wei?, was jetzt noch kommt
        if (foundBooks.size()>0){
            std::cout << "Ich habe folgende Bücher gefunden, die zu Ihrer Beschreibung passen:\n";
            printBooks(foundBooks);
            if(toDelete){
                bool bookSelected = false;
                std::cout << "\nBitte wählen Sie anhand des Index eines dieser Bücher aus.\n";
                while(!bookSelected){
                    try{
                        int index;
                        std::getline(std::cin, input);
                        index = stoi(input);
                        std::vector<struct book>::iterator it = foundBooks.begin();
                        if(index > foundBooks.size()){
                            throw new std::invalid_argument("");
                        }
                        it += index - 1;
                        foundBook = *it;
                        bookSelected = true;
                    }catch(std::invalid_argument* ia){
                        std::cout << "Das ist leider kein gültiger Index. Bitte versuchen Sie es erneut.\n";
                    }
                }
                std::cout << "\nAlles klar, " << foundBook.title << " von " << foundBook.author << ".\n";
            }
        }
    }catch(std::runtime_error* re){
        return re;
    }
    return nullptr;
}

std::exception* PSdeleteBook(){
    try{
        std::cout << "Bitte wählen Sie aus, welches Buch Sie löschen möchten";
        struct book bookToDelete;
        struct book& bookToDeleteRef = bookToDelete;
        std::exception* err = PSfindBook(bookToDeleteRef, true);
        if(err != nullptr){throw err;}
        std::vector<struct book> books;
        std::vector<struct book>& booksRef = books;
        err = loadBooks(booksRef, fileLocation);
        if(err != nullptr){throw err;}
        err = removeBook(booksRef, bookToDeleteRef);
        if(err != nullptr){throw err;}
        err = saveBooks(booksRef, fileLocation);
        if(err != nullptr){throw err;}
    }catch(std::exception* re){
        return re;
    }
    std::cout << "Das von Ihnen gewählte Buch wurde aus der Datenbank gelöscht.\n";
    pauseForEnter();
    return nullptr;
}

std::exception* PSloadOriginal(){
    try{
        std::cout << "\nIch werde nun versuchen, unsere Datenbank wieder auf den Originalzustand zu setzen.\n";
        std::exception* err = loadOrigin(fileLocationOriginal, fileLocation);
        if(err != nullptr){
            throw new std::runtime_error("Etwas ist bei der Synchronisierung mit der Hauptdatenbank schiefgegangen. Es tut mir leid.");
        }
    }catch(std::runtime_error* err){
        return err;
    }
    std::cout << "Die Bücher wurden mit der Hauptdatenbank synchronisiert.\n";
    pauseForEnter();
    return nullptr;
}

std::exception* PSallBooks(){
    std::vector<struct book> books;
    std::vector<struct book>& booksRef = books;
    try{
        std::exception* err = loadBooks(booksRef, fileLocation);
            if(err != nullptr){
                throw new std::runtime_error("Ich war leider nicht in der Lage die Bücher zu laden, überprüfen Sie bitte, ob die lokale Buchliste angelegt wurde.\n");
            }
        printBooks(booksRef);
        pauseForEnter();
    }catch(std::exception* re){
        return re;
    }
    return nullptr;
}

//TODO: refactor functions to return errors instead of just bools
//Wählt eins der Oben stehenden Programme aus. Widerholt sich, bis der User das Programm beenden müchte.
void selectProgram(){
    bool exit = false;
    while(!exit){
        try{
            std::vector<std::string>& programsRef = programs;
            drawMainMenu(programsRef);
            std::string input;
            std::getline(std::cin, input);
            int selection = std::stoi(input);
            switch (selection){
                case 1:{
                    if(PSallBooks() != nullptr){
                        throw new std::runtime_error("Leider ist etwas schiefgelaufen. Bitte versuchen Sie es später nochmal\n");
                    }
                }break;
                case 2:{
                    clearScreen();
                    std::exception* err = PSaddBook();
                    if(err != nullptr){
                        throw err;
                    }
                }break;

                case 3:{
                    clearScreen();
                    std::exception* err = PSdeleteBook();
                    if(err != nullptr){
                        throw err;
                    }
                }break;

                case 4:{
                    clearScreen();
                    struct book tempBook;
                    struct book& tempBookRef = tempBook;
                    if(PSfindBook(tempBookRef, false) != nullptr){
                        throw new std::runtime_error("Leider konnten wir Ihr gesuchtes Buch nicht finden. Bitte versuchen Sie es erneut.\n");
                    }
                    pauseForEnter();
                }break;
                case 5:{
                    clearScreen();
                    std::exception* err = PSloadOriginal();
                    if(err != nullptr){
                        throw err;
                        break;
                    }
                }break;
                case 6:{
                    clearScreen();
                    exitMessage();
                    exit = true;
                }
            }
        }catch(std::invalid_argument* ia){
            std::cerr << "Das ist leider keine gültige Programmauswahl.\n"
                << "Versuchen Sie es bitte erneut.\n";
            pauseForEnter();
        }catch(std::exception* re){
            std::cerr << (*re).what();
            pauseForEnter();
        }
    }
}