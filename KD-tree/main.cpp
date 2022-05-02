#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cmath>
#include <limits>


using namespace std;

void printVecInt(vector<int> p) {
    cout << "|";
    for (int i: p)
        std::cout << i << '|';
    cout << endl;
}


//Clase menor
template <class T>
class menor
{
public:
    inline bool operator()(T x, T y, int altura) {
        return x[altura] < y[altura];
    }
};

//Clase mayor
template <class T>
class mayor
{
public:
    inline bool operator()(T x, T y, int altura) {
        return x[altura] > y[altura];
    }
};

typedef vector<int> vecInt;
typedef menor<vecInt> menorIntVec;
typedef pair<float, vecInt> pair_D_VecInt;

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

//// ///////////////////////////////////////
struct S
{
    float dist_S;
    vecInt punto_S;

    S(float i, vecInt punto) {
        dist_S = i;
        punto_S = punto;
    }
    friend bool operator< (S const& x, S const& y) { return x.dist_S < y.dist_S; }
};

class BPQ
{
public:
    priority_queue<S> _bpq;
    BPQ(){
        this->k = 0;
        cont_elem=0;
    }
    BPQ(int k){
        this->k = k;
        cont_elem=0;
    }
    int k;
    int cont_elem;

    int sizeBPQ() {
        return _bpq.size();
    }

    float topBPQ(){
        return _bpq.top().dist_S;
    }

    void printBPQ(){
        while(!emptyBPQ()) {
            std::cout << topBPQ() << ' ';
            printVecInt(topVecBPQ());
            cout << endl;
            deleteBPQ();
        }
        std::cout << '\n';
    }

    void set_k(int _k) {
        this->k = _k;
    }

    bool emptyBPQ(){
        return _bpq.empty();
    }

    void insertBPQ(S data){
        _bpq.push(data);
        cont_elem++;
        if (cont_elem > k) {
            deleteBPQ();
        }
    }
    void deleteBPQ(){
        _bpq.pop();
    }

    float topDistBPQ(){
        return _bpq.top().dist_S;
    }
    vecInt topVecBPQ(){
        return _bpq.top().punto_S;
    }
};



//// ///////////////////////////////////////
template <class T, class C>
class KDTree
{
public:
    int dim;
    int totalNodes;
    KDTree(int n_dim)
    {
        this->dim = n_dim;
        this->root = nullptr;
        totalNodes = 0;
    }

    bool allDiff(T data, T x) {
        int temp=0;
        for (int i = 0; i < dim; ++i) {
            if (data[i] == x[i]) {
                temp++;
            }
            if (temp==dim) {
                return false;
            }
        }
        return true;
    }

    bool find(T x, Node<T>**& p, int& tempAltura) {
//  (*p)->data != x
        for (
                p = &root;
                *p && allDiff((*p)->data, x);
                p = &((*p)->nodes[cmp((*p)->data, x, (*p)->altura % dim)]), tempAltura++
                );
        return !!(*p);
    }

    void buscar_punto(T punto)
    {
        Node<T>** p;
        int altura;
        if(find(punto,p, altura)) {
            cout<<"Se encontro el dato ";
            printVecInt((*p)->data);
        }
        else
            cout<<"No se encontro el elemento\n";
    }

    bool insert(T x) {
        Node<T>** p;
        int tempAltura = 0;
        if (find(x, p, tempAltura))
            return 0;
        *p = new Node<T>(x, dim, tempAltura);
        totalNodes++;
        return 1;
    }

    float calc2Dist(T P1, T P2) {
        float sum=.0f;
        for (int i = 0; i < dim; ++i) {
            float diff = P1[i]-P2[i];
            float diffPow = diff*diff;
            sum+=diffPow;
        }
        return (sqrt(sum))*1.f;
    }

    /* nn BEGIN */
    BPQ s_nn;
    void _nn(T key) {
        Node<T>* m_nnc= nullptr;
        double bst_dst= numeric_limits<double>::infinity();
        S m_dst_p(bst_dst, key);
        _nnAux(root, m_nnc, m_dst_p , 0, key);
        printVecInt(m_dst_p.punto_S);
        cout<<m_dst_p.dist_S<<endl;
    }

