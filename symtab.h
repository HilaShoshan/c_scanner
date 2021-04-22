#pragma once

#include <map>
#include "token.h"


class SymbolTable {
	map<string, shared_ptr<Token>> symMap;  // map of variables and reserved words only
	void initReserved();  // init the map (table) from "reserved.txt" file
public:
	SymbolTable () { initReserved(); }
	shared_ptr<Token> lookupToken(string text);  // search the (variable) word in the table
	void insertToken(string text, shared_ptr<Token> tokenp);  // add a new word to the table
	void xref();  // print the variables (not the reserved words) in the symbol-table
};

