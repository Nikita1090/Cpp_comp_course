#include <iostream>
#include <cstring>

using namespace std;

class ElemOfData // пункт 2
{
    char* Name;
    unsigned long int phone_number;
    int number_of_order;
public:
    ElemOfData()
    {
        Name = new char[1];
        Name[0] = '\0';
        phone_number = 0;
        number_of_order = -1;
        cout << "|Constructor " << " |  ";
    }
    ElemOfData(const char* name, const unsigned long int num, const int order)
    {
        Name = new char[strlen(name) + 1];
        strcpy(Name, name);
        phone_number = num;
        number_of_order = order;
        cout << "|Constructor " << Name << " |  ";
    }
    ElemOfData(const ElemOfData & c) // тут ничего интересного, всё как обычно
    {
        Name = new char[strlen(c.Name) + 1];
        strcpy(Name, c.Name);
        phone_number = c.phone_number;
        number_of_order = c.number_of_order;
        cout << "|Constructor Copy " << " |  ";
    }
    ElemOfData(ElemOfData && c)
    {
        char* b = c.Name;
        c.Name = Name;
        Name = b;
        phone_number = c.phone_number;
        number_of_order = c.number_of_order;
        cout << "|Constructor Move " << " |  ";
    }
    ElemOfData & operator=(const ElemOfData & c)
    {
        delete []Name;
        Name = new char[strlen(c.Name) + 1];
        strcpy(Name, c.Name);
        phone_number = c.phone_number;
        number_of_order = c.number_of_order;
        cout << "|Operator= " << " |  ";
        return *this;
    }
    ~ElemOfData()
    {
        cout << "|Deconstructor " << Name << " |";
        delete[] Name;
        Name = nullptr; // на всякий случай
    }
    void edit(const char* name, const unsigned long int num, const int order) // put nullptr; 0; -1; to do nothing
    {
        cout << "|func edit() " << Name << "->" << name << " |  ";
        if(name != nullptr)
        {
            delete[] Name;
            Name = new char[strlen(name) + 1];
            strcpy(Name, name);
        }
        if(num != 0)
        {
            phone_number = num;
        }
        if (order >= 0)
        {
        number_of_order = order; 
        }
        
    }
    char* get_Name()
    {
        cout << "|getN() " << " |";
        char* b = new char[strlen(Name) + 1];
        strcpy(b, Name);
        return b;
    }
    unsigned long int get_phone() // геттеры
    {
        cout << "|getph() " << " |";
        return phone_number;
    }
    int get_order()
    {
        cout << "|getorder() " << " |";
        return number_of_order;
    }
    void print_all()
    {
        cout << "Zakaz: " << Name << " " << phone_number << " " << number_of_order << endl;
    }
};