    void _nnAux(Node<T> *curr, Node<T>* m_nnc, S &m_dst_p, int depth, T key) {
        if (!curr) {
            return;
        }
        float dist_nn = calc2Dist(curr->data, key)*1.f;
        if(dist_nn < m_dst_p.dist_S){
            m_dst_p.dist_S= dist_nn;
            m_nnc = curr;
            m_dst_p.punto_S = curr->data;
        }

        int axis = depth % dim;
        bool right = false;

        if (key[axis] < curr->data[axis]) {
            right = false;
            _nnAux(curr->nodes[0], m_nnc , m_dst_p,++depth, key);
        }
        else {
            right = true;
            _nnAux(curr->nodes[1], m_nnc, m_dst_p, ++depth, key);
        }

        int fabs_n = abs(curr->data[axis] - key[axis]);

        //       cout << endl;
        if (fabs_n < m_dst_p.dist_S) {
            if (right) {
                _nnAux(curr->nodes[0], m_nnc, m_dst_p,++depth, key);
            }
            else {
                _nnAux(curr->nodes[1], m_nnc,m_dst_p,++depth, key);
            }
        }
    }
    /* nn  END */


    /* knn vecinos BEGIN */
    BPQ nnc;
    void _knn(int k,  T key) {
        nnc.set_k(k);
        _knnAux(root, nnc, k, 0, key);
        nnc.printBPQ();
    }

    void _knnAux(Node<T> *curr, BPQ& nnc, int k, int depth, T key) {
        if (!curr) {
            return;
        }
        float dist_nn = calc2Dist(curr->data, key)*1.f;
//    cout << "Ref: ";  printVecInt(key);
//    cout << "PuntoCurr: "; printVecInt(curr->data);
//    cout << "distEucl: " << dist_nn << endl;
//    cout << "depth: " << depth << endl;
//    cout << endl;

        nnc.insertBPQ(S(dist_nn, curr->data));
        int axis = depth % dim;
        bool right = false;

        if (key[axis] < curr->data[axis]) {
            right = false; //si mi nodo es menor a la izq
            _knnAux(curr->nodes[0], nnc, k, ++depth, key);
        }
        else {
            right = true;
            _knnAux(curr->nodes[1], nnc, k, ++depth, key);
        }

        int fabs_n = abs(curr->data[axis] - key[axis]);
//    cout << "nnc.topBPQ(): " << nnc.topBPQ() << endl;
//    cout << "PuntoCurr: "; printVecInt(curr->data);
 //       cout << endl;
        if (fabs_n < nnc.topDistBPQ()) {
            if (right) {
                _knnAux(curr->nodes[0], nnc, k, ++depth, key);
            }
            else {
                _knnAux(curr->nodes[1], nnc, k, ++depth, key);
            }
        }
    }
    /* knn vecinos END */

    /* range query BEGIN */
    BPQ nncQ;
    void rangeQ(T x, int distQ){
        nncQ.set_k(totalNodes+2);
        _rangeQ_Aux(root, nncQ, totalNodes,0, x, distQ);
       // printVecInt(nncQ.topVecBPQ());
        nncQ.printBPQ();
    }

    void _rangeQ_Aux(Node<T> *curr, BPQ& nncQ, int k, int depth, T key, int distQ) {
        if (!curr) {
            return;
        }
        float dist_nn = calc2Dist(curr->data, key)*1.f;
        if (dist_nn<=distQ) {
            nncQ.insertBPQ(S(dist_nn, curr->data));
        }

        int axis = depth % dim;
        bool right = false;

        if (key[axis] < curr->data[axis]) {
            right = false;
            _rangeQ_Aux(curr->nodes[0], nncQ, k, ++depth, key, distQ);
        }
        else {
            right = true;
            _rangeQ_Aux(curr->nodes[1], nncQ, k, ++depth, key, distQ);
        }

        int fabs_n = abs(curr->data[axis] - key[axis]);
        if (fabs_n < nncQ.topDistBPQ()) {
            if (right)
                _rangeQ_Aux(curr->nodes[0], nncQ, k, ++depth, key, distQ);
            else
                _rangeQ_Aux(curr->nodes[1], nncQ, k, ++depth, key, distQ);
        }
    }
    /* range query END */

    Node<T>** rep(Node<T> **p) {
        bool dir = rand() % 2;
        for (
                p = &((*p)->nodes[dir]);
                (*p)->nodes[!dir];
                p = &((*p)->nodes[!dir])
                );
        return p;
    }


