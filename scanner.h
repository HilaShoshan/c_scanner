#pragma once

#include <algorithm>
#include <regex>
#include "symtab.h"


class Scanner {
	ifstream& inputFile;
	SymbolTable& symTab;
	int lineno = 1;  // the current row 
    char ch;  // the next char
public:
	Scanner(ifstream& file, SymbolTable& tab) :
       		inputFile(file), symTab(tab) {}
	shared_ptr<Token> nextToken();
	/* this method reads the next char and updates the line number */
    bool nextChar() {  
	    inputFile.get(ch);
	    if (ch == '\n') ++lineno;
	    if (inputFile.eof()) return false;
	    return true;
    }
};
