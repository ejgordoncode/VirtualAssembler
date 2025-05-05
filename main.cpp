// Written by E.J. Gordon 2025

#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"
#include "assembler.h"
using namespace std;


int main (int argc, char* argv[]) {

    string filename = argv[1];
    int dotindex = filename.find('.');
    string name = (dotindex == string::npos) ? filename : filename.substr(0, dotindex);

    
    for (int i = 1; i < argc; ++i){
        ifstream file(argv[i]);
        if (file.is_open()){
            string line;
            vector<Line> parsedLines;
            while (getline(file, line)){
                
                Line parsed = parseLine(line);
                parsedLines.push_back(parsed);
                
            }

            
            file.close();
            Pass1 assembler;
            assembler.pass1(parsedLines);

            Pass2 secondPass;
            secondPass.pass2(parsedLines, filename);

        }
        

    }
    return 0;
}
