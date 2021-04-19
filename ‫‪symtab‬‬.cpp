#include "symtab.h" 
#include <fstream>

void SymbolTable::initReserved() { 
    string txt, token_num; 
    ifstream file ("reserved.txt");
    if (file.is_open()) {
        while (file >> txt) {
            file >> token_num; 
            Token t((tokenType)stoi(token_num), txt); 
            shared_ptr<Token> token = make_shared<Token>(t); 
            this->symMap.insert({txt, token});
        }  
        file.close();
    }
    else cout << "Unable to open file"; 
} 

shared_ptr<Token> SymbolTable::lookupToken(string text) {
    auto it = this->symMap.find(text); 
    // it->second.get()->add_line(line_num);
    if (it == symMap.end()) {  // not found
        // insertToken(text, ); ??
        return nullptr; 
    }   
    else {  
        return it->second;  
    }  
}

void SymbolTable::insertToken(string text, shared_ptr<Token> tokenp) {
    this->symMap.insert({text, tokenp});
}

void SymbolTable::xref() {
    map<string, shared_ptr<Token>>::iterator it = this->symMap.begin();
    while (it != symMap.end()) {
        string txt = it->first;  // get the key 
        shared_ptr<Token> token = it->second;  // get the value 
        shared_ptr<set<int>> lines = token.get()->getLines(); 
        cout << txt << "    "; 
        for (auto it = lines.get()->begin(); it != lines.get()->end(); ++it)
            cout << *it << ' ';
        cout << endl; 
        it++;
    }
}

