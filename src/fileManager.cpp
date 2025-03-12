#include <vector>
#include <iostream>
#include <fstream>
#include <string>

//liest die Datei mit dem eingegebenen Dateinamen Zeile für Zeile in den referenzierten Vector aus Strings.
//gibt true zurück wenn das einlesen erfolgreich war, ansonsten false
bool readFile(std::vector<std::string>& lines, std::string fileLocation){
    std::fstream file(fileLocation, std::ios::in);
    try{
        if(!file.is_open()){
            throw std::runtime_error("FILE COULT NOT BE OPENED\n");
        }
        std::string line;
        while(getline(file, line)){
                lines.push_back(line);
                if(file.bad()){
                    throw std::runtime_error("ENCOUNTERED ISSUE WHILE READING LINES\n");
                }            
        }
        if(lines.size() <= 0){
            return false;
        }
    }
    catch(std::runtime_error re){
        std::cerr << re.what(); 
        file.close();
        return false;
    }
    file.close();
    return true;
}

//Überschreibt die Datei in dem angegebenen Pfad, oder erzeugt eine neue Datei fall die Datei noch nicht existiert
//gibt true zurück, wenn das schreiben erfolgreich war, false falls nicht.
bool writeFile(std::vector<std::string>& lines, std::string fileLocation){
    std::fstream file(fileLocation, std::ios::out);
    try{
        if (!file.is_open()){
            throw std::runtime_error("FILE TO WRITE COULD NOT BE OPENED\n");
        }
        std::string line;
        std::vector<std::string>::iterator it;
        for(it = lines.begin(); it != lines.end(); it++){
            file << *it;
        }
    }
    catch(std::runtime_error re){
        std::cerr << re.what();
        file.close();
        return false;
    }
    file.close();
    return true;
}

//fügt eine Zeile am Ende der angegeben Datei ein
//gibt true zurück, wenn keine Fehler aufgetreten sind, ansonsten false
bool appendFile(std::string line, std::string fileLocation){
    std::fstream file(fileLocation, std::ios::app);
    try{
        if(!file.is_open()){
            throw std::runtime_error("FILE TO APPEND COULD NOT BE OPENED\n");
        }
        file << line;
    }
    catch(std::runtime_error re){
        std::cerr << re.what();
        file.close();
        return false;
    }
    file.close();
    return true;
}