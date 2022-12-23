#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <chrono>
#include "source/cSkipList.h"

using namespace std;

size_t num_threads = 6;

vector<int> mydata;
vector<int> delete_list;
vector<int> search_list;

SkipList<int> skiplist(20, 0.5);



void skiplist_add(size_t start, size_t end){
    if(end >= mydata.size()) end = mydata.size();
    if(start == end) skiplist.insert(mydata[start], mydata[start]);
    for(size_t i = start; i < end; i++){
        skiplist.insert(mydata[i], mydata[i]);
    }
}

void skiplist_remove(size_t start, size_t end){
    if(end >= delete_list.size()) end = delete_list.size();
    if(start == end) skiplist.borrar(delete_list[start]);
    for(size_t i = start; i < end; i++){
        skiplist.borrar(delete_list[i]);
    }
}

void skiplist_search(size_t start, size_t end, bool printflag){
    if(end >= search_list.size()) end = search_list.size();
    if(start == end) end++;
    if(printflag == 0){
        for(size_t i = start; i < end; i++){
            string s = skiplist.search(search_list[i]);
            //if(s.empty()) s = "Not Found";
            //cout << s << endl;
        }
    }
    else
    {
        for(size_t i = start; i < end; i++){
            string s = skiplist.search(search_list[i]);
            //if(s.empty()) s = "Not Found";
            cout << s << endl;
        }
    }
}



int main(){
    int max_number;

    num_threads = 6;
    int elementos[5] = {1000,10'000,100'000,1'000'000,10'000'000};


    std::cout<<"\n***********TEST DE DATA:***************\n";

    max_number=12;

    for(int i = 1; i <= max_number; i++){
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


    vector<thread> threads;

    // insert
    int chunk_size = ceil(float(mydata.size()) / num_threads);

    for (size_t i = 0; i < mydata.size(); i = i + chunk_size) {
        threads.push_back(thread(skiplist_add, i, i + chunk_size));
    }
    for (auto &th: threads) {
        th.join();
    }

    cout << "\n(insert)Skiplist: " << endl;

    skiplist.display();
    threads.clear();

    cout << "\nelementos a buscar" << endl;

    std::chrono::time_point<std::chrono::high_resolution_clock> tstart1, end1;
    tstart1 = std::chrono::high_resolution_clock::now();

    for (auto i = search_list.begin(); i != search_list.end(); ++i)
        std::cout << *i << ' ';

    // search
    chunk_size = ceil(float(delete_list.size()) / num_threads);
    for (size_t i = 0; i < delete_list.size(); i = i + chunk_size) {
        threads.push_back(thread(skiplist_remove, i, i + chunk_size));
    }
    for (auto &th: threads) {
        th.join();
    }

    cout << "\n\n(search)Skiplist" << endl;
    threads.clear();


    cout << "\nelementos a eliminiar" << endl;


    for (auto i = delete_list.begin(); i != delete_list.end(); ++i)
        std::cout << *i << ' ';

    // delete
    chunk_size = ceil(float(delete_list.size()) / num_threads);
    for (size_t i = 0; i < delete_list.size(); i = i + chunk_size) {
        threads.push_back(thread(skiplist_remove, i, i + chunk_size));
    }
    for (auto &th: threads) {
        th.join();
    }

    cout << "\n\n(delete)Skiplist" << endl;

    skiplist.display();
    threads.clear();


    // search
    chunk_size = ceil(float(search_list.size()) / num_threads);
    for(size_t i = 0; i < search_list.size(); i = i + chunk_size){
        threads.push_back(thread(skiplist_search, i, i+chunk_size, 1));
    }
    for (auto &th : threads) {
        th.join();
    }

    threads.clear();
    mydata.clear();
    delete_list.clear();
    search_list.clear();
    SkipList<int> skiplist(20, 0.5);


    std::cout<<"\n***********TEST DE TIEMPOS:***************\n";

    for(int elem = 0 ; elem < 5; ++elem) {
        max_number=elementos[elem];


        for(int i = 1; i <= max_number; i++){
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


        vector<thread> threads;

        // insert
        int chunk_size = ceil(float(mydata.size()) / num_threads);

        std::chrono::time_point<std::chrono::high_resolution_clock> tstart, end;
        tstart = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < mydata.size(); i = i + chunk_size) {
            threads.push_back(thread(skiplist_add, i, i + chunk_size));
        }
        for (auto &th: threads) {
            th.join();
        }

        end = std::chrono::high_resolution_clock::now();
        int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - tstart).count();
        std::cout << "\nTiempo con " << mydata.size() << " operaciones de insercion " << ": " << duration << " ns."
                  << " equivalente a " \
        << duration * 0.000000001f << " s." << "\n";

        //cout << "\n(insert)Skiplist: " << endl;

        //skiplist.display();

        threads.clear();

        //cout << "\ndeletion elements" << endl;

        std::chrono::time_point<std::chrono::high_resolution_clock> tstart1, end1;
        tstart1 = std::chrono::high_resolution_clock::now();

        for (auto i = delete_list.begin(); i != delete_list.end(); ++i)
            // std::cout << *i << ' ';

            // delete
            chunk_size = ceil(float(delete_list.size()) / num_threads);
        for (size_t i = 0; i < delete_list.size(); i = i + chunk_size) {
            threads.push_back(thread(skiplist_remove, i, i + chunk_size));
        }
        for (auto &th: threads) {
            th.join();
        }

        end1 = std::chrono::high_resolution_clock::now();
        int64_t duration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - tstart1).count();
        std::cout << "Tiempo con " << delete_list.size() << " operaciones de eliminar " << ": " << duration1 << " ns."
                  << " equivalente a " \
        << duration1 * 0.000000001f << " s." << "\n";

        //cout << "\n\n(delete)Skiplist" << endl;

        // skiplist.display();

        threads.clear();

        std::chrono::time_point<std::chrono::high_resolution_clock> tstart2, end2;
        tstart2 = std::chrono::high_resolution_clock::now();

        // search
        chunk_size = ceil(float(search_list.size()) / num_threads);
        for(size_t i = 0; i < search_list.size(); i = i + chunk_size){
            threads.push_back(thread(skiplist_search, i, i+chunk_size, 0));
        }
        for (auto &th : threads) {
            th.join();
        }

        end2 = std::chrono::high_resolution_clock::now();
        int64_t duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - tstart2).count();
        std::cout << "Tiempo con " << delete_list.size() << " operaciones de busqueda " << ": " << duration2 << " ns."
                  << " equivalente a " \
        << duration2 * 0.000000001f << " s." << "\n";

        threads.clear();

        mydata.clear();
        delete_list.clear();
        search_list.clear();
        SkipList<int> skiplist(20, 0.5);
    }

    return 0;
}