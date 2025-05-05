// Written by E.J. Gordon 2025

#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <string>


using namespace std;

class Line{
public:
    string label;
    string opcode;
    string operand;

    Line(string label = "", string opcode = "", string operand = "");

    bool isComment() const; 
    
};
Line parseLine(const string& lineToRead);
string trimWhitespace(const string& line);

#endif