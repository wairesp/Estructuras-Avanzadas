#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>
#include <math.h>
#include <chrono>

using namespace std;

vector<int> mydata;
vector<int> delete_list;
vector<int> search_list;
bool printflag = 1;

template<class T>
class SkipList {
public:
    SkipList(int level, float prob);
    ~SkipList();
    void find(int searchKey) ;
    void insert(int searchKey,  T newValue);
    void borrar(int searchKey);
    void displayList () ;
    void print();

private:
    struct Node {
        int key;
        T value;
        //apunta hacia los nodos sucesores
        std::vector<Node*> forward;
        Node(int k, T v, int level):
                forward(level,nullptr)
        {
            this->key=k;
            this->value=v;
        }

        Node(int k, int level):
                forward(level,nullptr)
        {
            this->key=k;
        }
    };

    static int nodeLevel(const Node* v);
    int randomLevel() ;
    // data members
    float probabilidad;
    int maxLvl;
    Node* head; // pointer to first node
    Node* last_node;  // last node
};

/***************Members********************/

template<class T>
int SkipList<T>::nodeLevel(const Node* v) {
    return v->forward.size();
}

template<class T>
int SkipList<T>::randomLevel()  {
    int v = 1;
    //aleatorio entre 0.0 y 1.0
    //float myr = (float)std::rand() / RAND_MAX;
    while ( (double)std::rand() / RAND_MAX < probabilidad && v < maxLvl) {
        v++;
    }
    return v;
}

template<class T>
SkipList<T>::SkipList(int level, float prob) {
    this->probabilidad = prob;
    this->maxLvl = level;

    int headKey = std::numeric_limits<int>::min();


    head = new Node(headKey, maxLvl);

    int lastKey = std::numeric_limits<int>::max();
    last_node = new Node(lastKey, maxLvl);

    std::fill(head->forward.begin(), head->forward.end(), last_node);
}

template<class T>
SkipList<T>::~SkipList() {
    auto node = head;
    while (node->forward[0]) {
        auto tmp = node;
        node = node->forward[0];
        delete tmp;
    }
    delete node;
}

template<class T>
void SkipList<T>::insert(int searchKey, T newValue) {
    int niveles = nodeLevel(head);
    unsigned int n = niveles;
    std::vector<Node*> update(niveles,nullptr);
    Node* x = head;

    //determinamos que nodos los nodos que pueden actualizarse con un insert
    while ( n-- > 0) {
        while ( x->forward[n]->key < searchKey) {
            x = x->forward[n];
        }
        update[n] = x;
        //std:: cout<< x->forward[n]->value <<std::endl;
    }

    //reasigno el valor si ya existe en la lista
    Node* next = update[0]->forward[0];
    if (next->key == searchKey && next != last_node) {
        next->value = newValue;
        return;
    }

    // creo un nuevo nodo
    const int newNodeLevel = randomLevel();
    Node* newNodePtr =  new Node(searchKey, newValue, newNodeLevel);

    // conecto los nodos predecesores a los sucesores correspondientes
    for (int i = 0; i < newNodeLevel; ++i) {
        newNodePtr->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNodePtr;
    }
}

template<class T>
void SkipList<T>::find(int searchKey) {
    //nodo actual
    Node* aux = head;
    unsigned int i = nodeLevel(head);

    while(i-- > 0) {
        while ( aux->forward[i]->key < searchKey) {
            aux = aux->forward[i];
        }
    }
    Node* current = aux->forward[0];
    if(printflag == 1) {
        if (current->key == searchKey && current != last_node) {
            std::cout << "Q: find(key =" << searchKey << "): Value: " << '"' << current->value << '"' << '\n';
        } else {
            std::cout << "Q: find(key =" << searchKey << "): not found" << endl;
        }
    }
}

template<class T>
void SkipList<T>::borrar(int searchKey) {
    int niveles = nodeLevel(head);
    unsigned int n = niveles;
    std::vector<Node*> update(niveles,nullptr);
    Node* aux = head;

    while ( n-- > 0) {
        while ( aux->forward[n]->key < searchKey) {
            aux = aux->forward[n];
        }
        update[n] = aux;
        //std:: cout<< x->forward[n]->value <<std::endl;
    }

    //check if the node exists
    Node* node = update[0]->forward[0];
    if (node->key != searchKey || node == last_node) {
        return;
    }

    // update pointers and delete node
    for (size_t i = 0; i < nodeLevel(node); ++i) {
        update[i]->forward[i] = node->forward[i];
    }
    delete node;
}

template<class T>
void SkipList<T>::displayList()
{
    std::cout<<"\n";
    for(int i=0;i<=maxLvl;i++)
    {
        Node *node = head->forward[i];
        std::cout<<"Level "<<i<<": ";
        std::cout<<"[";
        while(node!= nullptr && node !=last_node)
        {
            std::cout<<" key: "<<node->key<<" ";
            std::cout<<"value: "<<node->value<<"";
            std::cout<<"->";
            node = node->forward[i];
            if(node!=last_node)
            {
                std::cout<<" ";
            }

        }
        std::cout<<"]";
        std::cout<<"\n";
    }
}

