#include <iostream>
#include <vector>
#include <string>
#include "../include/UIhandler.h"
#include "../include/book.h"
#include "../include/saveLoadManager.h"
#include "../include/bookManager.h"

std::vector<std::string> programs = {"Alle Bücher anzeigen", "Buch hinzufügen", "Buch löschen", "Buch suchen", "Originalbestand herstellen", "Bücher sortieren", "Buchliste neu laden", "Buchliste speichern", "Bibliothek verlassen"};
std::vector<std::string> sortModes = {"Nach Datum (aktuell fehlerbehaftet)", "Nach Autor", "Nach Titel", "Abbrechen"};
std::string fileLocation = "resources/buchliste_current.csv";
std::string fileLocationAlt = "../resources/buchliste_current.csv";
std::string fileLocationOriginal = "resources/buchliste_origin.csv";
std::string fileLocationOriginalAlt = "../resources/buchliste_origin.csv";
//Beide werden hier oben initialisiert, damit sie nicht für jede Operation neu geladen werden müssen.
std::vector<struct book> books;
std::vector<struct book>& booksRef = books;

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
        booksRef.push_back(thisBookRef);
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
        std::exception* err = findBook(booksRef, foundBooks, input);
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
                        unsigned int index;
                        std::getline(std::cin, input);
                        index = stoul(input);
                        std::vector<struct book>::iterator it = foundBooks.begin();
                        if(index > foundBooks.size()){
                            throw std::invalid_argument("");
                        }
                        it += index - 1;
                        foundBook = *it;
                        bookSelected = true;
                    }catch(std::invalid_argument& ia){
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

std::exception* PSallBooks(){
    try{
        printBooks(booksRef);
        pauseForEnter();
    }catch(std::exception* re){
        return re;
    }
    return nullptr;
}

std::exception* PSsortByDate(){
    try{
        clearScreen();
        std::cout << "Möchten Sie die Bücher in aufsteigender oder absteigender Reihenfolge sortieren?\n"
            <<"Für Neu-Alt wählen Sie 1\nFür Alt-Neu wählen Sie 2.\n";
        bool valid = false;
        std::string input;
        int choice = 0;
        while(!valid){    
            std::getline(std::cin, input);
            choice = std::stoi(input);
            if(choice > 3 || choice < 1){
                throw std::invalid_argument("");
            }else if(choice == 3){
                return nullptr;
            }
            valid = true; 
        }
        choice--;
        std::exception* err = sortByDate(booksRef, choice);
        if (err != nullptr){throw err;}
        err = saveBooks(booksRef, fileLocation);
        if (err != nullptr){throw err;}
    }catch(std::invalid_argument& ia){
        std::cerr << "Diese Eingabe konnte leider nicht erkannt werden.\nVersuchen Sie es erneut, oder wählen Sie 3 um das sortieren zu beenden.\n";
    }catch(std::exception* err){
        return err;
    }
    return nullptr;
}

std::exception* PSsortByString(int mode){
    try{
        clearScreen();
        std::cout << "Möchten Sie die Bücher in aufsteigender oder absteigender Reihenfolge sortieren?\n"
            <<"Für A-Z wählen Sie 1\nFür Z-A wählen Sie 2.\n";
        bool valid = false;
        std::string input;
        int choice = 0;
        while(!valid){    
            std::getline(std::cin, input);
            choice = std::stoi(input);
            if(choice > 3 || choice < 1){
                throw std::invalid_argument("");
            }else if(choice == 3){
                return nullptr;
            }
            valid = true; 
        }
        std::exception* err;
        choice--;
        switch(mode){
            case 0:{
                err = sortByAuthor(booksRef, choice);
            }break;
            case 1:{
                err = sortByTitle(booksRef, choice);
            }break;
            default: {
                throw new std::invalid_argument("Etwas ist bei der Auswahl des Sortiermodus fehlgeschlagen.\n");
            }
        }
        
        if (err != nullptr){throw err;}
    }catch(std::invalid_argument& ia){
        std::cerr << "Diese Eingabe konnte leider nicht erkannt werden.\nVersuchen Sie es erneut, oder wählen Sie 3 um das sortieren zu beenden.\n";
    }catch(std::exception* err){
        return err;
    }
    return nullptr;
}

std::exception* PSsort(){
    std::vector<std::string>::iterator it;
    int index = 1;
    clearScreen();
    try{
        for(it = sortModes.begin(); it != sortModes.end(); it++){
            std::cout << index << ": " << *it << "\n";
            index++;
        }
        std::string input;
        bool valid = false;
        int choice = 0;
        while(!valid){
            std::getline(std::cin, input);
            choice = stoi(input) - 1;
            if(choice < 0 || choice > sortModes.size()){
                throw std::invalid_argument("");
            }
            valid = true;
        }
        switch(choice){
            case 0:{
                std::exception* err = PSsortByDate();
                if (err != nullptr){throw err;}
            }break;
            case 1:{
                std::exception* err = PSsortByString(0); //sort by author
                if (err != nullptr){throw err;}
            }break;
            case 2:{
                std::exception* err = PSsortByString(1); //sort by title
                if (err != nullptr){throw err;}
            }break;
            default:{
                return nullptr;
            }
        }
        std::cout << "Sortieren erfolgreich beendet\n";
        pauseForEnter();
    }catch(std::invalid_argument& ia){
        std::cout << "Das ist leider keine gültige Auswahl, bitte versuchen Sie es erneut.\n";
    }catch(std::exception* err){
        return err;
    }
    return nullptr;
}

std::exception* PSloadBooks(){
    try{
        booksRef.clear();
        std::exception* err = loadBooks(booksRef, fileLocation);
        if(err != nullptr){
            err = loadBooks(booksRef, fileLocationAlt);
            if(err != nullptr){throw err;}
        }
    }catch(std::exception* err){
        return err;
    }
    return nullptr;
}

std::exception* PSsaveBooks(){
    try{
        std::exception* err = saveBooks(booksRef, fileLocation);
        if(err != nullptr){throw err;}
    }catch(std::exception* err){
        return err;
    }
    return nullptr;
}

std::exception* PSloadOriginal(){
    try{
        std::cout << "\nIch werde nun versuchen, unsere Datenbank wieder auf den Originalzustand zu setzen.\n";
        std::exception* err = loadOrigin(fileLocationOriginal, fileLocation);
        if(err != nullptr){
            std::cout << "Es ist ein Fehler aufgetreten. Ein Moment bitte, ich versuche etwas anderes.\n";
            //dieser Block kompensiert ein Problem, dass bei mir aufgetretem ist, wodurch verschiedene Compiler mit
            //verschiedenen Dateipfaden klarkommen.
            err = loadOrigin(fileLocationOriginalAlt, fileLocationAlt);
            if(err == nullptr){
                fileLocation = fileLocationAlt;
                fileLocationOriginal = fileLocationOriginalAlt;
                pauseForEnter();
            }else{
                throw new std::runtime_error("Etwas ist bei der Synchronisierung mit der Hauptdatenbank schiefgegangen.\nEs tut mire leid\n");
            }

        }
        err = PSloadBooks();
        if(err != nullptr){throw err;}
    }catch(std::runtime_error* err){
        return err;
    }
    std::cout << "Die Bücher wurden mit der Hauptdatenbank synchronisiert.\n";
    pauseForEnter();
    return nullptr;
}

std::exception* PSstartup(){
    try{
        std::exception* err = PSloadBooks();
        if(err != nullptr){
            try{
                std::cout << "Leider gab es ein Problem beim Laden der Buchliste.\n"<<
                    "Möglicherweise existiert die Liste noch nicht.\n"<<
                    "Möchten Sie die lokale Liste aus unserer Haupdatenbank anlegen?\n"<<
                    "1: Ja\n2: Nein\n";
                std::string input;
                std::getline(std::cin, input);
                int choice = std::stoi(input);
                if(choice == 2){
                    std::cout << "Wie Sie wünschen. Das Programm wird mit einer leeren Buchliste gestartet.\n"<<
                        "Bitte beachten Sie, dass dies zu unerwartetem Verhalten führen kann, bzw. manche Funktionalitäten nicht ausführbar sein werden.\n"<<
                        "Wilkommen in der Bibliothek.\n";
                        pauseForEnter();
                    booksRef = {};
                    return nullptr;
                }else{
                    err = PSloadOriginal();
                    if(err != nullptr){throw err;}
                    std::cout << "Die Buchliste wurde aus der Hauptdatenbank übernommen.\nWilkommen in der Bibliothek!\n";
                    pauseForEnter();
                    return nullptr;
                }
            }catch(std::invalid_argument* ia){
                return ia;
            }catch (std::exception* er){
                return err;
            }
        }
        return nullptr;
    }catch(std::exception* err){
        return err;
    }
}

//Wählt eins der Oben stehenden Programme aus. Widerholt sich, bis der User das Programm beenden müchte.
void selectProgram(){
    pauseForEnter();
    bool exit = false;
    std::exception* err = PSstartup();
    if(err != nullptr){
        std::cerr << "Es tut mir leid, leider ist ein fataler Fehler aufgetreten.\n"<<
        "Bitte wenden Sie sich an Julius Clausen, um die Ursache des Problems zu finden.\n"<<
        "Das Programm wird sich nun selbst beenden.\n";
        pauseForEnter();
        return;
    }
    while(!exit){
        try{
            std::vector<std::string>& programsRef = programs;
            drawMainMenu(programsRef);
            std::string input;
            std::getline(std::cin, input);
            if(input.length() == 0){
                throw new std::invalid_argument("");
            }
            int selection = std::stoi(input);
            switch (selection){
                case 1:{
                    if(PSallBooks() != nullptr){
                        throw new std::runtime_error("Leider ist etwas schiefgelaufen. Bitte versuchen Sie es später nochmal\n");
                    }
                }break;
                case 2:{
                    clearScreen();
                    err = PSaddBook();
                    if(err != nullptr){
                        throw err;
                    }
                }break;

                case 3:{
                    clearScreen();
                    err = PSdeleteBook();
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
                    err = PSloadOriginal();
                    if(err != nullptr){
                        throw err;
                    }
                }break;
                case 6:{
                    err = PSsort();
                    if (err != nullptr){throw err;}
                }break;
                case 7:{
                    err = PSloadBooks();
                    if (err != nullptr){throw err;}
                }break;
                case 8:{
                    err = PSsaveBooks();
                    if (err != nullptr){throw err;}
                }break;
                case 9:{
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