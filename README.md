ESSY - A two-pass SIC/XE model assembler

--- 

Hi! This project was created by me, E.J. Gordon, in 2025.
It simulates a SIC/XE two-pass assembler (Software Systems, Leland) and returns a listing file and a SYMTAB file. 

---

There are five components:
    - main.cpp : handles cmd-line args and file io
    - assembler.cpp : cleans and tokenizes lines of assembly code
    - parser.cpp : implements pass 1 and pass 2
    - SYMTAB.st : symtab dumn file
    - listing.l : output file that shows memory address + obj code for each instruction


To compile this, input the following into the terminal:
'''bash
    g++ -std=c++11 main.cpp assembler.cpp parser.cpp -o assembler

---
