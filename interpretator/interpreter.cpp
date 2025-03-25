#include <iostream>
#include <ctype.h>
#include <vector>
#include <stack>
#include <string>
#include <cstring>
#include <algorithm>

enum token_type {
    Null,                                                   //0
    Not,    And,    Or,     If,    Else,                    //5
    For,    Do,     While,  Break, Goto,                    //10
    Read,   Write,  True,   False,                          //14
    Int,    String, Bool,   Struct,                         //18
    Program,                                                //19
    Fin,                                                    //20
    Times, Slash,   Plus,   Minus,  Assign,                 //25
    Lss,   Gtr,     Leq,    Geq,    Equal,   Neq,           //31
    Semicolon,      Colon,  Comma,  Point,                  //35
    LPar,           RPar,   LBrace, RBrace,                 //39
    Num, Str,                                               //41
    Id,                                                     //42
    Poliz_label,                                            //43
    Poliz_address,                                          //44
    Poliz_go,                                               //45
    Poliz_fgo,                                              //46
    Poliz_minus,                                            //47
    Object                                                  //48
};


//Token that you can get type, text and value of
class Token
{
    token_type type;
    std::string text;
    int value, var;
public:
    Token(token_type tpe = Null, int val = 0, int v = -1, std::string str = "")
        : type(tpe), text(str), value(val), var(v) {}
    Token(const Token& op) {
        type = op.type;
        text = op.text;
        value = op.value;
        var = op.var;
    }
    int getValue() const { return value; }
    int getVariable() const { return var; }
    std::string getText() const { return text; }
    token_type getType() const { return type; }
    Token& operator=(const Token& op) {
        if(&op != this) {
            type = op.type;
            text = op.text;
            value = op.value;
            var = op.var;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& out, const Token& op);
};


//Identifier
class Ident
{
    token_type type;
    std::string name, str;
    bool declare, assign, is_struct, is_label, is_object;
    int value, struct_value;
public:
    std::vector<Ident> tid; //struct_tid
    Ident();
    Ident(const std::string text = "");
    Ident& operator= (Ident& op);
    bool operator==(const std::string& text) const { return name == text; }
    token_type getType() const { return type; }
    std::string getName() const { return name; }
    std::string getStr() const { return str; }
    bool getDeclare() const { return declare; }
    bool getAssign() const { return assign; }
    bool isStruct() const { return is_struct; }
    bool isObject() const { return is_object; }
    bool isLabel() const { return is_label; }
    int getValue() const { return value; }
    int getStructValue() const { return struct_value; }
    void putType(token_type tpe) { type = tpe; }
    void putName(std::string text) { name = text; }
    void putStr(std::string text) { str = text; }
    void putDeclare() { declare = true; }
    void putAssign() { assign = true; }
    void putIsStruct() { is_struct = true; }
    void putIsObject() { is_object = true; }
    void putIsLabel() { is_label = true; }
    void putValue(int n) { value = n; }
    void putValue(std::string text) { str = text; }
    void putStructValue(int n) { struct_value = n; }
    void printInfo();
};

Ident::Ident()
{
    str = "";
    value = 0;
    struct_value = -1;
    declare = assign = is_struct = is_label = is_object = false;
}

Ident::Ident(const std::string text)
{
    name = text;
    str = "";
    value = 0;
    struct_value = -1;
    declare = assign = is_struct = is_label = is_object = false;
}

Ident& Ident::operator= (Ident& op) 
{
    if(&op != this) {
        std::vector<Ident>::iterator itr = op.tid.begin();
        if(!tid.empty())
            tid.clear();
        while(itr != op.tid.end()) {
            tid.push_back(*itr);
            itr++;
        }
    }
    return *this;
}

void Ident::printInfo()
{
    std::cout << "Identifier '" << name << "':\n";
    std::cout << "was declared - " << declare << '\n';
    std::cout << "token type - " << type << '\n';
    std::cout << "assigned - " << assign << '\n';
    std::cout << "is a structure - " << is_struct << '\n';
    std::cout << "is an object - " << is_object << '\n';
    std::cout << "is a label - " << is_label << '\n';
    std::cout << "value = " << value << '\n';
    std::cout << "struct_value = " << struct_value << '\n';
    if(is_struct || is_object) {
        std::vector<Ident>::iterator itr = tid.begin();
        std::cout << "STRUCTURE||OBJECT:\n";
        while(itr != tid.end()) {
            std::cout << "\t name: " << (*itr).getName() << '\n';
            std::cout << "\t type: " << (*itr).getType() << '\n';
            std::cout << "\t was declared: " << (*itr).getDeclare() << '\n';
            std::cout << '\n';
            itr++;
        }
    }
}

int putIdentifier(const std::string& text, std::vector<Ident>& tid)
{
    auto itr = std::find_if(tid.begin(), tid.end(), [&text](const Ident& ident) {
        return ident.getName() == text;
    });

    if (itr != tid.end()) {
        return itr - tid.begin();
    }

    tid.push_back(Ident(text));
    return tid.size() - 1;
}


//TID
std::vector<Ident> TID;


class Scanner
{
    FILE* file;
    char current_symbol;
    int Look(const std::string buf, const char** list);
    void getChar() { current_symbol = fgetc(file); }
public:
    Scanner(const char* program);
    ~Scanner() { fclose(file); }
    void stepBack (int i) { fseek(file, -i, SEEK_CUR); }
    static const char*  TW[];
    static const char*  TD[];
    Token getToken(std::vector<Ident>& tid = TID);
};

int Scanner::Look(const std::string buf, const char** list) 
{
    int i = 0;
    while(list[i]) {
        if(buf == list[i])
            return i;
        i++;
    }
    return 0;
}

Scanner::Scanner(const char* program) 
{
    file = fopen(program, "r");
    if(file == NULL)
        throw "SOMETHING WRONG WITH THE FILE!!!"; 
}


const char* Scanner::TW[] =  //Service words table
{
  "",           // 0
  "not",        // 1
  "and",        // 2
  "or",         // 3
  "if",         // 4
  "else",       // 5
  "for",        // 6
  "do",         // 7
  "while",      // 8
  "break",      // 9
  "goto",       // 10
  "read",       // 11
  "write",      // 12
  "true",       // 13
  "false",      // 14
  "int",        // 15
  "string",     // 16
  "bool",       // 17
  "struct",     // 18
  "program",    // 19
  NULL
};

const char* Scanner::TD[] =  //Delimiter table
{
  "@",      // 0
  "*",      // 1
  "/",      // 2
  "+",      // 3
  "-",      // 4
  "=",      // 5
  "<",      // 6
  ">",      // 7
  "<=",     // 8
  ">=",     // 9
  "==",     // 10
  "!=",     // 11
  ";",      // 12
  ":",      // 13
  ",",      // 14
  ".",      // 15
  "(",      // 16
  ")",      // 17
  "{",      // 18
  "}",      // 19
  NULL
};


std::ostream& operator<<(std::ostream& out, const Token& op) 
{
    std::string str;
    if(op.getType() <= 19)
        str = Scanner::TW[op.getType()];
    else if((op.getType() >= 20) && (op.getType() <= 39))
        str = Scanner::TD[op.getType()-20];
    else if(op.getType() == 40)
        str = "NUM";
    else if(op.getType() == 42)
        str = TID[op.getValue()].getName();
    else if(op.getType() == 41)
        str = "STR";
    else if(op.getType() == 43)
        str = "POLIZ_LABEL";
    else if(op.getType() == 44)
        str = "POLIZ_ADDRESS";
    else if(op.getType() == 45)
        str = "!";
    else if(op.getType() == 46)
        str = "!F";
    else if(op.getType() == 47)
        str = "POLIZ_MINUS";
    else if(op.getType() == 48)
        str = "OBJECT";
    else
        throw op;
    if(op.getText() != "")
        out << '(' << str << ", " << op.getText() << ");\n";
    else if(op.getVariable() != -1)
        out << '(' << str << ", " << op.getValue() << ", " << op.getVariable() << ");\n";
    else
        out << '(' << str << ", " << op.getValue() << ");\n";
    return out;
}

Token Scanner::getToken(std::vector<Ident>& tid)
{
    enum state { H, IDENT, NUMB, ALE, NEQ, STCOM, INCOM, ENDCOM, STR };
    int number, j;
    std::string buf;
    state current_state = H;
    do {
        getChar();
        //std::cout << "current_symbol = '" << current_symbol << "'" << " state = " << current_state <<  '\n';
        if(current_symbol == EOF && current_state != H)
            throw "Unclosed COMMENT or STRING!!!";
        else if(current_symbol == EOF)
            return(Token(Fin));
        switch(current_state) {
            case H: 
                if(current_symbol == ' '  || current_symbol == '\n' || 
                    current_symbol == '\r' || current_symbol == '\t' );
                else if(isalpha(current_symbol)) {
                    buf.push_back(current_symbol);
                    current_state = IDENT;
                }
                else if(isdigit(current_symbol)) {
                    number = current_symbol - '0';
                    current_state = NUMB;
                }
                else if(current_symbol == '/') {
                    current_state = STCOM;
                }
                else if(current_symbol == '=' || current_symbol == '>' || current_symbol == '<') {
                    buf.push_back(current_symbol);
                    current_state = ALE;
                }
                else if(current_symbol == '!') {
                    buf.push_back(current_symbol);
                    current_state = NEQ;
                }
                else if(current_symbol == '\"') {
                    current_state = STR;
                }
                else {
                    //std::cout << "current_symbol = " << current_symbol << '\n';
                    buf.push_back(current_symbol);
                    if((j = Look(buf, TD)))
                        return Token((token_type)(j+(int)Fin), j);
                    else
                        throw current_symbol;
                }
                break;
            /* Identifier */
            case IDENT:
                if(isalpha(current_symbol) || isdigit(current_symbol))
                    buf.push_back(current_symbol);
                else {
                    ungetc(current_symbol, file);
                    if((j = Look(buf, TW)))
                        return Token((token_type)j, j);
                    else {
                        j = putIdentifier(buf, tid);
                        return Token(Id, j);
                    }
                }
                break;
            /* Number by Gorner scheme */
            case NUMB:
                if(isdigit(current_symbol))
                    number = number * 10 + (current_symbol - '0');
                else {
                    ungetc(current_symbol, file);
                    return Token(Num, number);
                }
                break;
            /* Start of the comment */
            case STCOM:
                if(current_symbol != '*') {
                    ungetc(current_symbol, file);
                    return Token(Slash, 2);
                }
                current_state = INCOM;
                break;
            /* In the comment */
            case INCOM:
                if(current_symbol == '*')
                    current_state = ENDCOM;
                if(current_symbol == EOF)
                    throw "UNCLOSED COMMENT!!!";
                break;
            /* End of comment */
            case ENDCOM:
                if(current_symbol == '/')
                    current_state = H;
                else if(current_symbol != '*')
                    current_state = INCOM;
                break;
            /* ALE - don't know what it stands for */
            case ALE:
                if(current_symbol == '=') {
                    buf.push_back(current_symbol);
                    j = Look(buf, TD);
                    return Token((token_type)(j+(int)Fin), j);
                }
                else {
                    ungetc(current_symbol, file);
                    j = Look(buf, TD);
                    return Token((token_type)(j+(int)Fin), j);
                }
                break;
            /* Not equal */
            case NEQ:
                if(current_symbol == '=') {
                    buf.push_back(current_symbol);
                    j = Look(buf, TD);
                    return Token(Neq, j);
                }
                else
                    throw '!';
                break;
            /* String literal */
            case STR:
                if(current_symbol != '\"')
                    buf.push_back(current_symbol);
                else
                    return Token(Str, 0, -1, buf);
                break;
        }                           //end switch
    } while(true);
}

struct Go_to {
    int address;
    int value;
};

class Parser
{
    Token current_token;
    token_type current_type;
    int current_value;
    Scanner scan;
    std::stack<int> value_stack;
    std::stack<token_type> type_stack;
    static std::vector<int> brk;
    static std::vector<Go_to> jmpto;

