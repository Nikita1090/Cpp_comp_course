#include <iostream>
#include <vector>
#include <list>

using namespace std;

// Шаблонная функция для вывода содержимого контейнера в прямом порядке
template <typename Container>
void printContainer(const Container& cont) 
{
    for (typename Container::const_iterator it = cont.begin(); it != cont.end(); it++) 
    {
        cout << *it << " ";
    }
    cout << endl;
}

// Шаблонная функция для вывода содержимого контейнера в обратном порядке
template <typename Container>
void printContainerReverse(const Container& cont) 
{
    for (typename Container::const_reverse_iterator it = cont.rbegin(); it != cont.rend(); it++) 
    {
        cout << *it << " ";
    }
    cout << endl;
}

template <typename Arif>
void SummContainer(vector<Arif>& vec, const list<Arif>& list2) 
{

    typename list<Arif>::const_iterator itl = list2.cbegin();
    for (typename vector<Arif>::iterator it = ++(vec.begin()); it != vec.end(); it++) 
    {
        if(itl != list2.end())
        {
            it = vec.insert(it, *itl);
            it++;
            itl++;
            if(itl != list2.end())
                itl++;
        }
        else
        {
            it = vec.insert(it, static_cast<Arif>(0));
            it++;
        }
        it++;
        if(it == vec.end())
        {
            break;
        }

    }
    cout << endl;
}

template <typename Container>
void delN(Container& container, int n)
{
    if (n > container.size())
    {
        throw invalid_argument("too big arg\n");
    }
    typename Container::iterator i = container.begin();
    while(i != container.end())
    {
        size_t counter = 1;
        while (counter < n && not(++i == container.end()))
        {
            counter++;
        }

        if (i != container.end())
        {
            i = container.erase(i);
        }
    }
}
/*
template <typename Container>
    void PrintContainer (const Container& c){
        for (const auto& element: c){
            cout << element << " ";
        }
        cout << endl;
    }
*/


int main()
{
    vector<int> vect1 = {1, 2, 3, 4, 5};
    list<char> list1 = {'a', 'b', 'c', 'd', 'e'};
    list<int> list3 = {7, 7, 7};
    printContainer(vect1);
    printContainerReverse(vect1); 
    printContainer(list1);
    printContainerReverse(list1);
    int a, b;
    cout << "Enter count of numbers" << endl;
    cin >> a;
    vector<int> vect3;
    cout << "Enter numbers" << endl;
    for(int i = 0; i < a; i++)
    {
        cin >> b;
        vect3.push_back(b);
    }
    printContainer(vect3);
    SummContainer(vect3, list3);
    printContainer(vect3);
    //--------До этого момента принято на паре---------
    cout << "----------------------" << endl;
    try
    {
        delN(vect3, 2);
        printContainer(vect3);
        delN(list3, 3);
        printContainer(list3);
        delN(list3, 6);
        printContainer(list3);
    }
    catch(const invalid_argument& err)
    {
        cerr << "Err caught (but not into a pokeball): " << err.what() << endl;
    }
    return 0;
}
