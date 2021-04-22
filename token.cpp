#include "token.h"
#include "tokenstrings.h"


tokenType Token::getType() {
    return this->type; 
}

string Token::getText() {
    return this->text; 
}

void Token::printToken() {
    if (this->type < 258) {
        cout << (char)type << " " << this->text << endl; 
    }
    else {
        cout << tokenStrings[type-258] << " " << this->text << endl;  // print the enam-name of the value
    }   
}
 
void varToken::add_line(int l) {
    this->lines.get()->insert(l); 
}

shared_ptr<set<int>> varToken::getLines() {
    return this->lines; 
}

