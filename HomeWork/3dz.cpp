#include <iostream>

using namespace std;


class Matrix
{
    int size = 0;
    int *num = nullptr;
    friend ostream& operator<< (ostream &os, const Matrix &m);
public:
    Matrix()
    {
        size = 3;
        num = new int[3*3];
        for(int i = 0; i < 3*3; num[i++] = 0){}
    }
    Matrix(int s)
    {
        size = s;
        num = new int[s*s];
        for(int i = 0; i < s*s; num[i++] = 0){}

    }
    ~Matrix()
    {
        delete []num;
    }
    int* operator[](int index)
    {
        if(index < size)
            return &num[index*size];
        else
            exit(1);
    }
    const int* operator[](int index) const
    {
        if(index < size)
            return &num[index*size];
        else
            exit(1);
    }
    

};

ostream& operator<< (ostream &os, const Matrix &m)
    {
        for(int i = 0; i < m.size; i++)
        {
            for(int j = 0; j < m.size; j++)
                os << m.num[i*m.size+j] << " ";
        os << endl;
        }
        return os;
    }

int main()
{
    const Matrix mc;
    Matrix m;
    m[1][2] = 5;
    cout << m << mc;
    int x = m[2][3];
    cout << x << endl;
    cout << mc[1][1];
    //mc[1][1] = 100;
    
    return 0;
}