#include <iostream>
using namespace std;

#define SIZE 10

int gVect[SIZE];
int gnCount=0;//tamaño de elmentos


void display(int gVect[])
{
    cout<<"\nElementos del vector:\n";
    for(int i=0;i<gnCount;i++)
        cout<<gVect[i]<<" ";
    if(gnCount==0){
        cout<<"Empty"<<endl;
    }
    cout<<'\n';
}

void insert(int elem)
{
    if(gnCount<SIZE)
        gVect[gnCount++] = elem;
}

int buscar(int elem)
{
    for(int i=0; i<gnCount;++i)
    {
        if(elem == gVect[i])
        {
            return i;
        }
    }
    return -1;
}

void delete_i(int elem) {
    int x = 0;
    int index = buscar(elem);
    if (index >= 0 && index < gnCount)
    {
        x=gVect[index];
        for (int i = index; i < gnCount; i++)
            gVect[i] = gVect[i + 1];
        gnCount--;
        cout<<"\nSe elimino el elemento: "<<x<<endl;
    }
    else {
        cout << "\nError";
    }
}

int main() {

    insert(5);
    insert(3);
    insert(4);

    display(gVect);

    delete_i(3);


    display(gVect);


    return 0;
}