template<class T>
void SkipList<T>::print()  {
    Node* list = head->forward[0];
    int lineLenght = 0;

    std::cout << "{";

    while (list != last_node) {
        std::cout << "value: " << list->value
                  << ", key: " << list->key
                  << ", level: " << nodeLevel(list);

        list = list->forward[0];

        if (list != last_node) std::cout << " : ";

        if (++lineLenght % 2 == 0) std::cout << "\n";
    }
    std::cout << "}\n";
}

int main() {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 9999);//numeros entre 1 y 999

    int elementos[5] = {1000,10'000,100'000,1'000'000,10'000'000};
    int* data= new int[10'000'000];


    SkipList<std::string> my_list(4,0.5);
    SkipList<int> my_list2 (4,0.5);


    std::cout<<"\n***********TEST 1 STRING:***************\n";

    my_list.insert(5, "estas");
    my_list.insert(4, "como");
    my_list.insert(3, "mundo");
    my_list.insert(2, "hola");


    my_list.displayList();
    my_list.find(2);

    std::cout<<"\nREASIGNAR 4:";
    my_list.insert(4, "DONDE");
    my_list.displayList();

    std::cout<<"\nBORRAR 2:";
    my_list.borrar(2);

    my_list.displayList();

    std::cout<<"\n***********TEST 2 ENTERO:***************\n";


    my_list2.insert(3,3);
    my_list2.insert(6,6);
    my_list2.insert(7,7);
    my_list2.insert(9,9);
    my_list2.insert(12,12);
    my_list2.insert(19,10);
    my_list2.insert(26,26);
    my_list2.insert(21,21);
    my_list2.insert(25,25);

    my_list2.displayList();

    std::cout<<"\nINSERTAR KEY 17:";

    my_list2.insert(17,17);

    my_list2.displayList();

    std::cout<<"\nBORRAR KEY 6:";
    my_list2.borrar(6);

    my_list2.displayList();

    std::cout<<"\n***********TEST DE TIEMPOS:***************\n";


    //Test para hacer desde 100 hasta un millon de operaciones de insercion con llaves aleatorias
    printflag =0;
    for(int elem = 0 ; elem < 5; ++elem){
        SkipList<int> test_list(100,0.5);
        //llenar hasta los elementos deseados
        for(int i = 0; i<elementos[elem];++i) {
            mydata.push_back(i);
        }

        for(size_t i = 0; i < mydata.size(); i++){
            if( rand() % 4 == 0 ){
                delete_list.push_back(mydata[i]);
            }
        }

        for(size_t i = 0; i < mydata.size(); i++){
            if( rand() % 4 == 0 ){
                search_list.push_back(mydata[i]);
            }
        }

        //begin time
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();
        //medimos tiempo solo de la operacion insert
        for(int j = 0; j<elementos[elem];++j)
            test_list.insert(j,mydata[j]);
        //end time
        end = std::chrono::high_resolution_clock::now();
        int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << "Tiempo con " << elementos[elem] <<" operaciones de insercion "<< ": " << duration << " ns." << " equivalente a " \
        << duration*0.000000001f << " s." <<"\n";

        //begin time
        std::chrono::time_point<std::chrono::high_resolution_clock> start2, end2;
        start2 = std::chrono::high_resolution_clock::now();
        //medimos tiempo solo de la operacion delete
        for(int j = 0; j<delete_list.size();++j)
            test_list.borrar(delete_list[j]);
        //end time
        end2 = std::chrono::high_resolution_clock::now();
        int64_t duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2).count();
        std::cout << "Tiempo con " << delete_list.size() <<" operaciones de eliminacion "<< ": " << duration2 << " ns." << " equivalente a " \
        << duration2*0.000000001f << " s." <<"\n";

        std::chrono::time_point<std::chrono::high_resolution_clock> start3, end3;
        start3 = std::chrono::high_resolution_clock::now();
        //medimos tiempo solo de la operacion search
        for(int j = 0; j < search_list.size(); ++j)
            test_list.find(search_list[j]);
        //end time
        end3 = std::chrono::high_resolution_clock::now();
        int64_t duration3 = std::chrono::duration_cast<std::chrono::nanoseconds>(end3 - start3).count();
        std::cout << "Tiempo con " << search_list.size() << " operaciones de busqueda " << ": " << duration3 << " ns." << " equivalente a " \
 << duration3*0.000000001f << " s." <<"\n\n";

        mydata.clear();
        delete_list.clear();
        search_list.clear();
    }


    //my_list2.print();
    std::cout << "\nFIN!\n";

    return 0;
}