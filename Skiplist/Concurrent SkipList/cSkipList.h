#ifndef CONcurrentNodeENT_SKIPLIST_CSKIPLIST_H
#define CONcurrentNodeENT_SKIPLIST_CSKIPLIST_H

#include <map>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <atomic>
#include <string>
#include <iostream>
#include <math.h>

using namespace std;

template<class T>
class DataPair{
private:
    int key;
    T value;
public:
    DataPair();
    DataPair(int key, T value);
    DataPair(int key);
    ~DataPair();
    int get_key();
    T get_value();
};

template<class T>
class Node{
public:
    DataPair<T> data_pair;
    vector<Node*> next;
    mutex node_barrier;
    atomic<bool> takenDeletion = false;
    atomic<bool> done_linking = false;
    int last_lvl_available;

    Node();
    Node(int key, int level);
    Node(int key, T value, int level);
    ~Node();
    int get_key();
    T get_value();
    void lockNode();
    void unlockNode();
};

template<class T>
class SkipList{
private:
    Node<T> *head;
    Node<T> *tail;
public:
    SkipList();
    SkipList(int max_elements, float probability);
    ~SkipList();
    int randomLevel();

    int find(int searchKey, vector<Node<T>*> &predecessors, vector<Node<T>*> &successors);
    bool insert(int searchKey, T value);
    string search(int searchKey);
    bool borrar(int searchKey);
    void display();
};

#define FIRST numeric_limits<int>::min()
#define LAST numeric_limits<int>::max()
static int maxLvl;

template<class T>
DataPair<T>::DataPair(){
}

template<class T>
DataPair<T>::DataPair(int k, T v){
    key = k;
    value = v;
}

template<class T>
DataPair<T>::DataPair(int k){
    key = k;
}

template<class T>
int DataPair<T>::get_key(){
    return key;
}

template<class T>
T DataPair<T>::get_value(){
    return value;
}

template<class T>
DataPair<T>::~DataPair(){
}


template<class T>
Node<T>::Node(){
}

template<class T>
Node<T>::Node(int key, int level){
    data_pair = DataPair<T>(key);
    next.resize(level + 1);
    for (size_t i = 0; i < next.size(); i++){
        next[i] = NULL;
    }
    last_lvl_available = level;
}

template<class T>
Node<T>::Node(int key, T value, int level){
    data_pair = DataPair<T>(key, value);
    next.resize(level + 1);
    for (size_t i = 0; i < next.size(); i++){
        next[i] = NULL;
    }
    last_lvl_available = level;
}

template<class T>
int Node<T>::get_key(){
    return data_pair.get_key();
}

template<class T>
T Node<T>::get_value(){
    return data_pair.get_value();
}

template<class T>
void Node<T>::lockNode(){
    node_barrier.lock();
}

template<class T>
void Node<T>::unlockNode(){
    node_barrier.unlock();
}

template<class T>
Node<T>::~Node(){
}

template<class T>
SkipList<T>::SkipList(int levels, float prob){
    maxLvl = levels;
    //cout <<"\nMAX LEVEL: "<< maxLvl<<'\n';
    head = new Node<T>(FIRST, maxLvl);
    tail = new Node<T>(LAST, maxLvl);

    for (size_t i = 0; i < head->next.size(); i++) {
        head->next[i] = tail;
    }
}

template<class T>
int SkipList<T>::find(int searchKey, vector<Node<T>*> &predecessors, vector<Node<T>*> &successors) {
    Node<T> *prevNode = head;
    int keyFound = -1;
    int level = maxLvl;
    while (level >= 0 ){
        Node<T> *currentNodeentNode = prevNode->next[level];

        while (searchKey > currentNodeentNode->get_key()){
            prevNode = currentNodeentNode;
            currentNodeentNode = prevNode->next[level];
        }

        if(keyFound == -1 && searchKey == currentNodeentNode->get_key()){
            keyFound = level;
        }

        predecessors[level] = prevNode;
        successors[level] = currentNodeentNode;
        level--;
    }
    return keyFound;
}

template<class T>
int SkipList<T>::randomLevel() {
    int v = 0;

    while(static_cast <float> (rand()) / static_cast <float> (RAND_MAX) <= 0.5){
        v++;
    }

    if (v > maxLvl)
        return maxLvl;
    else
        return v;
}

