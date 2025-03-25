#include <iostream> 
 
using namespace std; 

// +- тоже самое что и 1 задача

class A 
{ 
    int a; 
    ~A(){} 
public: 
    A(int i = 0) : a(i) {} 
    static void destroy(A* elem) // прост в методе деструктор вызываем лол
    { 
        elem->~A(); // ктоб знал что это работает так? 
    } 
    int get_a() const 
    { 
        return a; 
    } 
}; 
 
int main() 
{ 
    // Создание массива с placement new (в беседе Егор в сб объяснил че это как)
    const int N = 5; 
    A* arr = static_cast<A*>(operator new[](N * sizeof(A))); 
    // Инициализация
    for (int i = 0; i < N; i++) 
    { 
        new(&arr[i]) A(i); 
    } 
    for (int i = 0; i < N; i++) 
    { 
        cout << "obj " << i << ": " << arr[i].get_a() << endl; 
    } 
    for (int i = 0; i < N; i++) 
    { 
        cout << "destroying: " << arr[i].get_a() << endl; 
        A::destroy(&arr[i]); 
        cout << "destroyed" << endl; 
    } 
    operator delete[](arr); 

    return 0; 
}