class list
{
    struct listEl // структура списка
    {
        listEl* next = nullptr;
        listEl* prev = nullptr;
        ElemOfData* data = nullptr; //сами данные хранятся вне структуры списка
    };
    listEl* start = nullptr;
    listEl* end = nullptr;
    
public:
    list(){}
    ~list()
    {
        if (start != nullptr)
        {
            while(start != nullptr)
            {
                end = start;
                start = start->next;
                delete end->data;
                delete end;
            }
        }
    }
    void push_front(const ElemOfData & c)
    {
        listEl* buf = new listEl;
        buf->data = new ElemOfData;
        *buf->data = c;
        if(this->empty() == 1)
            end = buf;
        buf->next = start;
        if (buf->next != nullptr)
        {
            buf->next->prev = buf;
        }
        buf->prev = nullptr;
        start = buf;
    }
    void push_back(const ElemOfData & c)
    {
        listEl* buf = new listEl;
        buf->data = new ElemOfData;
        *(buf->data) = c;
        if(this->empty() == 1)
            start = buf;
        buf->prev = end;
        if (buf->prev != nullptr)
        {
            buf->prev->next = buf;
        }
        buf->next = nullptr;
        end = buf;
    }
    ElemOfData* front() const
    {
        if(this->empty() == 1)
        {
            return nullptr;
        }
        ElemOfData* buf = new ElemOfData(*(start->data));
        return buf;
    }
    ElemOfData* back() const
    {
        if(this->empty() == 1)
        {
            return nullptr;
        }
        ElemOfData* buf = new ElemOfData(*(end->data));
        return buf;
    }
    ElemOfData* pop_back()
    {
        if(this->empty() == 1)
        {
            return nullptr;
        }
        ElemOfData* buf = new ElemOfData(*(end->data));
        if(start == end)
        {
            start = nullptr;
        }
        listEl* bb = end;
        end = end->prev;
        if(bb->prev != nullptr)
        {
            bb->prev->next = nullptr;
        }
        delete bb->data;
        delete bb;
        return buf;
    }
    ElemOfData* pop_front()
    {
        if(this->empty() == 1)
        {
            return nullptr;
        }
        ElemOfData* buf = new ElemOfData(*(start->data));
        if(start == end)
        {
            end = nullptr;
        }
        listEl* bb = start;
        start = start->next;
        if(bb->next != nullptr)
        {
            bb->next->prev = nullptr;
        }
        delete bb->data;
        delete bb;
        return buf;
    }
    void insert(unsigned int p, const ElemOfData & c)
    {
        if(this->size() == 1) // некоторые ситуации сводятся к уже написанным алгоритмам, похожий код будет встречаться далее
        {
            this->push_front(c);
            return;
        }
        listEl* pi = start;
        while((pi->next != nullptr) && (p != 0))
        {
            pi = pi->next;
            p--;
        }
        if (pi == end)
        {
            this->push_back(c);
            return;
        }
        listEl* buf = new listEl;
        buf->data = new ElemOfData;
        *(buf->data) = c;
        buf->prev = pi;
        buf->next = pi->next;
        pi->next = buf;
        if (buf->next != nullptr)
            buf->next->prev = buf;

    }
    ElemOfData* erase(unsigned int p)
    {
        if(this->size() == 1)
        {
            return this->pop_front();
        }
        listEl* pi = start;
        while((pi->next != nullptr) && (p != 0))
        {
            pi = pi->next;
            p--;
        }
        if(pi == start)
        {
            return this->pop_front();
        }
        if(pi == end)
        {
            return this->pop_back();
        }

        ElemOfData* buf = new ElemOfData(*(pi->data));
        listEl* bb = pi;
        pi->next->prev = pi->prev;
        pi->prev->next = pi->next;
        delete bb->data; // не забываем про данные внутри
        delete bb;
        return buf;

    }
    int empty() const
    {
        if(start == nullptr)
        {
            return 1;
        }
        return 0;
    }
    unsigned int size() const // решил не хранить отдельно длину списка
    {
        listEl* pi = start;
        unsigned int p = 0;
        while(pi != nullptr)
        {
            pi = pi->next;
            p++;
        }  
        return p;
    } 
    void print() const
    {
        listEl* pi = start;
        cout << "List of size " << this->size() << ":" << endl;
        if (pi == nullptr)
        {
            cout << "-empty" << endl;
        }
        while(pi != nullptr)
        {
            char* u = pi->data->get_Name(); // скажем нет утечкам памяти!!!
            cout << u << endl;
            cout << "\t+order: " << pi->data->get_order() << endl;
            cout << "\t+phone: " << pi->data->get_phone() << endl;
            pi = pi->next;
            delete []u;
        }  
    }
};




int main()
{
    ElemOfData zakaz1("Yandex", 79927674781, 104);
    ElemOfData zakaz2(zakaz1);
    zakaz2.edit("NVIDIA", 139995651634, 105);
    ElemOfData zakaz3;
    zakaz3.edit("Google", 119907566644, 106);
    zakaz1.print_all();
    zakaz2.print_all();
    zakaz3.print_all();
    ////////////////////////// секция списка
    cout << endl << "--------------------------------" << endl;
    list List1;
    cout << "list created" << endl;
    cout << "Empty? " << (List1.empty() == 1? "Yes" : "No") << endl;
    List1.push_back(zakaz3);
    List1.push_front(zakaz1);
    List1.insert((unsigned) 0, zakaz2); /////////////
    cout << "list pushed" << endl;
    cout << "Empty? " << (List1.empty() == 1? "Yes" : "No") << endl;
    List1.print();
    ElemOfData* what_we_get = List1.erase(1); // мои удаляющие функции работают как pop в стеке. Это конечно не требовалось, но почему нет?
    delete what_we_get;
    cout << "++++++++"<< endl;
    ElemOfData* buf1 = List1.front();
    buf1->print_all();
    delete buf1;
    buf1 = List1.back();
    buf1->print_all();
    delete buf1;
    what_we_get = List1.pop_back();
    delete what_we_get;
    what_we_get = List1.pop_front();
    delete what_we_get;

    list List2;
    List1.push_back(zakaz3);
    List1.push_front(zakaz1);
    List1.insert(0, zakaz2);

    what_we_get = List1.erase(2); // мои удаляющие функции работают как pop в стеке. Это конечно не требовалось, но почему нет?
    delete what_we_get;
    what_we_get = List1.erase(1); // мои удаляющие функции работают как pop в стеке. Это конечно не требовалось, но почему нет?
    delete what_we_get;
    what_we_get = List1.erase(0); // мои удаляющие функции работают как pop в стеке. Это конечно не требовалось, но почему нет?
    delete what_we_get;
    return 0;
}
