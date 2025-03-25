#include <iostream>

using namespace std;


// Абстрактный класс 
class Figure 
{
public:
    virtual double area() const = 0;
    virtual void print() const = 0;
};

// Производный класс 1
class Circle : public Figure 
{
    double radius_;
public:
    Circle(double radius):radius_(radius){}

    double area() const override 
    {
        return 3.14 * radius_ * radius_;
    }

    void print() const override 
    {
        cout << "Circle: radius = " << radius_ << ", area = " << area() << endl;
    }

};

// Производный класс 2
class Rectangle : public Figure 
{
    double width_, height_;
public:
    Rectangle(double width, double height):width_(width), height_(height){}

    double area() const override 
    {
        return width_ * height_;
    }

    void print() const override 
    {
        cout << "Rectangle: width = " << width_ << ", height = " << height_ << ", area = " << area() << endl;
    }

};


// Независимый класс FiguresWorker
class FiguresWorker 
{
    unsigned int size = 0;
    Figure** figures_ = nullptr;
public:
    FiguresWorker(int a)
    {
        size = a;
        figures_ = new Figure*[size];
        for(int i = 0; i < size; figures_[i++]=nullptr){}
    }

    ~FiguresWorker()
    {
        delete []figures_;
    }

    void addFigure(Figure* figure) 
    {
        int i = 0;
        while (figures_[i] != nullptr)
        {
            if(i >= size)
                return;
            i++;
        }
        figures_[i] = figure;
    }

    void printFigures() const 
    {
        for (int i = 0; i < size; i++)
        {
            figures_[i]->print();
        }
    }

    double totalArea() const 
    {
        double total = 0.0;
        for (int i = 0; i < size; i++)
        {
            total += figures_[i]->area();
        }
        return total;
    }

};

int main() {
    Circle circle(5.0);
    Rectangle rectangle(4.0, 6.0);

    FiguresWorker figuresWorker(2);
    figuresWorker.addFigure(&circle);
    figuresWorker.addFigure(&rectangle);

    figuresWorker.printFigures();

    cout << "Total area: " << figuresWorker.totalArea() << endl;

    return 0;
}
