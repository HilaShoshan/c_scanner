#include "symtab.h" 


void SymbolTable::initReserved() { 
    string txt, token_num; 
    ifstream file ("reserved.txt");  // open the file
    if (file.is_open()) {
        while (file >> txt) {
            file >> token_num; 
            Token t((tokenType)stoi(token_num), txt);  // construct a Token
            shared_ptr<Token> token = make_shared<Token>(t);  // construct a shared_ptr to the Token
            this->symMap.insert({txt, token});  // add to the map 
        }  
        file.close();
    }
    else cout << "Unable to open file"; 
} 

shared_ptr<Token> SymbolTable::lookupToken(string text) {
    auto it = this->symMap.find(text); 
    if (it == symMap.end()) {  // not found
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
        if (it->second.get()->getType() != 258) { // not a variable -- don't print it
            it++; 
            continue; 
        }
        cout << "text: " << txt << endl; 
        shared_ptr<set<int>> lines = it->second.get()->getLines(); 
        cout << txt << "    "; 
        for (auto it2 = lines.get()->begin(); it2 != lines.get()->end(); ++it2)
            cout << *it2 << ' ';  // print the line and than a " "
        cout << endl; 
        it++;
    }
}

