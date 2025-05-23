#include <vector>
#include <iostream>
#include <fstream>
#include <string>

//liest die Datei mit dem eingegebenen Dateinamen Zeile für Zeile in den referenzierten Vector aus Strings.
//gibt true zurück wenn das einlesen erfolgreich war, ansonsten false
std::exception* readFile(std::vector<std::string>& lines, std::string fileLocation){
    std::fstream file(fileLocation, std::ios::in);
    try{
        std::string line;
        if(!file.is_open()){
            throw new std::runtime_error("FILE COULT NOT BE OPENED\n");
            std::cout << "ERROR";
        }
        while(getline(file, line)){
                lines.push_back(line);
                if(file.bad()){
                    throw new std::runtime_error("ENCOUNTERED ISSUE WHILE READING LINES\n");
                }            
        }
        if(lines.size() <= 0){
            throw new std::runtime_error("NO LINES IN FILE FOUND\n");
        }
    }
    catch(std::runtime_error* re){
        file.close();
        return re;
    }
    file.close();
    return nullptr;
}

//Überschreibt die Datei in dem angegebenen Pfad, oder erzeugt eine neue Datei fall die Datei noch nicht existiert
//gibt true zurück, wenn das schreiben erfolgreich war, false falls nicht.
std::exception* writeFile(std::vector<std::string>& lines, std::string fileLocation){
    std::fstream file(fileLocation, std::ios::out);
    try{
        if (!file.is_open()){
            throw new std::runtime_error("FILE TO WRITE COULD NOT BE OPENED\n");
        }
        std::string line;
        std::vector<std::string>::iterator it;
        for(it = lines.begin(); it != lines.end(); it++){
            file << *it << "\n";
        }
    }
    catch(std::runtime_error* re){
        file.close();
        return re;
    }
    file.close();
    return nullptr;
}

//fügt eine Zeile am Ende der angegeben Datei ein
//gibt true zurück, wenn keine Fehler aufgetreten sind, ansonsten false
std::exception* appendFile(std::string line, std::string fileLocation){
    std::fstream file(fileLocation, std::ios::app);
    try{
        if(!file.is_open()){
            throw new std::runtime_error("FILE TO APPEND COULD NOT BE OPENED\n");
        }
        file << line;
    }
    catch(std::runtime_error* re){
        file.close();
        return re;
    }
    file.close();
    return nullptr;
}