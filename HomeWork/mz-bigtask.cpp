//task2
/////////////////////////////////////////////////////////////////
/*
2. Дана грамматика 
S -> aBc                      
B -> Bb | cA
A -> aA | c

a)Применим ли метод рекурсивного спуска к данной грамматике? Ответ обосновать. 
Если РС не применим,преобразовать грамматику в эквивалентную так, чтобы РС был применим. 
b) Написать анализатор методом РС, используя предложенную заготовку класса Parser.
c) Определить язык, который порождает грамматика. 
d) В результате работы анализатора для корректных входных цепочек на экран должна выводиться последовательность нетерминальных символов грамматики в порядке их замены при построении правостороннего вывода.
В случае ошибки должно быть выведено сообщение: « Ошибочный символ - < здесь печатать символ, ставший причиной ошибки > ».
*/

#include<iostream>
#include<list>

using namespace std;

class Parser 
{
public:
static list<char> result;   //результат
static char c;              //буфер символа
static void gc(){cin>>c;} 

class S // моделируем правила S и т.п
{                   
    public:
    S()
    {
        if(c == 'a') 
        {
            gc();
            B();
            if(c == 'c')
            {
                gc();
            } 
            else 
                throw c;
        } 
        else 
            throw c; 
    }
    ~S()
    { 
        result.push_front('S');
    }
};

class A
{ 
public:
    A()
    { 
        if(c == 'a')
        {
            gc();
            A();
        }
        else if(c == 'c')
        {
            gc();
        } 
        else
            throw c;
    } 
    ~A()
    { 
        result.push_front('A');
    }
};
class Z
{
public:
    Z()
    {
        if (c == 'b')
        {
            gc();
            Z();//нет throw т.к. пустота в правиле допустима
        }                 
    }
    ~Z()
    { 
        result.push_front('Z');
    }
};
class B
{
public:
    B()
    {
        if (c == 'c')
        {
            gc();
            A();
            Z();
        }
        else
            throw c;
    }
    ~B()
    { 
        result.push_front('B');
    }
};
//////////////////////////////////////////
//////////////////////////////////////////
public:
    void analyze()
    {
        try
        {
            result.clear();
            gc();
            S();
            if (c != '#')
                throw c;
            cout << "Результат перевода для входной цепочки: ";
            for(auto p = result.begin(); p != result.end(); p++)
            {
                cout << *p;
            }
            cout << endl;
        }  
        catch(char c)
        {
            cout << "ошибочный символ: " << c << endl;
        }
    }
};
////////////////////////
////////////////////////
char Parser::c;
list<char> Parser::result; // объявили статические переменные

int main()
{ 
    Parser par;
    par.analyze();
    return 0;
}