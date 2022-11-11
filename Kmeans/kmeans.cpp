#include <random>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>
#include <algorithm>
#include <ratio>
#include <chrono>
#include <iomanip>

using namespace std;

//random number seed
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> distrib(1, 100);


float distEuclidiana(int dimensiones, vector<int> a, vector<int> b){
    int dist = 0;
    for (int i = 0; i < dimensiones; ++i){
        int t = a[i] - b[i];
        dist += (t * t);
    }
    return (sqrt(dist));
}


struct Punto {
    vector<int> valores;
    int dimension;
    int cluster;
    int pointID;

    Punto(int id, int dimensiones)
    {
        for (int a = 0; a <= dimensiones; ++a){
            int rand = distrib(gen);
            valores.push_back(rand);
            pointID = a;
        }
    }

    Punto(int id, int a, int b) {
        valores.push_back(a);
        valores.push_back(b);
        dimension = 2;
        cluster = NULL;
        pointID = id;
    }

    int getID(){
        return pointID;
    }

    void print_punto(){
        cout<<"{";
        for (int i = 0; i < dimension-1; ++i){
            cout<<valores[i]<<",";
        }
        cout<<valores[dimension-1]<<"}";
    }

    int getDimensions() { return dimension; }
    void setCluster(int val) { cluster = val; }
    int getCluster() { return cluster; }
};

class Cluster
{
public:
    int clusterId;
    vector<double> centroid;
    vector<Punto> puntos;
    
    Cluster(int clusterId, Punto centroid)
    {
        this->clusterId = clusterId;
        for (int i = 0; i < centroid.getDimensions(); i++)
        {
            this->centroid.push_back(centroid.valores[i]);
        }
        this->addPunto(centroid);
    }

    void addPunto(Punto p)
    {
        p.setCluster(this->clusterId);
        puntos.push_back(p);
    }

    bool removePoint(int pointId){
        int size = puntos.size();

        for(int i = 0; i < size; i++)
        {
            if(puntos[i].getID() == pointId)
            {
                puntos.erase(puntos.begin() + i);
                return true;
            }
        }
        return false;
    }

    Punto getPoint(int pos){
        return puntos[pos];
    }

    int getId() { return clusterId; }

    Punto getPunto(int pos) { return puntos[pos]; }

    int getSize() { return puntos.size(); }

    double getCentroidByPos(int pos) { return centroid[pos]; }

    void setCentroidByPos(int pos, double val) { this->centroid[pos] = val; }
};


class KMeans{
private:
    int K, iters, dimensions, total_points;
    vector<Cluster> clusters;

    int getNearestClusterId(Punto point){
        double sum = 0.0, min_dist;
        int NearestClusterId;

        for(int i = 0; i < dimensions; i++)
        {
            sum += pow(clusters[0].getCentroidByPos(i) - point.valores[i], 2.0);
        }

        min_dist = sqrt(sum);
        NearestClusterId = clusters[0].getId();

        for(int i = 1; i < K; i++)
        {
            double dist;
            sum = 0.0;

            for(int j = 0; j < dimensions; j++)
            {
                sum += pow(clusters[i].getCentroidByPos(j) - point.valores[j], 2.0);
            }

            dist = sqrt(sum);

            if(dist < min_dist)
            {
                min_dist = dist;
                NearestClusterId = clusters[i].getId();
            }
        }

        return NearestClusterId;
    }

public:
    KMeans(int K, int iterations){
        this->K = K;
        this->iters = iterations;
    }

    void run(vector<Punto>& all_points){

        total_points = all_points.size();
        dimensions = all_points[0].getDimensions();


        //Initializing Clusters
        vector<int> used_pointIds;

        for(int i=1; i<=K; i++)
        {
            while(true)
            {
                int index = rand() % total_points;

                if(find(used_pointIds.begin(), used_pointIds.end(), index) == used_pointIds.end())
                {
                    used_pointIds.push_back(index);
                    all_points[index].setCluster(i);
                    Cluster cluster(i, all_points[index]);
                    clusters.push_back(cluster);
                    break;
                }
            }
        }
        cout<<"Clusters  = "<<clusters.size()<<endl<<endl;


        cout<<"Inicio K-means.."<<endl;

        int iter = 1;
        while(true)
        {
            cout<<"Iter - "<<iter<<"/"<<iters<<endl;
            bool done = true;

            // anadir puntos al cluster segun la distancia euclidiana
            for(int i = 0; i < total_points; i++)
            {
                int currentClusterId = all_points[i].getCluster();
                int nearestClusterId = getNearestClusterId(all_points[i]);//distancia euclidiana

                if(currentClusterId != nearestClusterId)
                {
                    if(currentClusterId != 0){
                        for(int j=0; j<K; j++){
                            if(clusters[j].getId() == currentClusterId){
                                clusters[j].removePoint(all_points[i].getID());
                            }
                        }
                    }

                    for(int j=0; j<K; j++){
                        if(clusters[j].getId() == nearestClusterId){
                            clusters[j].addPunto(all_points[i]);
                        }
                    }
                    all_points[i].setCluster(nearestClusterId);
                    done = false;
                }
            }

            // Recalculating the center of each cluster
            for(int i = 0; i < K; i++)
            {
                int ClusterSize = clusters[i].getSize();

                for(int j = 0; j < dimensions; j++)
                {
                    double sum = 0.0;
                    if(ClusterSize > 0)
                    {
                        for(int p = 0; p < ClusterSize; p++)
                            sum += clusters[i].getPoint(p).valores[j];
                        clusters[i].setCentroidByPos(j, sum / ClusterSize);
                    }
                }
            }

            if(done || iter >= iters)
            {
                cout << "Clustering completed in iteration : " <<iter<<endl<<endl;
                break;
            }
            iter++;
        }


        //Print pointIds in each cluster
        for(int i=0; i<K; i++){
            cout<<"Points in cluster "<<clusters[i].getId()<<" : ";
            for(int j=0; j<clusters[i].getSize(); j++){
                cout<<clusters[i].getPoint(j).getID()<<" ";
            }
            cout<<endl<<endl;
        }
        cout<<"========================"<<endl<<endl;

        //Write cluster centers to file
        ofstream outfile;
        outfile.open("clusters.txt");
        if(outfile.is_open()){
            for(int i=0; i<K; i++){
                cout<<"Cluster "<<clusters[i].getId()<<" centroid : ";
                clusters[i].centroid
//                for(int j=0; j<dimensions; j++){
//                    cout<<clusters[i].getCentroidByPos(j)<<" ";     //Output to console
//
//                }
                cout<<endl;
                outfile<<endl;
            }
            outfile.close();
        }
        else{
            cout<<"Error: Unable to write to clusters.txt";
        }

    }
};


int main()
{
    vector<Punto> data;
    Punto a1(0,9,5);
    data.push_back(a1);
    Punto a2(1,8,4);
    data.push_back(a2);

    KMeans kmeans(2, 10);
    kmeans.run(data);
    return 0;
}