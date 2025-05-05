// Written by E.J. Gordon - 2025

#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"
#include <map>
#include <sstream>
#include "assembler.h"
#include <iomanip>


using namespace std;


// Glocal symbol and opcode tables
Pass1::Pass1() {
    LOCCTR = 0;
}
Pass2::Pass2() {
    LOCCTR = 0;
}

map<string, int> symTab;
map<string, string> opcodeTable = {
    {"LDA", "00"}, {"LDX", "04"}, {"LDL", "08"},
    {"STA", "0C"}, {"STX", "10"}, {"STL", "14"},
    {"ADD", "18"}, {"SUB", "1C"}, {"MUL", "20"},
    {"DIV", "24"}, {"COMP", "28"}, {"TIX", "2C"},
    {"JEQ", "30"}, {"JGT", "34"}, {"JLT", "38"},
    {"JSUB", "48"}, {"RSUB", "4C"}, {"J", "3C"},
    {"TD", "E0"}, {"RD", "D8"}, {"WD", "DC"}
};

size_t instructionSize(string opcode, string operand){
    if (opcode == "WORD"){
        return 3;
    }
    if (opcode == "RESW"){
        return 3 * stoi(operand);
    }
    if (opcode == "RESB"){
        return stoi(operand);
    }
    if (opcode == "BYTE"){
        if (operand[0] == 'C'){ 
            return operand.size() - 3;
        }
        if (operand[0] == 'X'){
            return (operand.size() - 3) / 2;
        }
    }
    if (opcode == "END"){
        return 0;
    }
    else{
        return 3; // for LDA, JSUB, etc.
    }
}


void Pass1::pass1(vector<Line> lines){
    int index = 0;
    if (lines[0].opcode == "START"){
        stringstream startAddress(lines[0].operand);
        startAddress >> hex >> LOCCTR; // read hex val and store int val in LOCCTR
        index = 1;
    }
    
    for (int i = index; i < lines.size(); ++i){
        Line line = lines[i];

        if (!line.isComment()){
            if(!line.label.empty()){
                symTab[line.label] = LOCCTR;
            }
        }

        else{
            continue;
        }

        LOCCTR += instructionSize(line.opcode, line.operand);
    }

}

void Pass2::pass2(vector<Line> lines, const string& name){
    ofstream lfile(name + ".l");
    size_t currAdd = 0;
    string objectCode = "";

    if (lines[0].opcode == "START"){
        stringstream startAddress(lines[0].operand);
        startAddress >> hex >> currAdd; 
    }

    int index = 0;
    for(int i = index; i < lines.size(); ++i){
        Line line = lines[i];
        if (line.isComment()){
            continue;
        }

        objectCode = "";  

        if (line.opcode == "BYTE"){
            string directive = line.operand.substr(2, line.operand.size() - 3);
            if (line.operand[0] == 'C'){
                stringstream asciistr;
                for (int j = 0; j < directive.size(); ++j){
                    int asciiInt = static_cast<int>(directive[j]);
                    asciistr << hex << uppercase << setw(2) << setfill('0') << asciiInt;
                }
                objectCode = asciistr.str();
            }
            else if (line.operand[0] == 'X'){
                objectCode = directive;
                for (char& character : objectCode){
                    character = toupper(character);
                }
            }

            writeListing(lfile, currAdd, line, objectCode);


            cout << objectCode << endl;
            currAdd += instructionSize(line.opcode, line.operand);
        }

        else if (line.opcode == "WORD"){
            int operandInt = stoi(line.operand);
            stringstream asciistr;
            asciistr << hex << uppercase << setw(6) << setfill('0') << operandInt;    
            objectCode = asciistr.str();

            writeListing(lfile, currAdd, line, objectCode);


            cout << objectCode << endl;
            currAdd += instructionSize(line.opcode, line.operand);
        }

        else if (line.opcode == "RESW" || line.opcode == "RESB"){
            objectCode = "";
            writeListing(lfile, currAdd, line, objectCode);



            cout << "save space for " << line.operand << " "
                 << (line.opcode == "RESW" ? "words..." : "bytes...") << endl;
                 currAdd += instructionSize(line.opcode, line.operand);
        }

        else if (opcodeTable.count(line.opcode)){
            string opcode = opcodeTable[line.opcode];
            string address = "0000";

            if (line.opcode == "RSUB"){
                objectCode = opcode + "0000";
            }
            else if (symTab.count(line.operand)){
                int addressInt = symTab[line.operand];
                stringstream addressStream;
                addressStream << hex << uppercase << setw(4) << setfill('0') << addressInt;
                address = addressStream.str();
                objectCode = opcode + address;
            }

            writeListing(lfile, currAdd, line, objectCode);

            cout << line.opcode << "\t" << line.operand << "\t→\t" << objectCode << endl;
            currAdd += instructionSize(line.opcode, line.operand);
        }

        else if (line.opcode == "START" || line.opcode == "END"){
            objectCode = "";
            writeListing(lfile, currAdd, line, objectCode);

            if (line.opcode == "START")
                cout << "Starting address: " << line.operand << endl;
            else {
                cout << "End program..." << endl;
                break;
            }

            currAdd += instructionSize(line.opcode, line.operand);
        }
    }
    ofstream symfile(name + ".st");
    for (const auto& entry : symTab) {
        symfile << setw(10) << left << entry.first << "  "
                << setw(4) << setfill(' ') << hex << uppercase << entry.second << endl;
    }
    symfile.close();
    lfile.close();
}


// Helper function to write to the listing file a nicely formatted
//  output

void writeListing(ofstream& lfile, size_t address, const Line& line, const string& objectCode) {
    lfile << setw(4) << setfill('0') << hex << uppercase << address << "    "
          << setfill(' ')
          << setw(9) << left << line.label << " "
          << setw(9) << left << line.opcode << " "
          << setw(12) << left << line.operand << " "
          << objectCode << endl;
}

