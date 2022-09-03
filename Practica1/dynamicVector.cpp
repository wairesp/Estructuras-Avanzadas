#include <iostream>
using namespace std;

int *gpVect = NULL;
int gnCount = 0;
int gnMax = 0;

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
void Resize()
{
    const int delta = 10; // Used to increase the vector size
    //gpVect = realloc(gpVect, sizeof(int) * (gnMax + delta));
    // You can also use the following code:
    int *pTemp, i;
    pTemp = new int[gnMax + delta]; // Alloc a new vector
    for(i = 0 ; i < gnMax ; i++) // Transfer the elements
        pTemp[i] = gpVect[i]; // we can also use the function memcpy
    delete [ ] gpVect; // delete the old vector
    gpVect = pTemp; // Update the pointer
    gnMax += delta; // The Max has to be increased by delta
}


void Insert(int elem)
{
    if( gnCount == gnMax ) // There is no space at this moment for elem
        Resize(); // Resize the vector before inserting elem
    gpVect[gnCount++] = elem; // Insert the element at the end of the sequence
}

int buscar(int elem)
{
    for(int i=0; i<gnCount;++i)
    {
        if(elem == gpVect[i])
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
        x=gpVect[index];
        for (int i = index; i < gnCount; i++)
            gpVect[i] = gpVect[i + 1];
        gnCount--;
        cout<<"\nSe elimino el elemento: "<<x<<endl;
    }
    else {
        cout << "\nError";
    }
}


int main() {

    Insert(10);
    Insert(3);
    Insert(4);


    display(gpVect);

    delete_i(3);

    display(gpVect);

    return 0;
}