    //RD procedures
    void P();
    void D(token_type type, std::vector<Ident>& tid = TID, bool is_struct = false);//Variable descriptions
    void D1(std::vector<Ident>& tid = TID, bool is_struct = false);//Checking descriptions types
    void D2();//Working with structures
    void D3(int value = 0);//Struct-type variables description
    void B(bool is_cycle = false);//Statements and cycles
    void B1(bool is_cycle = false);//Checking cycles
    void G(Token token, bool is_cycle = false);
    void S(Token token);//Assigning fields
    void E(Token token, bool not_object = false, bool is_write = false);//Expression analysis
    void E1();//Pushing OR
    void E2();//Pushing AND
    void E3();//Pushing <, >, <=, >=, ==, !=
    void E4();//Pushing +,-
    void T();//Pushing *, /
    void F(bool is_minus = false, bool is_not = false);//type checking

    //Semantic analysis
    void Declare(token_type variable_type, std::vector<Ident>& tid = TID);
    void declareStruct(int value = 0);//Structure_type declarations into TID
    void checkId(int value);
    void checkLabel(int value);
    void checkObject();
    void eqTypeStruct();
    void checkOp();//Checking types of operators
    void checkNot();
    void checkMinus();
    void eqType();//Assignment context conditions
    void eqBool();//If, while, for context conditions
    void checkIdInRead();
    void getToken(std::vector<Ident>& tid = TID)
    {
        current_token = scan.getToken(tid);
        current_type = current_token.getType();
        current_value = current_token.getValue();
    }
public:
    std::vector<Token> poliz; //internal represantation of a program
    Parser(const char* program): scan(program) {} 
    void analysis(); //analyzer with actions
};

std::vector<int> Parser::brk;
std::vector<Go_to> Parser::jmpto;

//CORRECT!!!!!!

void Parser::analysis()
{
    getToken();
    P();
    if(current_type != Fin)
        throw current_token;
    std::vector<Token>::iterator itr = poliz.begin();
    int i = 0;
    while(itr != poliz.end()) {
        std::cout << i << ": " << *itr << '\n';
        i++;
        itr++;
    }
    std::cout << "Тачдааааааааааун!!!(Обработанно корректно)\n" << std::endl;
}

void Parser::P()
{
    if(current_type == Program)
        getToken();
    else
        throw current_token;
    if(current_type == LBrace)
        getToken();
    else
        throw current_token;
    D2();//working with structures
    D1();//checking descriptions types
    B1();//checking cycles
    if(current_type == RBrace)
        getToken();
    else
        throw current_token;

    //goto
    std::vector<Go_to>::iterator itr = jmpto.begin();
    int i;
    while(itr != jmpto.end())
    {
        i = (*itr).value;
        if(!TID[i].getDeclare())
            throw "GOTO: label was not declared";
        poliz[(*itr).address] = Token(Poliz_label, TID[i].getValue());
        itr++;
    }
}

//Working with structures
void Parser::D2()
{
    if(current_type != Struct)
        return;
    getToken();
    if(TID[current_value].getDeclare())
        throw "This STRUCTURE was already declared!!!\n";
    if(current_type != Id)
        throw current_token;
    TID[current_value].putDeclare();
    TID[current_value].putType(Struct);
    TID[current_value].putIsStruct();
    int idx = current_value;
    getToken();
    if(current_type != LBrace)
        throw current_token;
    getToken();
    D1(TID[idx].tid, true); //variable descriptions in structure
    if(current_type != RBrace)
        throw current_token;
    getToken();
    if(current_type != Semicolon)
        throw current_token;
    getToken();
    D2();//working with structures
}


//Checking descriptions types
void Parser::D1(std::vector<Ident>& tid, bool is_struct)
{
    if(current_type == Int || current_type == String || current_type == Bool)
    {
        token_type prev_type = current_type;
        getToken(tid);
        D(prev_type, tid, is_struct);//variable descriptions
        if(current_type == Semicolon)
            getToken();
        else
            throw current_token;
        D1(tid, is_struct);//checking descriptions types
    }
    //working with structure variable
    else if((current_type == Id) && (tid[current_value].isStruct() == 1))
    {
        int prev_value = current_value;
        getToken(tid);
        D3(prev_value);//struct-type variables description
        if(current_type == Semicolon)
            getToken();
        else
            throw current_token;
        D1(tid, is_struct);//checking descriptions types
    }
}

//Struct-type variables description
void Parser::D3(int value)
{
    if(current_type != Id)
        throw current_token;
    value_stack.push(current_value);
    getToken();
    while(current_type == Comma)
    {
        getToken();
        if(current_type != Id)
            throw current_token;
        else
        {
            value_stack.push(current_value);
            getToken();
        }
    }
    declareStruct(value);//moving this struct-type objs into TID
}


//Variable descriptions
void Parser::D(token_type variable_type, std::vector<Ident>& tid, bool is_struct)
{
    if(current_type != Id)
        throw current_token;
    value_stack.push(current_value);
    int prev_value = current_value;
    getToken(tid);
    if((current_type == Assign) && (is_struct == true))
        throw "Variable initialization within a structure is forbidden";
    if(current_type == Assign)
    {
        poliz.push_back(Token(Poliz_address, prev_value));
        getToken(tid);
        if(variable_type == Bool)
        {
            if(current_type == True)
                poliz.push_back(Token(True, 1));
            else if(current_type == False)
                poliz.push_back(Token(False, 0));
            else
                throw current_token;
        }
        else if(variable_type == String)
        {
            if(current_type == Str)
                poliz.push_back(current_token);
            else
                throw current_token;
        }
        else if(variable_type == Int)
        {
            if(current_type == Num)
                poliz.push_back(current_token);
            else 
                throw current_token;
        }
        poliz.push_back(Token(Assign));
        poliz.push_back(Token(Semicolon, 12));
        getToken(tid);
    }
    while(current_type == Comma)
    {
        getToken(tid);
        if(current_type != Id)
            throw current_token;
        value_stack.push(current_value);
        prev_value = current_value;
        getToken(tid);
        if((current_type == Assign) && (is_struct == true))
            throw "Variable initialization within a structure is forbidden";
        if(current_type == Assign) {
            poliz.push_back(Token(Poliz_address, prev_value));
            getToken(tid);
            if(variable_type == Bool)
            {
                if(current_type == True)
                    poliz.push_back(Token(True, 1));
                else if(current_type == False)
                    poliz.push_back(Token(False, 0));
                else
                    throw current_token;
            }
            else if(variable_type == String)
            {
                if(current_type == Str)
                    poliz.push_back(current_token);
                else
                    throw current_token;
            }
            else if(variable_type == Int)
            {
                if(current_type == Num)
                    poliz.push_back(current_token);
                else 
                    throw current_token;
            }
            poliz.push_back(Token(Assign));
            poliz.push_back(Token(Semicolon, 12));
            getToken(tid);
        }
    }
    Declare(variable_type, tid);
}

//Checking cycles
void Parser::B1(bool is_cycle)
{
    while(current_type != RBrace)
        B(is_cycle);
}

//Statements and cycles
void Parser::B(bool is_cycle)
{
    int pl0, pl1, pl2, pl3;
    int l1, l2, l3, l4;
    /*--------------------------------------------------------*/
    //If-statement
    if(current_type == If)
    {
        getToken();
        if(current_type != LPar)
            throw current_token;
        getToken();
        E(current_token);//expression analysis
        eqBool();//if, while, for context conditions
        pl2 = poliz.size();
        poliz.push_back(Token());
        poliz.push_back(Token(Poliz_fgo));
        if(current_type != RPar)
            throw current_token;
        getToken();
        B(is_cycle);//statements and cycles
        pl3 = poliz.size();
        poliz.push_back(Token());
        poliz.push_back(Token(Poliz_go));
        poliz[pl2] = Token(Poliz_label, poliz.size());
        if(current_type == Else) { 
            getToken();
            B(is_cycle);
            poliz[pl3] = Token(Poliz_label, poliz.size());
        }
        else
            throw current_token;
    }   //end of if-statement
    /*--------------------------------------------------------*/
    //While-loop
    else if(current_type == While) {
        pl0 = poliz.size();
        getToken();
        if(current_type != LPar)
            throw current_token;
        getToken();
        E(current_token);//expression analysis
        eqBool();//if, while, for context conditions
        pl1 = poliz.size();
        poliz.push_back(Token());
        poliz.push_back(Token(Poliz_fgo));
        if(current_type != RPar)
            throw current_token;
        getToken();
        B(true);//statements and cycles
        poliz.push_back(Token(Poliz_label, pl0));
        poliz.push_back(Token(Poliz_go));
        poliz[pl1] = Token(Poliz_label, poliz.size());
        //checking for break-statement
        std::vector<int>::iterator itr = brk.begin();
        while(itr != brk.end()) {
            poliz[*itr] = Token(Poliz_label, poliz.size());
            itr++;
        }
        brk.clear();
        //end of while-loop
    }
    /*--------------------------------------------------------*/
    //For-loop
    else if(current_type == For) {
        getToken();
        if(current_type != LPar)
            throw current_token;
        getToken();
        E(current_token);//expression analysis
        if(current_type != Semicolon)
            throw current_token;
        poliz.push_back(current_token);
        getToken();
        l1 = poliz.size();
        E(current_token);//expression analysis
        eqBool();//if, while, for context conditions
        if(current_type != Semicolon)
            throw current_token;
        getToken();
        l2 = poliz.size();
        poliz.push_back(Token());
        poliz.push_back(Token(Poliz_fgo));
        l3 = poliz.size();
        poliz.size();
        poliz.push_back(Token());
        poliz.push_back(Token(Poliz_go));
        l4 = poliz.size();
        E(current_token);//expression analysis
        if(current_type != RPar)
            throw current_token;
        poliz.push_back(Token(Semicolon, 12));
        getToken();
        poliz.push_back(Token(Poliz_label, l1));
        poliz.push_back(Token(Poliz_go));
        poliz[l3] = Token(Poliz_label, poliz.size());
        B(true);//statements and cycles
        poliz.push_back(Token(Poliz_label, l4));
        poliz.push_back(Token(Poliz_go));
        poliz[l2] = Token(Poliz_label, poliz.size());
        //checking for break-statement
        std::vector<int>::iterator itr = brk.begin();
        while(itr != brk.end()) { 
            poliz[*itr] = Token(Poliz_label, poliz.size());
            itr++;
        }
        brk.clear();
        //end of for-loop
    }
    /*--------------------------------------------------------*/
    //Break-statement
    else if(current_type == Break) {
        if(!is_cycle)
            throw current_token;
        brk.push_back(poliz.size());
        poliz.push_back(Token());
        poliz.push_back(Token(Poliz_go));
        getToken();
        if(current_type == Semicolon)
            getToken();
        else
            throw current_token;
        //end of break-statment
    }
    /*--------------------------------------------------------*/
    //Braces
    else if(current_type == LBrace) {
        getToken();
        B1(is_cycle);
        if(current_type == RBrace)
            getToken();
        else
            throw current_token;
        //end of Braces
    }
    /*--------------------------------------------------------*/
    //Id
    else if(current_type == Id) {
        G(current_token, is_cycle);
        //end of Id
    }
    /*--------------------------------------------------------*/
    //Empty statement
    else if(current_type == Semicolon) {
        getToken();
        //end of empty statement
    }
    /*--------------------------------------------------------*/
    //Const or LPar
    else if(current_type == Num || current_type == Str || current_type == True ||
        current_type == False || current_type == LPar || 
        current_type == Minus || current_type == Not)
    {
        E1();
        if(current_type == Semicolon) {
            poliz.push_back(current_token);
            getToken();
        }
        else
            throw current_token;
        //end of Const or LPar
    }
    /*--------------------------------------------------------*/
    //Read
    else if(current_type == Read)
    {
        getToken();
        if(current_type == LPar) {
            getToken();
            if(current_type == Id) {
                checkIdInRead();
                if(TID[current_value].isObject()) {
                    int i = current_value, j;
                    getToken();
                    if(current_type == RPar)
                        throw "OBJECT in READ!!!";
                    else if(current_type == Point) {
                        getToken(TID[i].tid);
                        if(current_type != Id)
                            throw current_token;
                        j = current_value;
                        if(!TID[i].tid[j].getDeclare())
                            throw "Wrong field of OBJECT!!!";
                        poliz.push_back(Token(Poliz_address, i, j));
                        getToken();
                    }
                    else
                        throw current_token;
                }
                else {
                    poliz.push_back(Token(Poliz_address, current_value));
                    getToken();
                }  
            }
            else
                throw current_token;
            if(current_type == RPar) {
                getToken();
                poliz.push_back(Token(Read));
            }
            else 
                throw current_token;
            if(current_type == Semicolon)
                getToken();
            else 
                throw current_token;
        }
        else
            throw current_token;
        //end of Read
    }
    /*--------------------------------------------------------*/
    //Write
    else if(current_type == Write)
    {
        getToken();
        if(current_type == LPar) {
            getToken();
            E(current_token, false, true);//expression analysis
            poliz.push_back(Token(Write));
            while(current_type == Comma) {
                getToken();
                E(current_token, false, true);//expression analysis
                poliz.push_back(Token(Write));
            }
            if(current_type != RPar)
                throw current_token;
            getToken();
            if(current_type != Semicolon)
                throw current_token;
            getToken();
        }
        else
            throw current_token;
        //end of Write
    }
    /*--------------------------------------------------------*/
    //GOTO
    else if(current_type == Goto)
    {
        getToken();
        if(current_type != Id)
            throw current_token;
        //if label was declared before goto
        if(TID[current_value].getDeclare() && TID[current_value].isLabel()) {
            poliz.push_back(Token(Poliz_label, TID[current_value].getValue()));
            poliz.push_back(Token(Poliz_go));
        }
        else if(TID[current_value].getDeclare() && !TID[current_value].isLabel()) {
            throw "Wrong operand in GOTO";
        }
        else {
            Go_to go_to;
            go_to.address = poliz.size();
            go_to.value = current_value;
            jmpto.push_back(go_to);
            poliz.push_back(Token());
            poliz.push_back(Token(Poliz_go));
        }
        getToken();
        if(current_type != Semicolon)
            throw current_token;
        getToken();
        //end of GOTO
    }
    /*--------------------------------------------------------*/
    else
        throw current_token;
    //end of B()
}

void Parser::G(Token token, bool is_cycle)
{
    int value;
    if(current_type == Id)
    {
        getToken();
        if(current_type == Colon)
        {
            value = token.getValue();
            checkLabel(value);
            TID[value].putIsLabel();
            TID[value].putDeclare();
            TID[value].putValue(poliz.size());
            getToken();
            G(current_token, is_cycle);
        }
        else {
            if(current_type <= 19)
                scan.stepBack(strlen(Scanner::TW[current_value]));
            else if((current_type >= 21) && (current_type <= 39))
                scan.stepBack(strlen(Scanner::TD[current_value]));
            else 
                throw current_token;
            current_token = token;
            current_type = current_token.getType();
            current_value = current_token.getValue();
            E(current_token);//expression analysis
            if(current_type == Semicolon) {
                poliz.push_back(Token(Semicolon, 12));
                getToken();
            }
            else
                throw current_token;
        }
    }
    else
        B(is_cycle);
}

//Expression analysis
void Parser::E(Token token, bool not_object, bool is_write)
{
    int i, j;
    if(current_type == Id)
    {
        getToken();
        if(current_type == Assign)
        {
            if(token.getType() == Object)
            {
                i = token.getValue();
                j = token.getVariable();
                type_stack.push(TID[i].tid[j].getType());
                poliz.push_back(Token(Poliz_address, i, j));
                getToken();
                E(current_token, not_object, is_write);
                eqType();//assignment context conditions
                poliz.push_back(Token(Assign));
            }
            else if(TID[token.getValue()].isObject())
            {
                if(not_object)
                    throw "Wrong operand types in ASSIGN";
                if(is_write) 
                    throw "OBJECT in WRITE";
                current_token = token;
                current_type = current_token.getType();
                current_value = current_token.getValue();
                scan.stepBack(1);
                S(current_token);//assigning fields
            }
            else
            {
                checkId(token.getValue());
                poliz.push_back(Token(Poliz_address, token.getValue()));
                getToken();
                E(current_token, true, is_write);//expression analysis
                eqType();//assignment context conditions
                poliz.push_back(Assign);
            }
        }
        else if(current_type == Plus  || current_type == Minus ||
                current_type == Times || current_type == Slash ||
                current_type == Lss   || current_type == Gtr) {
            if((token.getType() != Object) && TID[token.getValue()].isObject())
                throw "Wrong operand types in OPERATOR";
            scan.stepBack(1);
            current_token = token;
            current_type = current_token.getType();
            current_value = current_token.getValue();
            E1();
        }
        else if(current_type == Or  || current_type == Neq ||
                current_type == Leq || current_type == Geq ||
                current_type == Equal)
        {
            if((token.getType() != Object) && TID[token.getValue()].isObject())
                throw "Wrong operand types in BOOL OPERATOR";
            scan.stepBack(2);
            current_token = token;
            current_type = current_token.getType();
            current_value = current_token.getValue();
            E1();
        }
        else if(current_type == And)
        {
            if((token.getType() != Object) && TID[token.getValue()].isObject())
                throw "Wrong operand types in AND";
            scan.stepBack(3);
            current_token = token;
            current_type = current_token.getType();
            current_value = current_token.getValue();
            E1();
        }
        else if(current_type == RPar || (current_type == Semicolon && !is_write))
        {
            if(current_type == RPar && is_write && 
                token.getType() != Object && TID[token.getValue()].isObject())
                {
                throw "OBJECT in WRITE";
            }
            else if(token.getType() == Object)
            {
                i = token.getValue();
                j = token.getVariable();
                type_stack.push(TID[i].tid[j].getType());
                poliz.push_back(token);
            }
            else
            {
                checkId(token.getValue());
                poliz.push_back(token);
            }
        }
        else if(current_type == Comma && is_write)
        {
            if(token.getType() != Object && TID[token.getValue()].isObject())
                throw "OBJECT in WRITE";
            else if(token.getType() == Object)
            {
                i = token.getValue();
                j = token.getVariable();
                type_stack.push(TID[i].tid[j].getType());
                poliz.push_back(token);
            }
            else
            {
                checkId(token.getValue());
                poliz.push_back(token);
            }
        }
        else if (current_type == Point)
        {
            i = token.getValue();
            if(!TID[i].getDeclare() || !TID[i].isObject())
                throw "Object was not declared";
            getToken(TID[i].tid);
            if(current_type != Id)
                throw current_token;
            if(!TID[i].tid[current_value].getDeclare())
                throw "Wrong field of OBJECT!!!";
            current_token = Token(Object, i, current_value);
            E(current_token, true, is_write);
        }
        else
            throw current_token;
    }
    else
        E1();
}

//Assigning fields
void Parser::S(Token token)
{
    if(current_type == Id)
    {
        checkObject();
        getToken();
        if(current_type == Assign) {
            poliz.push_back(Token(Poliz_address, token.getValue()));
            getToken();
            S(current_token);
            eqTypeStruct();
            poliz.push_back(Token(Assign));
        }
        else {
            poliz.push_back(token);
            if(current_type != Semicolon && current_type != RPar)
                throw current_token;
        }
    }
    else
        throw current_token;
}

void Parser::checkObject()
{
    if(!TID[current_value].isObject())
        throw "Wrong operand types in ASSIGN";
    else
        value_stack.push(TID[current_value].getStructValue());
}

void Parser::eqTypeStruct()
{
    int top = value_stack.top();
    value_stack.pop();
    if(top != value_stack.top())
        throw "Wrong operand types in ASSIGN";
}

//Pushing OR
void Parser::E1()
{
    E2();
    while(current_type == Or)
    {
        type_stack.push(current_type);
        getToken();
        E2();
        checkOp();
    }
}

//Pushing AND
void Parser::E2()
{
    E3();
    while(current_type == And) {
        type_stack.push(current_type);
        getToken();
        E3();
        checkOp();
    }
}

//Pushing <, >, <=, >=, ==, !=
void Parser::E3()
{
    E4();
    if(current_type == Lss || current_type == Gtr || current_type == Leq ||
       current_type == Geq || current_type == Equal || current_type == Neq) { 
        type_stack.push(current_type);
        getToken();
        E4();
        checkOp();
    }
}

//Pushing +,-
void Parser::E4()
{
    T();
    while(current_type == Plus || current_type == Minus) {
        type_stack.push(current_type);
        getToken();
        T();
        checkOp();
    }
}

//Pushing *, /
void Parser::T()
{
    F();
    while(current_type == Times || current_type == Slash) {
        type_stack.push(current_type);
        getToken();
        F();
        checkOp();
    }
}

//type checking
void Parser::F(bool is_minus, bool is_not)
{ 
    int i,j;
    if(current_type == Object)
    {
        i = current_token.getValue();
        j = current_token.getVariable();
        type_stack.push(TID[i].tid[j].getType());
        poliz.push_back(Token(Object, i, j));
        getToken();
    }
    else if(current_type == Id)
    {
        if(TID[current_value].isObject())
        {
            i = current_value;
            getToken();
            if(current_type != Point)
                throw "Wrong types of operands";
            getToken(TID[i].tid);
            if(current_type != Id)
                throw current_token;
            j = current_value;
            if(!TID[i].tid[j].getDeclare())
                throw "Wrong field of OBJECT";
            type_stack.push(TID[i].tid[j].getType());
            poliz.push_back(Token(Object, i, j));
            getToken();
        }
        else
        {
            checkId(current_value);
            poliz.push_back(Token(Id, current_value));
            getToken();
        }
    }
    else if(current_type == Num)
    {
        type_stack.push(Int);
        poliz.push_back(current_token);
        getToken();
    }
    else if(current_type == True)
    {
        type_stack.push(Bool);
        poliz.push_back(Token(True, 1));
        getToken();
    }
    else if(current_type == False)
    {
        type_stack.push(Bool);
        poliz.push_back(Token(False, 0));
        getToken();
    }
    else if(current_type == Not)
    {
        if(is_minus)
            throw current_token;
        getToken();
        F(false, true);
        checkNot();
    }
    else if(current_type == LPar)
    {
        getToken();
        E(current_token);
        if(current_type == RPar)
            getToken();
        else
            throw current_token;
    }
    else if(current_type == Str)
    {
        type_stack.push(String);
        poliz.push_back(current_token);
        getToken();
    }
    else if(current_type == Minus)
    {
        if(is_not) 
            throw current_token;
        getToken();
        F(true, false);
        checkMinus();
    }
    else
        throw current_token;
}


void Parser::checkNot() 
{
    if(type_stack.top() != Bool)
        throw "Wrong operand type in NOT";
    else
        poliz.push_back(Token(Not));
}

void Parser::checkMinus() 
{
    if(type_stack.top() != Int)
        throw "Wrong operand type in MINUS";
    else
        poliz.push_back(Token(Poliz_minus));
}

//Assignment context conditions
void Parser::eqType()
{
    token_type tpe = type_stack.top();
    type_stack.pop();
    if(type_stack.empty())
        throw "Wrong operand types in ASSIGN";
    if(tpe != type_stack.top())
        throw "Wrong operand types in ASSIGN";
}

//If, while, for context conditions
void Parser::eqBool()
{
    if(type_stack.top() != Bool)
        throw "Not BOOLEAN expression";
    type_stack.pop();
}

void Parser::checkId(int value)
{
    if(TID[value].getDeclare())
    {
        if(TID[value].isLabel())
            throw "Wrong use of LABEL";
        type_stack.push(TID[value].getType());
    }
    else
        throw "Not declared";
}

void Parser::checkLabel(int value)
{
    if(TID[value].getDeclare())
        throw "LABEL matches variable or is redescribed";
}

//Checking types of operators
void Parser::checkOp() 
{
    token_type op1, op2, oprt;
    op2 = type_stack.top();
    type_stack.pop();
    oprt = type_stack.top();
    type_stack.pop();
    op1 = type_stack.top();
    type_stack.pop();
    if(op1 == op2 && op1 == String)
    {
        if(oprt == Plus)
            type_stack.push(String);
        else if(oprt == Gtr || oprt == Lss || oprt == Equal || oprt == Neq)
            type_stack.push(Bool);
        else
            throw "Unindentified STRING operator";
    }
    else if(op1 == op2 && op1 == Int)
    {
        if(oprt == Plus || oprt == Minus || oprt == Times || oprt == Slash)
            type_stack.push(Int);
        else
            type_stack.push(Bool);
    }
    else if(op1 == op2 && op1 == Bool)
    {
        if(oprt == Or || oprt == And)
            type_stack.push(Bool);
        else
            throw "Unindentified BOOL operator";
    }
    else
        throw "Wrong operand types in OPERATOR";
    poliz.push_back(Token(oprt));
}

void Parser::Declare(token_type type, std::vector<Ident>& tid) 
{
    int i;
    while(!value_stack.empty())
    {
        i = value_stack.top();
        value_stack.pop();
        if(tid[i].getDeclare())
            throw "DECLARED twice!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        else
        {
            tid[i].putDeclare();
            tid[i].putType(type);
        }
    }
}

//Structure_type declarations into TID
void Parser::declareStruct(int value)
{
    int i;
    while(!value_stack.empty())
    {
        i = value_stack.top();
        value_stack.pop();
        if(TID[i].getDeclare())
            throw "DECLARED twice";
        else
        {
            TID[i].putDeclare();
            TID[i].putType(Object);
            TID[i].putIsObject();
            TID[i].putStructValue(value);//saving which struct-type is current obj
            TID[i] = TID[value];//moving fields
        }
    }
}

void Parser::checkIdInRead() 
{
    if(!TID[current_value].getDeclare())
        throw "Not declared";
}

struct Element
{
    int value, variable;
    std::string str;
    bool is_string, is_object;
    Element(int val, int var = -1) : value(val), variable(var), is_string(false)
    {
        is_object = (variable != -1);
    }
    Element(std::string text): str(text), is_string(true), is_object(false){}
    int getValue() const { return value; }
    int getVariable() const { return variable; }
    void putIsObject() { is_object = true; }
    bool isString() const { return is_string; }
    operator int() { return value; }
    operator std::string() { return str; }

};

class Executer
{
    Token token;
public:
    void execute(std::vector<Token>& poliz);
};

void Executer::execute(std::vector<Token>& poliz)
{
    std::stack<Element> args;
    std::string str1, str2;
    Ident* id;
    int i, j, num, idx = 0, size = poliz.size();
    while (idx < size)
    {
        token = poliz[idx];
        switch(token.getType())
        {
            case True:
            case False:  
            case Num:
            case Poliz_label:
                args.push(token.getValue());
                break;

            case Poliz_address:
                if(token.getVariable() != -1)
                {
                    i = token.getValue();
                    j = token.getVariable();
                    args.push(Element(i,j));
                }
                else
                    args.push(token.getValue());
                break;

            case Str:
                args.push(token.getText());
                break;

            case Id:
                i = token.getValue();
                if(TID[i].isObject())
                {
                    args.push(i);
                    args.top().putIsObject();
                    break;
                }
                else if(TID[i].getAssign())
                {
                    if(TID[i].getType() == String)
                        args.push(TID[i].getStr());
                    else
                        args.push(TID[i].getValue());
                }
                else
                    throw "POLIZ: indefinite identifier";
                break;
      
            case Object:
                i = token.getValue();
                j = token.getVariable();
                id = &(TID[i].tid[j]);
                if((*id).getAssign()) {
                    if((*id).getType() == String)
                        args.push((*id).getStr());
                    else
                        args.push((*id).getValue());
                }
                else
                    throw "POLIZ: indefinite identifier";
                break;

            case Semicolon:
                args.pop();
                break;

            case Not:
                i = (int)args.top();
                args.pop();
                args.push(!i);
                break;

            case Poliz_minus:
                i = (int)args.top();
                args.pop();
                args.push(-i);
                break;

            case Or:
                i = (int)args.top();
                args.pop();
                j = (int)args.top();
                args.pop();
                args.push(j || i);
                break;

            case And:
                i = (int)args.top();
                args.pop();
                j = (int)args.top();
                args.pop();
                args.push(j && i);
                break;

            case Poliz_go:
                i = (int)args.top();
                args.pop();
                idx = i - 1;
                break;

            case Poliz_fgo:
                i = (int)args.top();
                args.pop();
                j = (int)args.top();
                args.pop();
                if(!j)
                    idx = i - 1;
                break;

            case Write:
                if(args.top().isString())
                {
                    str2 = (std::string)args.top();
                    args.pop();
                    std::cout << str2 << std::endl;
                }
                else
                {
                    j = (int)args.top();
                    args.pop();
                    std::cout << j << std::endl;
                }
                break;

            case Read:
                int k;
                if(args.top().is_object)
                {
                    i = args.top().value;
                    j = args.top().variable;
                    args.pop();
                    id = &(TID[i].tid[j]);
                    if((*id).getType() == Int)
                    {
                        std::cout << "Input int value for " << TID[i].getName() 
                         << "." << (*id).getName() << std::endl;
                        std::cin >> k;
                        (*id).putValue(k);
                        (*id).putAssign();
                    }
                    else if((*id).getType() == String)
                    {
                        std::cout << "Input string value for " << TID[i].getName() 
                             << "." << (*id).getName() << std::endl;
                        std::cin >> str1;
                        (*id).putValue(str1);
                        (*id).putAssign();
                    }
                    else
                    {
                        std::string j;
                        while(1) /////////////////////111
                        {
                            std::cout << "Input boolean value (true or false) for " 
                               << TID[i].getName() << "." << (*id).getName() << std::endl;
                            std::cin >> j;
                            if ( j != "true" && j != "false" ) {
                                std::cout << "Error in input: true/false" << std::endl;
                                continue;
                            }
                            k = ((j == "true") ? 1 : 0);
                            break;
                        }
                        (*id).putValue(k);
                        (*id).putAssign();
                    }
                }
                else
                {
                    i = (int)args.top();
                    args.pop();
                    if(TID[i].getType() == Int)
                    {
                        std::cout << "Input int value for " << TID[i].getName() << std::endl;
                        std::cin >> k;
                        TID[i].putValue(k);
                        TID[i].putAssign();
                    }
                    else if(TID[i].getType() == String)
                    {
                        std::cout << "Input string value for " << TID[i].getName() << std::endl;
                        std::cin >> str1;
                        TID[i].putValue(str1);
                        TID[i].putAssign();
                    }
                    else
                    {
                        std::string j;
                        while(1)
                        {
                            std::cout << "Input boolean value (true or false) for " << TID[i].getName() << std::endl;
                            std::cin >> j;
                            if( j != "true" && j != "false" )
                            {
                                std::cout << "Error in input: true/false" << std::endl;
                                continue;
                            }
                            k = ((j == "true") ? 1 : 0);
                            break;
                        }
                        TID[i].putValue(k);
                        TID[i].putAssign();
                    }
                }
                break;

            case Plus:
                if(args.top().isString())
                {
                    str2 = (std::string)args.top();
                    args.pop();
                    str1 = (std::string)args.top();
                    args.pop();
                    args.push(str1 + str2); 
                }
                else
                {
                    i = (int)args.top();
                    args.pop();
                    j = (int)args.top();
                    args.pop();
                    args.push(i + j);
                }
                break;

            case Times:
                i = (int)args.top();
                args.pop();
                j = (int)args.top();
                args.pop();
                args.push(i * j);
                break;

            case Minus:
                i = (int)args.top();
                args.pop();
                j = (int)args.top();
                args.pop();
                args.push(j - i);
                break;

            case Slash:
                i = (int)args.top();
                args.pop();
                j = (int)args.top();
                args.pop();
                if(i) {
                    args.push(j / i);
                    break;
                }
                else
                    throw "POLIZ: divide by zero";

            case Equal:
                if(args.top().isString()) {
                    str2 = (std::string)args.top();
                    args.pop();
                    str1 = (std::string)args.top();
                    args.pop();
                    args.push(str1 == str2); 
                }
                else
                {
                    i = (int)args.top();
                    args.pop();
                    j = (int)args.top();
                    args.pop();
                    args.push(j == i);
                }
                break;

            case Lss:
                if(args.top().isString())
                {
                    str2 = (std::string)args.top();
                    args.pop();
                    str1 = (std::string)args.top();
                    args.pop();
                    args.push(str1 < str2); 
                }
                else
                {
                    i = (int)args.top();
                    args.pop();
                    j = (int)args.top();
                    args.pop();
                    args.push(j < i);
                }
                break;

            case Gtr:
                if(args.top().isString())
                {
                    str2 = (std::string)args.top();
                    args.pop();
                    str1 = (std::string)args.top();
                    args.pop();
                    args.push(str1 > str2); 
                }
                else
                {
                    i = (int)args.top();
                    args.pop();
                    j = (int)args.top();
                    args.pop();
                    args.push(j > i);
                }
                break;

            case Leq:
                i = (int)args.top();
                args.pop();
                j = (int)args.top();
                args.pop();
                args.push(j <= i);
                break;

            case Geq:
                i = (int)args.top();
                args.pop();
                j = (int)args.top();
                args.pop();
                args.push(j >= i);
                break;

            case Neq:
                if(args.top().isString())
                {
                    str2 = (std::string)args.top();
                    args.pop();
                    str1 = (std::string)args.top();
                    args.pop();
                    args.push(str1 != str2); 
                }
                else
                {
                    i = (int)args.top();
                    args.pop();
                    j = (int)args.top();
                    args.pop();
                    args.push(j != i);
                }
                break;

            case Assign:
                if(args.top().is_object)
                {
                    i = (int)args.top();
                    args.pop();
                    j = (int)args.top();
                    args.pop();
                    TID[j] = TID[i];
                    args.push(i);
                    args.top().putIsObject();
                }
                else if(args.top().isString())
                {     
                    str2 = (std::string)args.top();
                    args.pop();
                    if(args.top().is_object) {
                        i = args.top().value;
                        j = args.top().variable;
                        id = &(TID[i].tid[j]);
                        args.pop();
                        (*id).putValue(str2);
                        (*id).putAssign();
                    }
                    else
                    {
                        j = (int)args.top();
                        args.pop();
                        TID[j].putValue(str2);
                        TID[j].putAssign();
                    }
                    args.push(str2);
                }
                else
                {
                    num = (int)args.top();
                    args.pop();
                    if(args.top().is_object)
                    {
                        i = args.top().value;
                        j = args.top().variable;
                        id = &(TID[i].tid[j]);
                        args.pop();
                        (*id).putValue(num);
                        (*id).putAssign();
                    }
                    else
                    {
                        j = (int)args.top();
                        args.pop();
                        TID[j].putValue(num);
                        TID[j].putAssign();
                    }
                    args.push (num);
                }
                break;

            default:
                throw "POLIZ: unexpected element";
        } //end of switch
        idx++;
    } //end of while
    std::cout << "\nГООООЛ(Выполнено)!!!" << std::endl;
}

class Interpreter
{
    Parser parsing;
    Executer exe;
public:
    Interpreter(const char* program): parsing(program) {}
    void Interpretation();
};

void Interpreter::Interpretation()
{
    parsing.analysis();
    exe.execute(parsing.poliz);
}

int main(int argc, char** argv) 
{
    try
    {
        Interpreter intpr(argv[1]);
        intpr.Interpretation();
        return 0;
    }
    catch(char c)
    {
        std::cout << "Unexpected symbol" << c << std::endl;
        return 1;
    }
    catch(Token token)
    {
        std::cout << "Unexpected token" << token << std::endl;
        return 1;
    }
    catch(const char* c)
    {
        std::cout << c << std::endl;
        return 1;
    }
}
