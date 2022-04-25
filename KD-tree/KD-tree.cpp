#include <iostream>
#include <vector>
#include <queue>

using namespace std;

template <class T>
class menor
{
public:
    inline bool operator()(T x, T y, int altura) {
        return x[altura] < y[altura];
    }
};

template <class T>
class Node
{
public:
    Node(T x, int dim, int altura) {
        data.resize(dim);
        for (int i=0; i<dim; i++)
            data[i] = x[i];
        this->altura = altura;
        nodes[0] = nodes[1] = nullptr;
    }
    Node(int dim) {
        data.resize(dim);
        altura = 0;
    }

    int altura;
    T data;
    Node* nodes[2];
};

template <class T, class C>
class BinTree
{
public:
    int dim;
    BinTree(int n_dim)
    {
        this->dim = n_dim;
        this->root = 0;
    }
    bool allDiff(T data, T x) {
        for (int i = 0; i < dim; ++i) {
            if (data[i] != x[i]) {
                return true;
            }
        }
        return false;
    }

    bool find(T x, Node<T>**& p, int& tempAltura) {
        for (
                p = &root;
                *p && allDiff((*p)->data, x);
                p = &((*p)->nodes[cmp((*p)->data, x, (*p)->altura % dim)]), tempAltura++
                );
        return !!(*p);
    }

    bool insert(T x) {
        Node<T>** p;
        int tempAltura = 0;
        if (find(x, p, tempAltura))
            return 0;
        *p = new Node<T>(x, dim, tempAltura);
        return 1;
    }

    bool erase(T x);

//    void print(Node<T>* p, int level) {
//        if (!p)
//            return;
//        print(p->nodes[1], level + 1);
//        for (int i = 0; i < level; i++)
//            cout << "\t";
//        cout << p->data[0] << endl;
//        print(p->nodes[0], level + 1);
//    }

    void print(Node<T>* p, int level) {
        if (!p)
            return;
        print(p->nodes[1], level + 1);
        for (int i = 0; i < level*dim; i++) {
            cout << "\t";
        }
        cout << "|";
        for (int i = 0; i < dim; ++i) {
            cout << p->data[i] << "|";
        }
        cout << endl;
        print(p->nodes[0], level + 1);
    }
    void rep(Node<T>**& q);
    void inOrder();
    //Elementos
    C cmp; //Function Object
    Node<T>* root;
};

int main()
{
    BinTree<
    vector<int>,
            menor<vector<int>>
    > arbol(2);

    vector<int> v1(2);
    v1[0] = 100;
    v1[1] = 200;
    arbol.insert(v1);

    vector<int> v2(2);
    v2[0] = 100;
    v2[1] = 400;
    arbol.insert(v2);

    vector<int> v3(2);
    v3[0] = 20;
    v3[1] = 70;
    arbol.insert(v3);

    vector<int> v4(2);
    v4[0] = 1;
    v4[1] = 2;
    arbol.insert(v4);

    arbol.print(arbol.root, 0);
    return 0;
}