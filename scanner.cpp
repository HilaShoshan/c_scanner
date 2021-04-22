#include "scanner.h"
#include <regex>


bool skip_spaces(Scanner &s, char* c) {
    while (*c == ' ' || *c == '\n' || *c == '\t' || *c == '\r') {  // skip all spaces
        if (!s.nextChar()) return false; 
    }
    return true; 
}

void skip_line_command(Scanner &s, char* c) {
    do {
        s.nextChar();  
    } while (*c != '\n');  // read chars till a '\n'
}

void skip_block_command(Scanner &s, char* c) {
    char prev;
    s.nextChar(); 
    while (*c != '*') {  
        s.nextChar(); 
    }
    prev = *c;  // '*'
    s.nextChar(); 
    while (prev != '*' || *c != '/') {  // not the end of the command
        prev = *c; 
        s.nextChar(); 
    }
}

tokenType getDoubleTokenType(char c) {
    switch (c) {
    case '+':
        return INC_OP; 
    case '-':
        return DEC_OP;
    case '&':
        return AND_OP; 
    case '|':
        return OR_OP;
    case '=':
        return EQ_OP; 
    default:
        break;
    }
}

tokenType getEqTokenType(char c) {
    switch (c) {
    case '<':
        return LE_OP; 
    case '>':
        return GE_OP; 
    case '!':
        return NE_OP;     
    default:
        break;
    }
}

string readNumber(Scanner &s, char* c) {
    string num_str = string(1,*c); 
    s.nextChar(); 
    while (isdigit(*c) || *c == '.' || *c == 'E' || *c == 'e') {
        num_str = num_str+(*c);  // add to the string
        s.nextChar();
    }
    return num_str; 
}

bool isValidNum(string num_str) {
    regex D("[0-9]");
    regex E("[Ee][+-]?{D}+"); 
    regex num1("[1-9]{D}*");
    regex num2("{D}+{E}");
    regex num3("{D}*'.'{D}+{E}?");
    regex num4("{D}+'.'{D}*{E}?"); 
    if (regex_match(num_str, num1) || regex_match(num_str, num2) || regex_match(num_str, num3) || regex_match(num_str, num4)) {
        return true; 
    }
    return false; 
}

bool isLetter(char c) {
    regex L("[a-zA-Z_]"); 
    if (regex_match(string(1,c),L)) {
        return true; 
    }
    return false; 
}

string readVariable(Scanner &s, char* c) {
    string num_var = string(1,*c); 
    regex D("[0-9]");
    regex L("[a-zA-Z_]"); 
    s.nextChar(); 
    while (regex_match(string(1,*c),D) || regex_match(string(1,*c),L)) {
        num_var = num_var+(*c);  // add to the string
        s.nextChar();
    }
    return num_var; 
}

string readString(Scanner &s, char* c) {
    string str = string(); 
    s.nextChar(); 
    while (*c != '"') {
        str = str+(*c); 
    }
    return str; 
}

/* this method implements the scanner */
shared_ptr<Token> Scanner::nextToken() { 
    // this->ch = '\0'; 
    nextChar();  
    while (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' || ch == '/') {
        if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {  
            bool flag = skip_spaces(*this, &ch); 
            if (!flag) return nullptr; 
        }
        if (this->ch == '/') {  
            nextChar(); 
            if (this->ch == '/') {  // start a command in form of "//"
                skip_line_command(*this, &ch); 
                nextChar();  // read the char after the command (and the end of line)
            }
            else if (this->ch == '*') {  // a command in form of "/* ... */"
                skip_block_command(*this, &ch); 
                nextChar();  // read the char after the command
            }
            else {
                inputFile.unget();  // it was not a command, just "/"
                break;
            }
        }
    }
    char curr; 
    switch (ch) {
    // each character represents itself (not a start of another token)
    case ';' : case '{' : case '}' : case ',' : case ':' : \
    case '(' : case ')' : case '[' : case ']' : case '~' : \
    case '*' : case '%' : case '^' : case '?' : case '/':
        return shared_ptr<Token>(new Token(static_cast<tokenType>(ch),string(1,ch)));
        break;
    // characters that can be duplicate 
    case '+' : case '-' : case '&' : case '|' : case '=':
        curr = ch; 
        nextChar(); 
        if (this->ch == curr) {
            tokenType tt = getDoubleTokenType(ch); 
            return shared_ptr<Token>(new Token(tt,string(2,ch))); 
        }
        else if (curr == '-' && ch == '>') {
            return shared_ptr<Token>(new Token(PTR_OP,string(1,curr)+ch)); 
        }
        else {
            inputFile.unget(); 
            return shared_ptr<Token>(new Token(static_cast<tokenType>(ch),string(1,ch)));
        }
        break; 
    // characters that can appear on their own or with a '=' after 
    case '<' : case '>' : case '!': 
        curr = ch; 
        nextChar(); 
        if (this->ch == '=') {
            tokenType tt = getEqTokenType(curr); 
            return shared_ptr<Token>(new Token(tt,string(1,curr)+ch)); 
        }
        else {
            inputFile.unget(); 
            return shared_ptr<Token>(new Token(static_cast<tokenType>(ch),string(1,ch)));
        }
        break;
    }
    if (isdigit(ch)) {
        string num_str = readNumber(*this, &ch);
        inputFile.unget();
        if (isValidNum(num_str)) {
            return shared_ptr<Token>(new Token(CONSTANT,num_str));
        }
        else {
            return shared_ptr<Token>(new Token(ERROR,num_str));
        }
    }
    if (isLetter(ch)) {
        string var_str = readVariable(*this, &ch); 
        inputFile.unget();
        shared_ptr<Token> token_ptr = symTab.lookupToken(var_str); 
        if (token_ptr != nullptr) {
            tokenType tt = token_ptr.get()->getType(); 
            if (tt == IDENTIFIER) {
                token_ptr.get()->add_line(lineno);
            }
            else {
                return token_ptr; 
            }
        }
        else {  // the token not in the symbol table
            varToken var(var_str); 
            shared_ptr<varToken> var_ptr = make_shared<varToken>(var);
            symTab.insertToken(var_str, var_ptr);
            var.add_line(lineno);
            return var_ptr; 
        }
    }
    if (ch == '\'') {
        nextChar(); 
        char c = ch; 
        nextChar(); 
        if (ch == '\'') {
            return shared_ptr<Token>(new Token(CONSTANT,string(1,c)));
        }
        else {
            return shared_ptr<Token>(new Token(ERROR,string(1,c)));
        }
    }
    if (ch == '"') {
        string str = readString(*this, &ch); 
        return shared_ptr<Token>(new Token(STRING_LITERAL,str));
    }
    return nullptr; 
}

