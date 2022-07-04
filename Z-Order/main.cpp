#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include <iomanip>
#include <vector>
#include <math.h>
#include <chrono>
#include <queue>



using namespace std;

#define ELEMENTOS 5000
#define DIMENSIONES 10


// Function to find the transpose of matrix
vector<vector<int> > transpose(
        vector<vector<int> > mat,
        int row, int col)
{

    // Stores the transpose
    // of matrix mat[][]
    vector<vector<int> > tr(
            col, vector<int>(row));

    // Traverse each row of the matrix
    for (int i = 0; i < row; i++) {

        // Traverse each column of the matrix
        for (int j = 0; j < col; j++) {

            // Transpose matrix elements
            tr[j][i] = mat[i][j];
        }
    }

    return tr;
}

// Function to sort the given
// matrix in row wise manner
void RowWiseSort(vector<vector<int> >& B)
{
    // Traverse the row
    for (int i = 0; i < (int)B.size(); i++) {

        // Row - Wise Sorting
        sort(B[i].begin(), B[i].end());
    }
}

// Function to print the matrix
// in column wise sorted manner
vector<vector<int>> sortCol(vector<vector<int> > &mat,
             int N, int M)
{
    // Function call to find transpose
    // of the the matrix mat[][]
    vector<vector<int> > B
            = transpose(mat, N, M);

    // Sorting the matrix row-wise
    RowWiseSort(B);

    // Calculate transpose of B[][]
    mat = transpose(B, M, N);

    return mat;
}

// Print the matrix mat[][]
void print_matriz(vector<vector<int>> matriz,int filas, int columnas){
    cout<<"\nprint: "<<endl;
    for (int i = 0; i < filas; i++){
        for (int j = 0; j < columnas; j++)
            cout << matriz[i][j] << " | ";
        cout << "\n";
    }
}


struct S {
    float dist_S;
    vector<int> punto_S;

    S(float i, vector<int> punto) {
        dist_S = i;
        punto_S = punto;
    }

    S(float i, int punto) {
        dist_S = i;
        punto_S.push_back(punto);
    }

    friend bool operator<(S const &x, S const &y)
    {
        return x.dist_S < y.dist_S;
    }
};

void printVecInt(vector<int> p) {
    cout << "|";
    for (int i: p)
        std::cout << i << '|';
    cout << endl;
}

class BPQ {
public:
    priority_queue<S> _bpq;

    BPQ() {
        this->k = 0;
        cont_elem = 0;
    }

    BPQ(int k) {
        this->k = k;
        cont_elem = 0;
    }

    int k;
    int cont_elem;

    int sizeBPQ() {
        return _bpq.size();
    }

    float topBPQ() {

        return _bpq.top().dist_S;
    }

    void printBPQ(){
        cout << "lista desde el mas lejano lejano arriba al mas cercano abajo" << endl;
        for (int i = 0; i < k-1; ++i) {

            std::cout << topBPQ() << ' ';
            printVecInt(topVecBPQ());
            cout << endl;
            deleteBPQ();
        }
        std::cout << '\n';
    }

    void printBPQ2(){
        cout << "lista desde el mas lejano lejano arriba al mas cercano abajo" << endl;
        for (int i = 0; i < k-1; ++i) {

            std::cout << topBPQ() << ' ';
            cout << endl;
            deleteBPQ();
        }
        std::cout << '\n';
    }

    void set_k(int _k) {
        this->k = _k;
    }

    bool emptyBPQ() {
        return _bpq.empty();
    }

    void insertBPQ(S data) {
        _bpq.push(data);
        cont_elem++;
        if (cont_elem > k) {
            deleteBPQ();
        }
    }

    void deleteBPQ() {
        _bpq.pop();
    }

    float topDistBPQ() {
        return _bpq.top().dist_S;
    }

    vector<int> topVecBPQ() {
        return _bpq.top().punto_S;
    }
};

