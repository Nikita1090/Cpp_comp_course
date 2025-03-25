#include <iostream>
#include <ctype.h>
#include <vector>
#include <stack>
#include <string>
#include <cstring>
#include <algorithm>
enum lexema_type
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
    Id,                                                     //42                                                //48
};


//lexema that you can get type, text and value of
class lexema // языковая единица
{
    lexema_type type;
    std::string text;
    int value;
public:
    lexema(lexema_type tpe = Null, int val = 0, int v = -1, std::string str = "")
        : type(tpe), text(str), value(val) {}
    lexema(const lexema& op) // конструкторы и бла бла бла
    {
        type = op.type;
        text = op.text;
        value = op.value;
    }
    int getValue() const { return value; }
    std::string getText() const { return text; }
    lexema_type getType() const { return type; }
    lexema& operator=(const lexema& op) // оператор сравнения с учётом значений
    {
        if(&op != this)
        {
            type = op.type;
            text = op.text;
            value = op.value;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& out, const lexema& op);
};


//Identifier
class Ident
{
    lexema_type type;
    std::string name, str;
    bool declare, assign, is_struct, is_label, is_object;
    int value, struct_value;
public:
    std::vector<Ident> tid; //struct_tid
    Ident();
    Ident(const std::string text = "");
    Ident& operator= (Ident& op);
    bool operator==(const std::string& text) const { return name == text; }
    lexema_type getType() const { return type; }
    std::string getName() const { return name; }
    std::string getStr() const { return str; }
    bool getDeclare() const { return declare; }
    bool getAssign() const { return assign; }
    bool isStruct() const { return is_struct; }
    bool isObject() const { return is_object; }
    bool isLabel() const { return is_label; }
    int getValue() const { return value; }
    int getStructValue() const { return struct_value; }
    void putType(lexema_type tpe) { type = tpe; }
    void putName(std::string text) { name = text; }
    void putStr(std::string text) { str = text; }
    void putDeclare() { declare = true; }
    void putAssign() { assign = true; }
    void putIsStruct() { is_struct = true; }
    void putIsObject() { is_object = true; }
    void putIsLabel() { is_label = true; }
    void putValue(int n) { value = n; }
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

Ident& Ident::operator=(Ident& op) 
{
    if(&op != this)
    {
        std::vector<Ident>::iterator itr = op.tid.begin();
        if(!tid.empty())
            tid.clear();
        while(itr != op.tid.end())
        {
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
    std::cout << "lexema type - " << type << '\n';
    std::cout << "assigned - " << assign << '\n';
    std::cout << "is a structure - " << is_struct << '\n';
    std::cout << "is an object - " << is_object << '\n';
    std::cout << "is a label - " << is_label << '\n';
    std::cout << "value = " << value << '\n';
    std::cout << "struct_value = " << struct_value << '\n';
    if(is_struct || is_object)
    {
        std::vector<Ident>::iterator itr = tid.begin();
        std::cout << "STRUCTURE||OBJECT:\n";
        while(itr != tid.end())
        {
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
    FILE* file; // текущий файл из которого читаем код на модельном яп
    char current_symbol;
    int Look(const std::string buf, const char** list); // поиск строки в buf возвращает номер в списке строк
    void gc() { current_symbol = fgetc(file); } // типичный gc() как везде в заданиях 
public:
    Scanner(const char* program); // при создания объекта следует открыть файл
    ~Scanner() { fclose(file); }
    static const char*  TW[];
    static const char*  TD[];
    lexema getlexema(std::vector<Ident>& tid = TID);
};

int Scanner::Look(const std::string buf, const char** list) 
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


std::ostream& operator<<(std::ostream& out, const lexema& op) 
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
        str = "LABEL";
    else if(op.getType() == 44)
        str = "ADDRESS";
    else if(op.getType() == 45)
        str = "!";
    else if(op.getType() == 46)
        str = "!F";
    else if(op.getType() == 47)
        str = "MINUS";
    else if(op.getType() == 48)
        str = "OBJECT";
    else
        throw op;
    if(op.getText() != "")
        out << '(' << str << ", " << op.getText() << ");\n";
    else
        out << '(' << str << ", " << op.getValue() << ");\n";
    return out;
}

lexema Scanner::getlexema(std::vector<Ident>& tid)
{
    enum state { H, IDENT, NUMB, ALE, NEQ, STCOM, INCOM, ENDCOM, STR };
    int number, j;
    std::string buf;
    state current_state = H;
    do
    {
        gc();
        if(current_symbol == EOF && current_state != H)
            throw "Unclosed COMMENT or STRING!!!";
        else if(current_symbol == EOF)
            return(lexema(Fin));
        switch(current_state)
        {
            case H: 
                if(current_symbol == ' '  || current_symbol == '\n' || 
                    current_symbol == '\r' || current_symbol == '\t' );
                else if(isalpha(current_symbol))
                {
                    buf.push_back(current_symbol);
                    current_state = IDENT;
                }
                else if(isdigit(current_symbol))
                {
                    number = current_symbol - '0';
                    current_state = NUMB;
                }
                else if(current_symbol == '/')
                {
                    current_state = STCOM;
                }
                else if(current_symbol == '=' || current_symbol == '>' || current_symbol == '<')
                {
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
                else
                {
                    //std::cout << "current_symbol = " << current_symbol << '\n';
                    buf.push_back(current_symbol);
                    if((j = Look(buf, TD)))
                        return lexema((lexema_type)(j+(int)Fin), j);
                    else
                        throw current_symbol;
                }
                break;
            /* Identifier */
            case IDENT:
                if(isalpha(current_symbol) || isdigit(current_symbol))
                    buf.push_back(current_symbol);
                else
                {
                    ungetc(current_symbol, file);
                    if((j = Look(buf, TW)))
                        return lexema((lexema_type)j, j);
                    else
                    {
                        j = putIdentifier(buf, tid);
                        return lexema(Id, j);
                    }
                }
                break;
            /* Number by Gorner scheme */
            case NUMB:
                if(isdigit(current_symbol))
                    number = number * 10 + (current_symbol - '0');
                else
                {
                    ungetc(current_symbol, file);
                    return lexema(Num, number);
                }
                break;
            /* Start of the comment */
            case STCOM:
                if(current_symbol != '*')
                {
                    ungetc(current_symbol, file);
                    return lexema(Slash, 2);
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
                if(current_symbol == '=')
                {
                    buf.push_back(current_symbol);
                    j = Look(buf, TD);
                    return lexema((lexema_type)(j+(int)Fin), j);
                }
                else
                {
                    ungetc(current_symbol, file);
                    j = Look(buf, TD);
                    return lexema((lexema_type)(j+(int)Fin), j);
                }
                break;
            /* Not equal */
            case NEQ:
                if(current_symbol == '=')
                {
                    buf.push_back(current_symbol);
                    j = Look(buf, TD);
                    return lexema(Neq, j);
                }
                else
                    throw '!';
                break;
            /* String literal */
            case STR:
                if(current_symbol != '\"')
                    buf.push_back(current_symbol);
                else
                    return lexema(Str, 0, -1, buf);
                break;
        }                           //end switch
    }while(true);
}

int main(int argc, char** argv) 
{
    Scanner scan(argv[1]);
    lexema current_lexema;
    lexema_type current_type;
    try
    {
        do
        {
            current_lexema = scan.getlexema();
            current_type = current_lexema.getType();
            std::cout << current_lexema;
        }
        while(current_type != Fin);
        return 0;
    }
    catch(char c)
    {
        std::cout << "unexpected symbol '" << c << "'" << std::endl;
        return 1;
    }
    catch(lexema tk)
    {
        std::cout << "unexpected lexema" << tk << std::endl;
        return 1;
    }
    catch(const char* str)
    {
        std::cout << str << std::endl;
        return 1;
    }
}