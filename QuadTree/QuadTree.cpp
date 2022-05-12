#include <iostream>
#include <random>
#include <vector>

struct MyVec {
    float x, y;
    MyVec(float x, float y) : x(x), y(y) {};
};

class QT
{
    QT* hijos[4] = { NULL,NULL,NULL,NULL };
    int prof;
    float x0; // menor valor en x
    float y0;
    float x1; // mayor valor en x
    float y1;

    std::vector<MyVec*> data;
public:
    QT(int h, float x0, float x1, float y0, float y1) {
        this->prof = h; // profundidad
        this->x0 = x0;
        this->y0 = y0;
        this->x1 = x1;
        this->y1 = y1;
    }

    void profundizar(int h) {
        // Paso 1: Crear tus hijos
        // for(int i = 0; i < 4; ++i)
        //                                x0      x1         y0             y1
        hijos[0] = new QT(this->prof + 1, x0, (x0 + x1) / 2, (y0 + y1) / 2, y1);
        hijos[1] = new QT(this->prof + 1, (x0 + x1) / 2, x1, (y0 + y1) / 2, y1);
        hijos[2] = new QT(this->prof + 1, x0, (x0 + x1) / 2, y0, (y0 + y1) / 2);
        hijos[3] = new QT(this->prof + 1, (x0 + x1) / 2, x1, y0, (y0 + y1) / 2);
        // Paso 2: Profundizar sus hijos
        for (int i = 0; i < 4; ++i) {
            if (hijos[i]->prof == h)
                return;
            hijos[i]->profundizar(h);
        }
    }

    int getCuadrante(float x, float y) {
        if (x0 < x && x < (x0 + x1) / 2 && (y0 + y1) / 2 < y && y < y1) /// cuadrante1
            return 0;
        else if ((x0 + x1) / 2 < x && x < x1 && (y0 + y1) / 2 < y && y < y1) /// cuadrante2
            return 1;
        else if (x0 < x && x < (x0 + x1) / 2 && y0 < y && y < (y0 + y1) / 2) /// cuadrante3
            return 2;
        else                                                                /// cuadrante4                
            return 3;
    }

    void insert(float x, float y) {
        /// Es hoja
        if (hijos[0] == NULL) {
            data.push_back(new MyVec(x, y)); // va al heap
            //std::cout << data[0]->x << std::endl;
            //std::cout << data[0]->y << std::endl;
        }
        /// no es hoja
        else {
            int cuadrante = getCuadrante(x, y);
            //std::cout << cuadrante;
            hijos[cuadrante]->insert(x, y);
        }
    }
    
    void print() {
        //std::cout<<"Prof: " << prof << '\n';

        if (!hijos[0]) {
            for (auto el : data)
                std::cout << '(' << el->x << "," << el->y << ')';
        }
        else {
            hijos[0]->print();
            hijos[1]->print();
            hijos[2]->print();
            hijos[3]->print();
        }
    }

};


int main()
{
    int h;
    std::cout << "Depth: " << std::endl;
    std::cin >> h;
    float myMinx, myMaxx, myMiny, myMaxy;

    //___________INPUT DE PRUEBA

   
    MyVec arr[3] = { MyVec(2.1,1.1), MyVec(1.2,1.3), MyVec(50.4,5.2) };
    myMinx = arr[0].x;
    myMiny = arr[0].y;
    myMaxx = arr[0].x;
    myMaxy = arr[0].y;

    /// Paso 0: Calcular el x0,y0,x1,y1 
    for (auto element : arr) {
        // std::cout<<element.x <<" " <<element.y<<'\n';
        if (element.x < myMinx)
            myMinx = element.x;
        else if (element.x > myMaxx)
            myMaxx = element.x;

        if (element.y < myMiny)
            myMiny = element.y;
        else if (element.y > myMaxy)
            myMaxy = element.y;
    }
    //std::cout <<"MyMinx: " << myMinx << ' ' << "MyMaxx: " << myMaxx << ' ' << "MyMiny: " << myMiny << ' ' << "MyMaxy: " << myMaxy << '\n';


    QT* test = new QT(0, myMinx, myMaxx, myMiny, myMaxy);
    /// Paso 1: Profundizar
    test->profundizar(h);

    //Paso 2: Ingresar datos
    for (auto element : arr)
        test->insert(element.x, element.y);
    test->print();

    return 0;
}
