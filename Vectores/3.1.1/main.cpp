#include <iostream>
using namespace std;

#define SIZE 10

int gVect[SIZE];
int gnCount=0;//tamaño de elmentos


void display(int gVect[])
{
    cout<<"Elementos del vector:\n";
    for(int i=0;i<gnCount;i++)
        cout<<gVect[i]<<" ";
}

void insert(int elem)
{
    if(gnCount<SIZE)
        gVect[gnCount++] = elem;
}

int main() {

    insert(5);
    display(gVect);

    return 0;
}