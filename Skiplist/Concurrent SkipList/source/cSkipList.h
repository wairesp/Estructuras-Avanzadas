#ifndef CONCURRENT_SKIPLIST_CSKIPLIST_H
#define CONCURRENT_SKIPLIST_CSKIPLIST_H

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

    // Stores the reference of the next node until the top level for the node
    vector<Node*> next;
    // Lock to lock the node when modifing it
    mutex node_lock;
    // Atomic variable to be marked if this Node is being deleted
    atomic<bool> marked = {false};
    // Atomic variable to indicate the Node is completely linked to predecessors and successors
    atomic<bool> fully_linked = {false};

    // The Maximum level until which the node is available
    int top_level;

    Node();
    Node(int key, int level);
    Node(int key, T value, int level);
    ~Node();
    int get_key();
    T get_value();
    void lock();
    void unlock();
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
    int get_random_level();

    int find(int key, vector<Node<T>*> &predecessors, vector<Node<T>*> &successors);
    bool add(int key, T value);
    string search(int key);
    bool remove(int key);
    void display();
};

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
    top_level = level;
}

template<class T>
Node<T>::Node(int key, T value, int level){
    data_pair = DataPair<T>(key, value);
    next.resize(level + 1);
    for (size_t i = 0; i < next.size(); i++){
        next[i] = NULL;
    }
    top_level = level;
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
void Node<T>::lock(){
    node_lock.lock();
}

template<class T>
void Node<T>::unlock(){
    node_lock.unlock();
}

template<class T>
Node<T>::~Node(){
}

#define MIN numeric_limits<int>::min()
#define MAX numeric_limits<int>::max()

static int maxLvl;

template<class T>
SkipList<T>::SkipList(int levels, float prob){
    maxLvl = levels;
    //cout <<"\nMAX LEVEL: "<< maxLvl<<'\n';
    head = new Node<T>(MIN, maxLvl);
    tail = new Node<T>(MAX, maxLvl);

    for (size_t i = 0; i < head->next.size(); i++) {
        head->next[i] = tail;
    }
}

template<class T>
int SkipList<T>::find(int key, vector<Node<T>*> &predecessors, vector<Node<T>*> &successors) {
    int found = -1;
    Node<T> *prev = head;

    for (int level = maxLvl; level >= 0; level--){
        Node<T> *curr = prev->next[level];

        while (key > curr->get_key()){
            prev = curr;
            curr = prev->next[level];
        }

        if(found == -1 && key == curr->get_key()){
            found = level;
        }

        predecessors[level] = prev;
        successors[level] = curr;
    }
    return found;
}

template<class T>
int SkipList<T>::get_random_level() {
    int l = 0;
    while(static_cast <float> (rand()) / static_cast <float> (RAND_MAX) <= 0.5){
        l++;
    }
    return l > maxLvl ? maxLvl : l;
}

template<class T>
bool SkipList<T>::add(int key, T value) {

    int top_level = get_random_level();

    vector<Node<T>*> preds(maxLvl + 1);
    vector<Node<T>*> succs(maxLvl + 1);

    for (size_t i = 0; i < preds.size(); i++){
        preds[i] = NULL;
        succs[i] = NULL;
    }


    while(true){


        int found = find(key, preds, succs);


        if(found != -1){
            Node<T>* node_found = succs[found];

            if(!node_found->marked){
                while(! node_found->fully_linked){
                }
                return false;
            }
            continue;
        }

        map<Node<T>*, int> locked_nodes;


        try{
            Node<T>* pred;
            Node<T>* succ;


            bool valid = true;

            for (int level = 0; valid && (level <= top_level); level++){
                pred = preds[level];
                succ = succs[level];


                if(!(locked_nodes.count( pred ))){
                    pred->lock();
                    locked_nodes.insert(make_pair(pred, 1));
                }

                valid = !(pred->marked.load(std::memory_order_seq_cst)) && !(succ->marked.load(std::memory_order_seq_cst)) && pred->next[level]==succ;
            }


            if(!valid){
                for (auto const& x : locked_nodes){
                    x.first->unlock();
                }
                continue;
            }

            Node<T>* new_node = new Node<T>(key, value, top_level);

            for (int level = 0; level <= top_level; level++){
                new_node->next[level] = succs[level];
            }

            for (int level = 0; level <= top_level; level++){
                preds[level]->next[level] = new_node;
            }

            new_node->fully_linked = true;

            for (auto const& x : locked_nodes){
                x.first->unlock();
            }

            return true;
        }catch(const std::exception& e){

            std::cerr << e.what() << '\n';
            for (auto const& x : locked_nodes){
                x.first->unlock();
            }
        }
    }
}

template<class T>
string SkipList<T>::search(int key){

    vector<Node<T>*> preds(maxLvl + 1);
    vector<Node<T>*> succs(maxLvl + 1);

    for (size_t i = 0; i < preds.size(); i++){
        preds[i] = NULL;
        succs[i] = NULL;
    }

    int found = find(key, preds, succs);

    if(found == -1){
        return "";
    }

    Node<T> *curr = head;

    for (int level = maxLvl; level >= 0; level--){
        while (curr->next[level] != NULL && key > curr->next[level]->get_key()){
            curr = curr->next[level];
        }
    }

    curr = curr->next[0];

    if ((curr != NULL) && (curr->get_key() == key) && succs[found]->fully_linked && !succs[found]->marked){
        return to_string(curr->get_value());
    }else {
        return "";
    }
}

template<class T>
bool SkipList<T>::remove(int key){
    // Initialization
    Node<T>* elem = NULL;
    bool is_marked = false;
    int top_level = -1;

    // Initialization of references of the predecessors and successors
    vector<Node<T>*> preds(maxLvl + 1);
    vector<Node<T>*> succs(maxLvl + 1);

    for (size_t i = 0; i < preds.size(); i++){
        preds[i] = NULL;
        succs[i] = NULL;
    }


    while(true){

        int found = find(key, preds, succs);

        if(found != -1){
            elem = succs[found];
        }

        if(is_marked |
           (found != -1 &&
            (elem->fully_linked && elem->top_level == found && !(elem->marked))
           )
                ){
            if(!is_marked){
                top_level = elem->top_level;
                elem->lock();
                if(elem->marked){
                    elem->unlock();
                    return false;
                }
                elem->marked = true;
                is_marked = true;
            }

            map<Node<T>*, int> locked_nodes;

            try{
                Node<T>* pred;
                //Node* succ;

                bool valid = true;

                for(int level = 0; valid && (level <= top_level); level++){
                    pred = preds[level];

                    if(!(locked_nodes.count( pred ))){
                        pred->lock();
                        locked_nodes.insert(make_pair(pred, 1));
                    }

                    valid = !(pred->marked) && pred->next[level] == elem;
                }


                if(!valid){
                    for (auto const& x : locked_nodes){
                        x.first->unlock();
                    }
                    continue;
                }

                for(int level = top_level; level >= 0; level--){
                    preds[level]->next[level] = elem->next[level];
                }

                elem->unlock();

                for (auto const& x : locked_nodes){
                    x.first->unlock();
                }

                return true;
            }catch(const std::exception& e){
                for (auto const& x : locked_nodes){
                    x.first->unlock();
                }
            }

        }else{
            return false;
        }
    }
}

template<class T>
void SkipList<T>::display(){
    std::cout<<"\n";
    for (int i = 0; i <= maxLvl; i++) {
        Node<T> *temp = head;
        int count = 0;
        if(!(temp->get_key() == MIN && temp->next[i]->get_key() == MAX)){
            printf("Level %d ", i);
            cout<<"[";
            while (temp != NULL){
                printf("key: %d value: %d-> ", temp->get_key(), temp->get_value() );
                temp = temp->next[i];
                count++;
            }
            cout<<"]";
            std::cout<<endl;
        }
        if(count == 3) break;
    }
    printf("\n");
}

template<class T>
SkipList<T>::SkipList(){
}

template<class T>
SkipList<T>::~SkipList(){
}

#endif //CONCURRENT_SKIPLIST_CSKIPLIST_H
