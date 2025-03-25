#include <iostream>
#include <string.h>

using namespace std;

class Str
{
private:
    char *S;
    int len;
public:
    Str() // конструктор по умолчанию, нужен для стабильной работы класса
    {
        this->S = new char[1]; // например если не создавать такую заглушку, то
        this->len = 0;            // можно схватить ошибку double free(), так как в нижеописанных методах класса нет проверки на пустой класс
    };                                  // вообще во всех случаях где в классах используются указатели, лучше выделять под них память даже если это кажется лишним
    Str(const char *S) // чтобы компилятор не слал вам бесконечные warning'ги, лучше ставьте где не изменяете сам объект слово const
    {// си'шные библиотеки пока у нас в ходу
        this->len = strlen(S); // this это указатель на самого себя внутри класса, он по умолчанию есть в private, неявно
        // если кто-то помнит питон, то по сути это "self"
        this->S = new char[len + 1]; // +1 так как strlen не учитывает "\0" - признак конца стр.
        strcpy(this->S, S);
        cout << "|Constructor: " << this->S <<"|"; //просто отладочная печать чтобы видеть как работают механизмы класса
    };     // fan fact: чтобы запомнить в какую сторону ставить ">>","<<" представляйте это как крик: 
            // т.е. cout кричит на нас текстом, а в cin наоборот кричим мы))) cin >> | cout <<
    Str(const Str & c)// ого опять то же имя у функции чзх?
    {                // это конструктор копирования, он отличается аргументом на вход. 
                    // Он просит на вход объект того же класса что и сам класс
        len = c.len; // обращаемся к полям копируемого класса
        S = new char[len + 1]; // обязательно создаём новый объект иначе получится просто передача по ссылке (и будет кошмар)
        strcpy(S, c.S); 
        cout << "|Copy |";
    };
    Str & operator=(const Str & c) // че та странное и непонятное, да?
    {                             // это та волшебная штука переопределяющая оператор =
                                 // тоесть определяющая что происходит при "строка1 = строка2"
        if(&c == this) // проверяем что мы не присваеваем объект сам себе
        {
            return *this; // возврат самого объекта, отмечу что на вход этой функции подается присваевает объект, а не тот что мы изменяем
        }
        delete []S; // удаляем старое содержимое объекта
        len = c.len;
        S = new char[len + 1];
        strcpy(S, c.S);
        cout << "| ===== |";
        return *this;
    };
    ~Str() // а это деконструктор, запускается если пора удалять объект, когда пора решает компилятор
    {
        cout << "|Deconstructor " << this->S << " |";
        delete []S;
    };
    int getlen() // метод возвращающий длину строки, так как поля у нас все private, то доступ к ним может получить только функция из класса
    {
        cout << "|Get len |";
        return this->len;
    };
    char* getstr() // этот метод чисто для галочки, в идеале надо было переопределить операцию <<
    {
        cout << "|Get str |";
        return this->S;
    };
    void set(const char * str0) // тут тоже ничего интересного
    {
        if(str0 == this->S)
        {
            return;
        }
        delete []S;
        len = strlen(str0);
        S = new char[len+1];
        strcpy(S, str0);
        cout << "|Set |";
    };
};


class Man // Man...
{
private:
    Str name;
    int age;
public:
    Man(){}; // тут у нас ничего не сломается даже если оставить конструктор по умолчанию пустым
    Man(Str name, int age) // конструктор
    {
        this->name = name;
        this->age = age;
        cout << "|Constructor: |";
    };
    Man(const Man & c) // копировние
    {
        age = c.age;
        name = c.name;
        cout << "|Copy |";
    };
    ~Man(){}; // тут всё само без нас удалится
    Str getname() // тут всё по аналогии
    {
        cout << "|Get name |";
        return this->name;
    };
    int getage()
    {
        cout << "|Get age |";
        return this->age;
    };
    void setname(Str N)
    {
        this->name = N;  // так как = для str уже задана, то всё отработает корректно
        cout << "|Set N|";
    };
    void setage(int A)
    {
        this->age = A;
        cout << "|Set A|";
    };
};





int main()
{
    Str str1("1");
    cout << " ///:" << str1.getstr() << " " << str1.getlen() << endl;
    str1.set("22");
    cout << " ///:" << str1.getstr() << " " << str1.getlen() << endl;
    str1 = "Peter";
    cout << " ///:" << str1.getstr() << " " << str1.getlen() << endl;
    Str str2(str1); // копирование
    cout << " ///:" << str2.getstr() << " " << str2.getlen() << endl;
    
    Man maan(str1, 19);
    cout << endl;
    cout << " ///////:" << maan.getname().getstr() << " " << maan.getage() << endl;

    maan.setname("Tom");
    maan.setage(23); 
    cout << endl;
    cout << " ///////:" << maan.getname().getstr() << " " << maan.getage() << endl;
    return 0;
}