template<class T>
bool SkipList<T>::insert(int searchKey, T value) {

    int last_lvl_available = randomLevel();

    vector<Node<T>*> preds(maxLvl + 1);
    vector<Node<T>*> succs(maxLvl + 1);

    for(;;){

        int found = find(searchKey, preds, succs);

        if(found != -1){
            Node<T>* node_found = succs[found];

            if(!node_found->takenDeletion){
                while(! node_found->done_linking){
                }
                return false;
            }
            continue;
        }

        map<Node<T>*, int> locked_nodes;

        Node<T>* pred;
        Node<T>* succ;

        bool valid = true;

        for (int level = 0; valid && (level <= last_lvl_available); level++){
            pred = preds[level];
            succ = succs[level];

            if(!(locked_nodes.count( pred ))){
                pred->lockNode();
                locked_nodes.insert(make_pair(pred, 1));
            }
            valid = !(pred->takenDeletion.load(std::memory_order_seq_cst)) &&\
                    !(succ->takenDeletion.load(std::memory_order_seq_cst)) &&\
                    pred->next[level]==succ;
        }


        if(!valid){
            for (auto const& x : locked_nodes){
                x.first->unlockNode();
            }
            continue;
        }

        Node<T>* new_node = new Node<T>(searchKey, value, last_lvl_available);

        for (int level = 0; level <= last_lvl_available; level++){
            new_node->next[level] = succs[level];
        }

        for (int level = 0; level <= last_lvl_available; level++){
            preds[level]->next[level] = new_node;
        }

        new_node->done_linking = true;

        for (auto const& x : locked_nodes){
            x.first->unlockNode();
        }

        return true;
    }
}

template<class T>
string SkipList<T>::search(int searchKey){

    vector<Node<T>*> preds(maxLvl + 1);
    vector<Node<T>*> succs(maxLvl + 1);

    int found = find(searchKey, preds, succs);

    if(found == -1) {
        string s = "no se encontro elemento con searchKey " + to_string(searchKey);
        return s;
    }

    Node<T> *currentNode = head;

    for (int level = maxLvl; level >= 0; level--){
        while (currentNode->next[level] != NULL && searchKey > currentNode->next[level]->get_key()){
            currentNode = currentNode->next[level];
        }
    }

    currentNode = currentNode->next[0];

    if ((currentNode != NULL) && (currentNode->get_key() == searchKey) && succs[found]->done_linking && !succs[found]->takenDeletion){
        string s = "se encontro  " + to_string(currentNode->get_value());
        return s;
    }else {
        string s = "no se encontro elemento con searchKey " + to_string(searchKey);
        return s;
    }
}

template<class T>
bool SkipList<T>::borrar(int searchKey){
    Node<T>* elem = NULL;
    bool is_marked = false;
    int last_lvl_available = -1;

    vector<Node<T>*> preds(maxLvl + 1);
    vector<Node<T>*> succs(maxLvl + 1);

    while(true){

        int found = find(searchKey, preds, succs);

        if(found != -1){
            elem = succs[found];
        }

        if(is_marked |
           (found != -1 &&
            (elem->done_linking && elem->last_lvl_available == found && !(elem->takenDeletion))
           )
                ){
            if(!is_marked){
                last_lvl_available = elem->last_lvl_available;
                elem->lockNode();
                if(elem->takenDeletion){
                    elem->unlockNode();
                    return false;
                }
                elem->takenDeletion = true;
                is_marked = true;
            }

            map<Node<T>*, int> locked_nodes;

            Node<T>* pred;
            //Node* succ;

            bool valid = true;

            for(int level = 0; valid && (level <= last_lvl_available); level++){
                pred = preds[level];

                if(!(locked_nodes.count( pred ))){
                    pred->lockNode();
                    locked_nodes.insert(make_pair(pred, 1));
                }

                valid = !(pred->takenDeletion) && pred->next[level] == elem;
            }


            if(!valid){
                for (auto const& x : locked_nodes){
                    x.first->unlockNode();
                }
                continue;
            }

            for(int level = last_lvl_available; level >= 0; level--){
                preds[level]->next[level] = elem->next[level];
            }

            elem->unlockNode();

            for (auto const& x : locked_nodes){
                x.first->unlockNode();
            }

            return true;

        }else{
            return false;
        }
    }
}

template<class T>
void SkipList<T>::display(){
    std::cout<<"\n";
    for (int i = 0; i <= maxLvl; i++) {
        Node<T> *tempNode = head;
        //int count = 0;
        if(!(tempNode->get_key() == FIRST && tempNode->next[i]->get_key() == LAST)){
            printf("Level: %d ", i);
            cout<<"[";
            while (tempNode !=  NULL){
                if (tempNode->get_key() > FIRST && tempNode->get_key() < LAST)
                    printf("key: %d value: %d-> ", tempNode->get_key(), tempNode->get_value() );
                tempNode = tempNode->next[i];
                //count++;
            }
            cout<<"]";
            std::cout<<endl;
        }
        //if(count == 3) break;
    }
    printf("\n");
}

template<class T>
SkipList<T>::SkipList(){
}

template<class T>
SkipList<T>::~SkipList(){
}

#endif //CONcurrentNodeENT_SKIPLIST_CSKIPLIST_H
