#include <iostream>
using namespace std;

float Addition(float a, float b)
{
    return a + b;
}

float Subtraction(float a, float b)
{
    return a - b;
}

float Division(float a, float b)
{
    return a / b;
}

float Multiplication(float a, float b)
{
    return a * b;
}

void my_switch(int a, int b, int opt)
{
    float c;
    switch (opt) {
        case 0:
            c = Addition(a, b);
            cout<<c;
            break;
        case 1:
            c = Subtraction(a, b);
            cout<<c;
            break;
        case 2:
            c = Multiplication(a, b);
            cout<<c;
            break;
        case 3:
            c = Division(a, b);
            cout<<c;
            break;
            // more cases operations go here
    }

}


int main() {
// Code A
    float a, b, c;
    int opt;
// enter the operands
    cout << "Ingrese 2 valores: "<<endl;
    cin >> a >> b;
    cout << "OPCIONES: 0-Addition, 1-Subtraction, 3-Multiplication, 4-Division";
    cin >> opt;


// Code B
// User type to simplify the declaration
    typedef float (*lpfnOperation)(float, float);
// CVector of pointer to functions
    lpfnOperation vpf[4] = {&::Addition, &::Subtraction,
                            &::Multiplication, &::Division};

// The next line replaces the switch and replaces the whole switch
    c = (*vpf[opt])(a, b);
    cout << c;
}