float calc2Dist(vector<int> P1, vector<int> P2) {
    float sum = .0f;
    for (int i = 0; i < DIMENSIONES; ++i) {
        float diff = P1[i] - P2[i];
        float diffPow = diff * diff;
        sum += diffPow;
    }
    return (sqrt(sum)) * 1.f;
}

float calc2Dist2(int P1, int P2) {
    float sum = .0f;
    float diff = P1 - P2;
    float diffPow = diff * diff;
    sum += diffPow;

    return (sqrt(sum)) * 1.f;
}



void myKNN(int k, vector<int> i1, vector<vector<int>> mymat){
    BPQ vecinitos(k + 1);
    for (int i = 0; i < ELEMENTOS ; ++i)
    {
        vecinitos.insertBPQ(S(calc2Dist(i1, mymat[i]), mymat[i]));
    }
    //vecinitos.deleteBPQ();
    vecinitos.printBPQ();
}


void myKNN2(int k, int i1, vector<int> mymat){
    BPQ vecinitos(k + 1);
    for (int i = 0; i <mymat.size() ; ++i)
    {
        vecinitos.insertBPQ(S(calc2Dist2(i1, mymat[i]), mymat[i]));
    }
    //vecinitos.deleteBPQ();
    vecinitos.printBPQ();
}

uint32_t calcZOrder(uint16_t xPos, uint16_t yPos)
{
    static const uint32_t MASKS[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
    static const uint32_t SHIFTS[] = {1, 2, 4, 8};

    uint32_t x = xPos;  // Interleave lower 16 bits of x and y, so the bits of x
    uint32_t y = yPos;  // are in the even positions and bits from y in the odd;

    x = (x | (x << SHIFTS[3])) & MASKS[3];
    x = (x | (x << SHIFTS[2])) & MASKS[2];
    x = (x | (x << SHIFTS[1])) & MASKS[1];
    x = (x | (x << SHIFTS[0])) & MASKS[0];

    y = (y | (y << SHIFTS[3])) & MASKS[3];
    y = (y | (y << SHIFTS[2])) & MASKS[2];
    y = (y | (y << SHIFTS[1])) & MASKS[1];
    y = (y | (y << SHIFTS[0])) & MASKS[0];

    const uint32_t result = x | (y << 1);
    return result;
}

void print(int x) {
    std::cout << x << " ";
}
int zorder_x_of(int index) {
    int x = 0;
    for (int b=0,k=0; (1<<b) <= index; b+=2,k++) {
        x += ((index & (1<<b)) != 0) << k;
    }
    return x;
}

int zorder_y_of(int index) {
    return zorder_x_of(index>>1);
}

template<typename M>
void ZOrder(const M& m, int size_1, int size_2, vector<int> &myvec)
{
    for (int i=0; i<size_1*size_2; i++) {
        m[zorder_y_of(i)][zorder_x_of(i)];
        myvec.push_back(m[zorder_y_of(i)][zorder_x_of(i)]);
    }
}

int main()
{
    vector<vector<int>> my_vector(ELEMENTOS, vector<int>(DIMENSIONES));

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 10);//numeros entre 1 y 100
    std::uniform_int_distribution<> punto(0, 19999);


    for (int i = 0; i < ELEMENTOS; ++i)//5000 elementos
        for (int j = 0; j < DIMENSIONES; ++j)//10 dim
            my_vector[i][j] = dis(gen);


    myKNN(5,my_vector[3],my_vector);

    my_vector=sortCol(my_vector,ELEMENTOS,DIMENSIONES);
    //print_matriz(my_vector,ELEMENTOS,DIMENSIONES);

    vector<int> myZorder;
    vector<vector<int>> my_data_copy = my_vector;

    ZOrder(my_data_copy,ELEMENTOS,DIMENSIONES,myZorder);

    vector<int> dato;
    dato.push_back(200);

    myKNN2(5,5,myZorder);



    return 0;
}