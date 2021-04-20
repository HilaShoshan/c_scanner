#include "symtab.h"

int main() {

    Token t1((tokenType)43, "+");  
    t1.printToken(); 
    Token t2((tokenType)291, "int");
    t2.printToken(); 
    varToken t3("x"); 
    t3.add_line(2); 
    t3.add_line(15); 
    t3.printToken(); 
    varToken t4("hila"); 
    t4.add_line(6); 
    t4.add_line(5); 
    t4.add_line(2000); 
    t4.printToken(); 

    SymbolTable s; 
    s.insertToken("x", make_shared<varToken>(t3)); 
    s.insertToken("hila", make_shared<varToken>(t4)); 
    s.xref(); 
    return 0; 
}