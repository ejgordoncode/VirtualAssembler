// Written by E.J. Gordon 2025
#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <map>
#include "parser.h"
#include <string>
#include <vector>

using namespace std;



extern map<string, int> symTab;
extern map<string, string> opcodeTable;


size_t instructionSize(string opcode, string operand);

struct Pass1 {
    size_t LOCCTR;
    Pass1();
    void pass1(vector<Line> lines);
};

struct Pass2 {
    size_t LOCCTR;
    Pass2();
    void pass2(vector<Line> lines, const string& name);
};


void writeListing(ofstream& lfile, size_t address, const Line& line, const string& objectCode);



#endif