    bool remover(T x)
    {
        int altura=0;
        Node<T>** p;
        if (!find(x, p,altura)) return false;
        if ((*p)->nodes[0] && (*p)->nodes[1])
        {
            Node<T>** q = rep(p);
            (*p)->data = (*q)->data;

            p = q;
        }
        Node<T>* t = *p;
        *p = (*p)->nodes[!(*p)->nodes[0]];
        delete t;
        totalNodes--;
        return false;
    }

    void printDibujarArbol(Node<T>* p, int level) {
        if (!p)
            return;
        printDibujarArbol(p->nodes[1], level + 1);
        for (int i = 0; i < level; i++) {
            cout << "\t";
        }
        cout<<"|";
        for (int i = 0; i < dim; ++i) {
            cout << p->data[i] << "|";
        }
        cout << endl;

        printDibujarArbol(p->nodes[0], level + 1);
    }



    void inOrder();
    //Elementos
    C cmp; //Function Object
    Node<T>* root;
};

int main() {

    KDTree<
            vecInt,
            menorIntVec
    > arbol(3);

    vecInt v1(3);
    v1[0] = 3;
    v1[1] = 1;
    v1[2] = 4;
    arbol.insert(v1);

    vecInt v2(3);
    v2[0] = 2;
    v2[1] = 3;
    v2[2] = 7;
    arbol.insert(v2);

    vecInt v3(3);
    v3[0] = 4;
    v3[1] = 3;
    v3[2] = 4;
    arbol.insert(v3);

    vecInt v4(3);
    v4[0] = 2;
    v4[1] = 1;
    v4[2] = 3;
    arbol.insert(v4);

    vecInt v5(3);
    v5[0] = 2;
    v5[1] = 4;
    v5[2] = 5;
    arbol.insert(v5);

    vecInt v6(3);
    v6[0] = 1;
    v6[1] = 4;
    v6[2] = 4;
    arbol.insert(v6);

    vecInt v7(3);
    v7[0] = 6;
    v7[1] = 1;
    v7[2] = 4;
    arbol.insert(v7);

    vecInt v8(3);
    v8[0] = 6;
    v8[1] = 1;
    v8[2] = 4;
    arbol.insert(v8);

    vecInt v9(3);
    v9[0] = 5;
    v9[1] = 2;
    v9[2] = 5;
    arbol.insert(v9);

    vecInt v10(3);
    v10[0] = 4;
    v10[1] = 0;
    v10[2] = 6;
    arbol.insert(v10);

    vecInt v11(3);
    v11[0] = 7;
    v11[1] = 1;
    v11[2] = 6;
    arbol.insert(v11);

    vecInt v12(3);
    v12[0] = 0;
    v12[1] = 5;
    v12[2] = 7;
    arbol.insert(v12);

    vecInt puntito(3);
    puntito[0]=0;
    puntito[1]=4;
    puntito[2]=3;

    arbol.printDibujarArbol(arbol.root, 0);
    cout<<"busqueda: (0,4,3): "<<endl;
    arbol.buscar_punto(puntito);
    cout<<"busqueda (3,1,4): "<<endl;
    arbol.buscar_punto(v1);
    arbol.remover(v1);
    arbol.remover(v3);
    cout<<"post borrado:(3,1,4) y (5, 2,5) \n";
    arbol.printDibujarArbol(arbol.root, 0);

<<<<<<< HEAD
    cout<<"Range Query Elemento: (0,4,3)"<<endl;
    //arbol.rangeQ(arbol.root->data, 3);
    arbol.rangeQ(puntito, 9);

    int prioridad_k = 110;
    cout<<"knn de (0,4,3) con k="<<prioridad_k<<endl;
    arbol._knn(prioridad_k, puntito);

    cout<<"nn de (0,4,3) "<<endl;
    arbol._nn( puntito);
=======
    cout<<"Range Query Elemento: (6,1,4)"<<endl;
    arbol.rangeQ(arbol.root->data, 3);

    int prioridad_k = 3;
    cout<<"Knn de (0,4,3) con k="<<prioridad_k<<endl;
    arbol._knn(prioridad_k, arbol.root->data);
>>>>>>> 97bf85461434bc763c8873a5b5003c76bae2d724

    return 0;
}
