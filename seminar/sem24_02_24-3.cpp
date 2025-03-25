#include <iostream>
#include <string>

using namespace std;


class BinaryNumber 
{
    string binary;
public:
    BinaryNumber(const string &s) : binary(s) // Удаляем ведущие нули, если они есть
    {
        binary.erase(0, min(binary.find_first_not_of('0'), binary.size() - 1)); // Петр кидал варик по проще
    }
    operator string() 
    {
        return binary; // возвращаем строку
    }
    void operator++(int) // оператор i++
    {
        int carry = 1; // керри флаг получается (привет ассемблер)
        for (int i = binary.size() - 1; i >= 0; i--)
        {
            if (binary[i] == '0') // типичный алгоритм сложения в двоичной с.с.
            {
                binary[i] = '1';
                carry = 0;
                break;
            } 
            else 
            {
                binary[i] = '0';
            }
        }
        if (carry) 
        {
            binary.insert(0, "1");
        }
    }
};

int main() 
{
    string inputstr;
    cout << "Введите число в двоичном виде: ";
    cin >> inputstr;
    BinaryNumber binaryNumber(inputstr);
    binaryNumber++;
    cout << "Результат++: " << static_cast<string>(binaryNumber) << endl;
    return 0;
}