#include <iostream>
#include <cstring>

using namespace std;

class Arr 
{
    int * arr;
    int size;
public:
    friend ostream & operator << (std::ostream &os, const Arr &arr);
    Arr(){arr = nullptr;}
    Arr(int s, int date=0)
    {
        cout << "std" << endl;
        size = s;
        arr = new int[s];
        for(int i=0;i<size;i++)
        {
            arr[i] = date;
        };
    }
    //////
    Arr(Arr & moved)
    {
        cout << "COPY" <<endl;
        size = moved.size;
        arr = new int[size];
        for (int i = 0; i < size; i++) 
        {                
            arr[i] = moved.arr[i];
        }
    }
    
    Arr(Arr && moved)
    {
        cout << "MOVE" <<endl;
        size = moved.size;  
        //delete []arr;
        arr = moved.arr; 
        moved.arr = nullptr;
        
    }
    
    ~Arr ()
    {
        delete[] arr;
    }

    Arr& operator=(const Arr& other) 
    {
        if (this != &other) 
        {
            delete[] arr;
            cout << "operator =" << endl;
            size = other.size;
            arr = new int[size];
            for (int i = 0; i < size; i++) 
            {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }
    Arr& operator=(Arr && other) 
    {
        if (this != &other) 
        {
            cout << "operator copy=" << endl;
            size = other.size;  
            delete []arr;
            arr = other.arr; 
            other.arr = nullptr;
        }
        return *this;
    }
    //////
    
    Arr operator|(const Arr& other) const 
    {
        Arr result(max(size, other.size));
        for (int i = 0; i < min(size, other.size); i++) 
        {
            result.arr[i] = arr[i] | other.arr[i];
        }
        return result;
    }

    Arr operator|(int value) const 
    {
        Arr result(size);
        for (int i = 0; i < size; i++) 
        {
            result.arr[i] = arr[i] | value;
        }
        return result;
    }
    
    Arr & operator|=(int value) 
    {
        for (int i = 0; i < size; i++) 
        {
            arr[i] = value;
        }
        return *this;
    }

    Arr operator-() const 
    {
        Arr result(size);
        for (int i = 0; i < size; i++) 
        {
            result.arr[i] = -arr[i];
        }
        return result;
    }

    int& operator[](int index) 
    {
        if ((index >= 0) && (index < size)) 
        {
            return arr[index];
        } 
        else
        {
            exit(1);
        }
    }    
    


};   

std::ostream& operator << (std::ostream &os, const Arr &arra)
{   
    for(int i = 0; i<arra.size; ++i)
    {
        os << arra.arr[i] << " ";
    }    
    os << endl;  
    return os;

}

int main()
{
    Arr a1(15), a2(20,5), a3(30,3);
    const Arr a4(5);
    Arr a5 = -a1;
    cout << a1 << a2 << a3 << a4 << endl;
    a2 = a2 | 2; // объект а2 здесь не меняется a2 =1;
    a2 |= 1; // а здесь меняется
    a3= (-a2) | (-a4);
    a3[5] = 10;
    a1 = a4 | a3;
    cout << a1 << a2 << a3 << a4 << endl;
    

    return 0;
}