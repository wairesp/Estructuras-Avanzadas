#include <iostream>
using namespace std;

// Class CVector definition
class CVector
{
    private:
    int *m_pVect, // Pointer to the buffer
        m_nCount, // Control how many elements are actually used
        m_nMax, // Control how many are allocated as maximum
        m_nDelta; // To control the growing
        void Init(int delta); // Init our private variables, etc
        void Resize(); // Resize the vector when occurs an overflow
    public:
        CVector(int delta)
        {
            m_nDelta = delta;
            m_nCount = 0;
            m_nMax = 0;
        }; // Constructor
        void insert(int elem); // insert a new element
        void display();
        int buscar(int elem);
        void delete_i(int elem);
        // More methods go here
};

void CVector::Init(int delta) {
    m_nDelta = delta;
}

void CVector::Resize() {
    const int delta = 10; // Used to increase the vector size
    int *pTemp, i;
    pTemp = new int[m_nMax + m_nDelta]; // Alloc a new vector
    for(i = 0 ; i < m_nMax ; i++) // Transfer the elements
        pTemp[i] = m_pVect[i]; // we can also use the function memcpy
    m_pVect = NULL; // delete the old vector
    delete[]m_pVect;
    m_pVect = pTemp; // Update the pointer
    m_nMax += delta; // The Max has to be increased by delta
}

void CVector::insert(int elem)
{
    if( m_nCount == m_nMax ) // Verify the overflow
        Resize(); // Resize the vector before inserting elem
    m_pVect[m_nCount++] = elem; // insert the element at the end
}

void CVector::display()
{
    cout<<"\nElementos del vector:\n";
    for(int i=0;i<m_nCount;i++)
        cout<<m_pVect[i]<<" ";
    if(m_nCount==0){
        cout<<"Empty"<<endl;
    }
    cout<<'\n';
}

int CVector::buscar(int elem)
{
    for(int i=0; i<m_nCount;++i)
    {
        if(elem == m_pVect[i])
        {
            return i;
        }
    }
    return -1;
}

void CVector::delete_i(int elem) {
    int x = 0;
    int index = buscar(elem);
    if (index >= 0 && index < m_nCount)
    {
        x=m_pVect[index];
        for (int i = index; i < m_nCount; i++)
            m_pVect[i] = m_pVect[i + 1];
        m_nCount--;
        cout<<"\nSe elimino el elemento: "<<x<<endl;
    }
    else {
        cout << "\nError";
    }
}


int main() {
    CVector myvec(10);

    myvec.insert(88);
    myvec.insert(98);

    myvec.display();

    myvec.buscar(88);
    myvec.delete_i(88);

    myvec.display();

    return 0;
}
