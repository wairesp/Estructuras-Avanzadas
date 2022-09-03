#include <iostream>

using namespace std;


struct Mod_Vector{

    int* m_pVect; //POINTER TO BUFF
    int m_size;   //CURRENT MAX SIZE
    int m_nCount; //# OF ELEMENTS

};

void my_init(Mod_Vector *vec, int size){
    vec->m_size = size;
    vec->m_pVect=new int[vec->m_size];
    vec->m_nCount=0;//size
}


int size(Mod_Vector *vec){
    return vec->m_nCount;
}

void resize(Mod_Vector *vec){
    int* p=new int[2*vec->m_size];
    for( int i = 0; i < vec->m_size ; i ++){
        p[i]=vec->m_pVect[i];
    }
    delete[] vec->m_pVect;
    vec->m_pVect=p;
    vec->m_size= 2*vec->m_size;
}

void insert(Mod_Vector *vec , int x){
    if(vec->m_nCount == vec->m_size)
        resize(vec);
    vec->m_pVect[vec->m_nCount++]=x;
}


int buscar(Mod_Vector *vec, int elem)
{
    for(int i=0; i<vec->m_nCount;++i)
    {
        if(elem == vec->m_pVect[i])
        {
            return i;
        }
    }
    return -1;
}

void delete_i(Mod_Vector *vec, int elem) {
    int x = 0;
    int index = buscar(vec,elem);
    if (index >= 0 && index < vec->m_nCount)
    {
        x=vec->m_pVect[index];
        for (int i = index; i < vec->m_nCount; i++)
            vec->m_pVect[i] = vec->m_pVect[i + 1];
        vec->m_nCount--;
        cout<<"\nSe elimino el elemento: "<<x<<endl;
    }
    else {
        cout << "\nError";
    }
}


void display(Mod_Vector * vec)
{
    cout<<"\nElementos del vector:\n";
    for(int i = 0; i< vec->m_nCount; ++i)
        cout << vec->m_pVect[i] << " ";
    if(vec->m_nCount==0){
        cout<<"Empty"<<endl;
    }
    cout<<'\n';
}


int main() {

    Mod_Vector vec;

    my_init(&vec, 3);

    insert(&vec,1);
    insert(&vec,2);
    insert(&vec,3);
    insert(&vec,5);

    display(&vec);

    delete_i(&vec,5);

    display(&vec);
    // cout<<"\nsize Mod_Vector: "<<vec.size();

    return 0;
}