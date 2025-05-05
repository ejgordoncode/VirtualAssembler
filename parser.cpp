// Written by E.J. Gordon 2025

#include "parser.h"
#include <iostream>
#include <cctype>
#include <string>
#include <sstream>

using namespace std;



Line::Line(string label, string opcode, string operand){
    this->label = label;
    this->opcode = opcode;
    this->operand = operand;
}

/**
 * Helper function used in parseLine to determine whether 
 *  a parsed line is a comment
 */
bool Line::isComment() const {
    if (!opcode.empty() && opcode[0] == '.'){
        return true;
    }
    return false;
}

Line parseLine(const string& lineToRead){
    // First Step: trim whitespace
    string trimmedLine = trimWhitespace(lineToRead);

    if (trimmedLine.empty()){ 
        return Line();
    }
    
    // Second Step: tokenize input
    istringstream ss(trimmedLine);
    string label, operand, opcode;

    // If you get label, opcode, and operand...
    if (ss >> label && ss >> opcode && ss >> operand){
        return Line(label, opcode, operand);
    }
    // If you get just opcode and operand...
    if (ss >> opcode && ss >> operand){
        return Line("", opcode, operand);
    }
    // If you get only opcode...
    if (ss >> opcode) {
        return Line("", opcode, "");
    }
    return Line(); // shouldn't hit this (still here in case no line is read, which shouldn't happen)
}


string trimWhitespace(const string& str){

    size_t first = str.find_first_not_of(" \t\n\r\f\v"); // find index of first char in str that is not whitespace

    if (first == string::npos){ 
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first) + 1); 
}