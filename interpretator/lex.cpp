#include <iostream>
#include <ctype.h>
#include <vector>

enum token_type 
{
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
};


//Token that you can get type, text and value of
class Token // языковая единица
{
    token_type type;
    std::string text;
    int value;
    bool is_str;
public:
    Token(token_type tpe = Null, int val = 0, bool flag = false, std::string str = "")
        : type(tpe), text(str), value(val), is_str(flag) {}
    Token(const Token& op) // конструкторы и бла бла бла
    {
        type = op.type;
        text = op.text;
        value = op.value;
        is_str = op.is_str;
    }
    int getValue() const { return value; }
    bool isString() const { return is_str; }
    std::string getText() const { return text; }
    token_type getType() const { return type; }
    Token& operator=(const Token& op) // оператор сравнения с учётом значений
    {
        if(&op != this)
        {
            type = op.type;
            text = op.text;
            value = op.value;
            is_str = op.is_str;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& out, const Token& op); // для вывода
};


//Identifier
class Ident
{
    token_type type;
    std::string name, str;
    bool declare, assign, boolean, is_struct, is_label;
    int value;
public:
    std::vector<Ident> struct_tid;
    Ident(const std::string text = "");
    Ident& operator= (const Ident& op);
    bool operator==(const std::string& text) const { return name == text; }
    token_type getType() const { return type; }
    std::string getName() const { return name; }
    std::string getStr() const { return str; }
    bool getDeclare() const { return declare; }
    bool getAssign() const { return assign; }
    bool getBoolean() const { return boolean; }
    bool isStruct() const { return is_struct; }
    bool isLabel() const { return is_label; }
    int getValue() { return value; }
    void putType(token_type tpe) { type = tpe; }
    void putName(std::string text) { name = text; }
    void putStr(std::string text) { str = text; }
    void putDeclare() { declare = true; }
    void putAssign() { assign = true; }
    void putBoolean() { boolean = true; }
    void putIsStruct() { is_struct = true; }
    void putIsLabel() { is_label = true; }
    void putValue(int n) { value = n; }
    void printInfo();
};

Ident::Ident(const std::string text)
{
    name = text;
    str = "";
    value = 0;
    declare = assign = is_struct = is_label = false;
}

void Ident::printInfo()
{
    std::cout << "Identifier '" << name << "':\n";
    std::cout << "was declared - " << declare << '\n';
    std::cout << "token type - " << type << '\n';
    std::cout << "assigned - " << assign << '\n';
    std::cout << "is a structure - " << is_struct << '\n';
    std::cout << "is a label - " << is_label << '\n';
    std::cout << "value = " << value << '\n';
}

/*
class IdentifierTable {
    Ident *pointer;
    int size, top;
public:
    IdentifierTable(int max_size = 512) : size(max_size), top(1) {
        pointer = new Ident[size];
    }
    ~IdentifierTable() { delete[] pointer;}
    Ident& operator[](int idx) { return pointer[idx]; }
    //int putIdentifier(const std::string text);
};


int IdentifierTable::putIdentifier(const std::string text)
{
    for(int i = 1; i < top; i++)
        if(pointer[i].getName() == text)
            return i;
    pointer[top].putName(text);
    top++;
    return top-1;
}
*/

int putIdentifier(const std::string text, std::vector<Ident>& tid)
{
    std::vector<Ident>::iterator itr;
    itr = find(tid.begin(), tid.end(), text);
    if(itr != tid.end())
        return itr - tid.begin();
    tid.push_back(Ident(text));
    return tid.size() - 1;
}

//TID
//IdentifierTable TID;
std::vector<Ident> TID;


class Scanner
{
    FILE* file; // текущий файл из которого читаем код на модельном яп
    char current_symbol;
    int Look(const std::string buf, const char** list); // поиск строки в buf возвращает номер в списке строк
    void gc() { current_symbol = fgetc(file); } // типичный gc() как везде в заданиях 
public:
    Scanner(const char* program) // при создания объекта следует открыть файл
    {
        file = fopen(program, "r");
        if(file == NULL)
            throw "BAD FILE :("; 
    }
    ~Scanner() 
    {
        fclose(file);
    }
    void stepBack (int i) 
    {
        fseek (file, -i, SEEK_CUR); // отступ на i позиций назад от текущего
    }
    static const char*  TW[]; // таблица служебных слов
    static const char*  TD[]; // таблица разделителей
    Token getToken(std::vector<Ident>& tid = TID);
};

int Scanner::Look(const std::string buf, const char** list) // поиск строки в buf возвращает номер в списке строк
{
    int i = 0;
    while(list[i]) 
    {
        if(buf == list[i])
            return i;
        i++;
    }
    return 0;
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
    else
        throw "NO TOKEN!!!";
    if(op.getText() != "")
    {
        //out << "TEXT: " <<  op.getText() << "\nEND!!!\n";
        out << '(' << str << ", " << op.getText() << ");\n";
    }
    else
        out << '(' << str << ", " << op.getValue() << ");\n";
    return out;
}

Token Scanner::getToken(std::vector<Ident>& tid)
{
    enum state { H, IDENT, NUMB, ALE, DELIM, NEQ, STCOM, INCOM, ENDCOM, STR }; // моделируем автомат как в грамматиках
    int number, j;
    std::string buf;
    state current_state = H; // начало
    do
    {
        gc();
        //std::cout << "current_symbol = '" << current_symbol << "'" << " state = " << current_state <<  '\n';
        if(current_symbol == EOF)
            return(Token(Fin));
        switch(current_state)
        {
            case H: 
                if ( current_symbol == ' '  || current_symbol == '\n' || 
                    current_symbol == '\r' || current_symbol == '\t' );
                else if(isalpha(current_symbol)) // если буква
                {
                    buf.push_back(current_symbol);
                    current_state = IDENT;
                }
                else if(isdigit(current_symbol)) // если цифра
                {
                    number = current_symbol - '0';
                    current_state = NUMB;
                }
                else if(current_symbol == '/') // если /
                {
                    current_state = STCOM;
                }
                else if(current_symbol == '=' || current_symbol == '>' || current_symbol == '<')
                { // если операции сравнения
                    buf.push_back(current_symbol);
                    current_state = ALE;
                }
                else if(current_symbol == '!')
                {
                    buf.push_back(current_symbol);
                    current_state = NEQ;
                }
                else if(current_symbol == '\"')
                {
                    current_state = STR;
                }
                else if(current_symbol == EOF)
                {
                    return Token(Fin);
                }
                else
                {
                    //std::cout << "current_symbol = " << current_symbol << '\n';
                    buf.push_back(current_symbol);
                    current_state = DELIM;
                }
                break;
            /* Identifier */
            case IDENT:
                if(isalpha(current_symbol) || isdigit(current_symbol))
                {
                    buf.push_back(current_symbol);
                }
                else
                {
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
            /* Deliminator */
            case DELIM:
                //buf.push_back(current_symbol);
                ungetc(current_symbol, file);
                if((j = Look(buf, TD))) 
                    return Token((token_type)(j+(int)Fin), j);
                else
                    throw current_symbol;
                break;
            case STR:
                if(current_symbol != '\"')
                    buf.push_back(current_symbol);
                else
                    return Token(String, 16, true, buf);
                break;
        }                           //end switch
    } while(true);
}



int main(int argc, char** argv) 
{
    Scanner scan(argv[1]); // открываем файл
    Token current_token;
    token_type current_type;
    int current_value;
    try 
    {
        do 
        {
            current_token = scan.getToken();
            current_type = current_token.getType();
            current_value = current_token.getValue();
            std::cout << current_token;
        } while(current_type != Fin);
        return 0;
    }
    catch(char c) 
    {
        std::cout << "unexpected symbol '" << c << "'" << std::endl;
        return 1;
    }
    catch(Token tk) 
    {
        std::cout << "unexpected token" << tk << std::endl;
        return 1;
    }
    catch(const char* str) 
    {
        std::cout << str << std::endl;
        return 1;
    }
}