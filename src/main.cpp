/*
Programmentwurf von Julius Clausen
Ich versichere, dass dieses Programm ohne fremde Hilfe, KI,
oder anderen Quellen als dem Vorlesungsmaterial und meiner eigenen Erfahrung geschrieben wurde.
Begonnen 07.03.2025 15:20
Pause 18:20 - 18:40
Alpha Version ohne Komplilierung 21:20
Jetzt nur noch den Kompiler eingestellt kriegen...
first compile at 21:28 with only one error (mixup in function definition)
21:32 Programm zum ersten Mal gestartet. jetzte ans debuggen...
21:56 weiterhin probleme mit dem fileManager, Datei scheint nicht öffnen zu wollen
22:44 fileManager Problem gelöst, aus irgendeinem Grund funktionieren relative Pfade nicht
23:04 debugging der findBook funktion, da diese für viele Features notwendig ist (aktuelle Programme: add, delete, find) find is notwendig für delete und find
23:10 kein Bock mehr überall hinzuschreiben warum ich mit it = *.begin() + 1 starte, deswegen hier: in der ersten Zeile
    stehen die Labels für die Kategorien, deswegen muss ich die bei allen möglichen Aktionen überspringen
    allerdings kann ich die Zeile auch nicht einfach beim einlesen löschen, da sie ja schließlich eine ordentliche
    CSV Datei sein muss.
    (Nachtrag 01:41: das it+1 war in den meisten fällen nicht nötig/hat fehler verursacht. ich hab's an der quelle angepackt.)
23:12 in diesem Programmentwurf wurden keinerlei Leistungsteigernde Substanzen genutzt. 
23:13 Wodka Energy zählt nicht als leistungssteigernde Substanz
23:17 Fehler beim parsen stammt daher, dass die ISBN zu lang für einen Int ist. Muss das Programm auf long int umschreiben.
23:24 wechsel zu long long, da es mit long immer noch einen Fehler gab
23:26 long long funktioniert, allerdings muss ich price neu splitten, da "," nicht als dezimal-trennzeichen akzeptiert wird
01:05 parsing der bücher scheint zu funktionieren, trotzdem weiterhin fehler in der suchfunktion
01:07 "Die ""Caine"" wahr ihr Schicksal" Herr Prof. Dr. Grüning, ich ziehe meinen Hut, Sie haben es geschafft den wahrscheinlich
nervigst möglichen Datensatz aller Zeiten zu erschaffen.
Jetzt muss ich meine Titel-split funktion komplett überarbeiten
01:40 split-title funktion war (fast) nicht nötig, modulo rettet den Tag. Parsing und finde funktion läuft, jetzt geht es an die delete 
    Funktion
01:56 delete funktioniert, allerdings verschwinden jetzt die preise
02:19 delete, add und find funktionieren rudimentär, allerdings noch viele bigs zu beseitigen
02:52 Problem beim suchen nach Text, stoll und/oder stof crashen bei string als Eingabewert
03:07 Grundfunktionalität fertig. Wenn Zeit noch polieren und extras hinzufügen
TODO: bug zeigt bücher mit preis 0 (unbekannt) an wenn nach 0.99 gesucht wird
03:08 Ende für Heute. Gesamte Arbeitszeit: 
*/

#include <iostream>
#include <Windows.h>
#include "../include/programSelect.h"

int main() {
    SetConsoleCP ( 1252 ) ;
    SetConsoleOutputCP ( 1252 ) ;
    if(!selectProgram()){
        return 0;
    }else{
        return 1;
    }
}