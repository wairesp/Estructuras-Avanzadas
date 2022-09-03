#include <iostream>
using namespace std;

// Class CVector definition
template<class T>
class CVector
{
private:
    T *m_pVect; // Pointer to the buffer
    int m_nCount, // Control how many elements are actually used
    m_nMax, // Control how many are allocated as maximum
    m_nDelta; // To control the growing
    void Init(int delta); // Init our private variables, etc
    void Resize(); // Resize the vector when occurs an overflow
public:
    CVector<T>(int delta)
    {
        m_nDelta = delta;
        m_nCount = 0;
        m_nMax = 0;
    }; // Constructor
    void insert(T elem); // insert a new element
    void display();
    int buscar(T elem);
    void delete_i(T elem);
    // More methods go here
};

template <class T>
void CVector<T>::Init(int delta) {
    m_nDelta = delta;
}

template<class T>
void CVector<T>::Resize() {
    const int delta = 10; // Used to increase the vector size
    T *pTemp;
    int i;
    pTemp = new T[m_nMax + m_nDelta]; // Alloc a new vector
    for(i = 0 ; i < m_nMax ; i++) // Transfer the elements
        pTemp[i] = m_pVect[i]; // we can also use the function memcpy
    m_pVect = nullptr; // delete the old vector
    delete[]m_pVect;
    m_pVect = pTemp; // Update the pointer
    m_nMax += delta; // The Max has to be increased by delta
}

template<class T>
void CVector<T>::insert(T elem)
{
    if( m_nCount == m_nMax ) // Verify the overflow
        Resize(); // Resize the vector before inserting elem
    m_pVect[m_nCount++] = elem; // insert the element at the end
}

template<class T>
void CVector<T>::display()
{
    cout<<"\nElementos del vector:\n";
    for(int i=0;i<m_nCount;i++)
        cout<<m_pVect[i]<<" ";
    if(m_nCount==0){
        cout<<"Empty"<<endl;
    }
    cout<<'\n';
}

template<class T>
int CVector<T>::buscar(T elem)
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

template<class T>
void CVector<T>::delete_i(T elem) {
    T x = 0;
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
    CVector<char> myvec(10);

    myvec.insert('a');
    myvec.insert('b');
    myvec.insert('d');
    myvec.display();
    myvec.delete_i('a');
    myvec.display();

    return 0;
}
