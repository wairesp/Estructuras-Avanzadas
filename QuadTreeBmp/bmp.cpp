#include <iostream>
#include <random>
#include <vector>

#include "bitmap.h"

#define SD 0.01

using namespace std;

struct MyVec {
    float x, y;
    MyVec(float x, float y) : x(x), y(y) {};
};

class QT
{
    QT* hijos[4] = { NULL,NULL,NULL,NULL };
    int prof;
    int minX; // menor valor en x
    int minY;
    int maxX; // mayor valor en x
    int maxY;

    PixelMatrix pixels;
public:
    QT(Bitmap image) {
        int height = image.pixels.size();
        int width = image.pixels[0].size();
        this->pixels = image.pixels; // pixels
        this->minX = 0;
        this->minY = 0;
        this->maxX = width;
        this->maxY = height;
        this->desEstandar();
    }
    
    QT(PixelMatrix pixels, float minX, float maxX, float minY, float maxY) {
        this->pixels = pixels; // pixels
        this->minX = minX;
        this->minY = minY;
        this->maxX = maxX;
        this->maxY = maxY;
    }

    void desEstandar() {
        const int height = pixels.size();

        const int width = pixels[0].size();
        
        float mean[3] = {0,0,0};
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                /// mean
                    mean[0] += pixels[i][j].red;
                    mean[1] += pixels[i][j].green;
                    mean[2] += pixels[i][j].blue;
            }
        }
        mean[0]  /= (height * width);
        mean[1]  /= (height * width);
        mean[2]  /= (height * width);
        // (x - mean)^2

        // new mean
        int standard_deviation[3]={0,0,0};
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                    standard_deviation[0] += pow(mean[0] - pixels[i][j].red,2) ;
                    standard_deviation[1] += pow(mean[1] - pixels[i][j].green,2) ;
                    standard_deviation[2] += pow(mean[2] - pixels[i][j].blue,2) ;
            }
        }
        standard_deviation[0] /= (height * width);
        standard_deviation[1] /= (height * width);
        standard_deviation[2] /= (height * width);
        
        for(int k=0;k<3;k++) {
            standard_deviation[k] = sqrt(standard_deviation[k]);
            std::cout<<standard_deviation[k]<<endl;
            if(standard_deviation[k] > SD) {
                this->dividir();
                break;
            }
        }
        
    }

    void dividir() {
        // Paso 1: Crear tus hijos
        // for(int i = 0; i < 4; ++i)
        //                                minX      maxX         minY             maxY
        // 8x8
        // 4x4
        std::vector< std::vector<Pixel>> newData;
        
        const int height = pixels.size();
        const int width = pixels[0].size();
        int newH = height / 2;
        int newW = width / 2;
        
        newData.resize(newW); // 2 fil
        for (int i = 0; i < newW; i++)
        {
            newData[i].resize(newH);
        }

        hijos[2] = new QT(newData, minX, (minX + maxX) / 2, (minY + maxY) / 2, maxY);
        hijos[3] = new QT(newData, (minX + maxX) / 2, maxX, (minY + maxY) / 2, maxY);
        hijos[0] = new QT(newData, minX, (minX + maxX) / 2, minY, (minY + maxY) / 2);
        hijos[1] = new QT(newData, (minX + maxX) / 2, maxX, minY, (minY + maxY) / 2);

        //cuadrante padre
        int Mx = (minX + maxX) / 2,
            My = (minY + maxY) / 2;
        // Paso 2: Profundizar sus hijos
        
        for(int row=0; row < height; row++)
        {
            for(int column=0; column < width; column++)
            {
                //reglas para pixeles
                if( pixels[row][column].x < Mx && pixels[row][column].y < My ) { // C1
                    // x 0-3 y 0-3
                    hijos[0]->pixels[row][column] = pixels[row][column];
                }
                else if( pixels[row][column].x >= Mx && pixels[row][column].y < My ) { // C2
                    // x 4-7 y 0-3
                    hijos[1]->pixels[row-newH][column] = pixels[row][column];
                }
                else if( pixels[row][column].x < Mx && pixels[row][column].y >= My ) { // C3
                    // x 0-3 y 4-7
                    hijos[2]->pixels[row][column-newW] = pixels[row][column];
                }
                else  { // C4
                    // x 4-7 y 4-7
                    hijos[3]->pixels[row-newH][column-newW] = pixels[row][column];
                }
            }
        }
        for (int i = 0; i < 4; ++i) {
            hijos[i]->desEstandar();
        }
    }

    int getCuadrante(float x, float y) {
        if (minX < x && x < (minX + maxX) / 2 && (minY + maxY) / 2 < y && y < maxY) /// cuadrante1
            return 0;
        else if ((minX + maxX) / 2 < x && x < maxX && (minY + maxY) / 2 < y && y < maxY) /// cuadrante2
            return 1;
        else if (minX < x && x < (minX + maxX) / 2 && minY < y && y < (minY + maxY) / 2) /// cuadrante3
            return 2;
        else                                                                /// cuadrante4
            return 3;
    }

    PixelMatrix& getPixelMatrix() {
        return pixels;
    }

    void draw(PixelMatrix &root){
        if (!hijos[0]) // !x015==0
            return;
        
        const int height = pixels.size();
        const int width = pixels[0].size();
        int newH = height / 2;
        int newW = width / 2;
        // int Mx = (hijos[k]->minX + hijos[k]->maxX) / 2,
        //     My = (hijos[k]->minY + hijos[k]->maxY) / 2;
        
        // recorrer todo el width del QT
        for (int i = 0; i < pixels[0].size(); i++)
        {
            int x = pixels[i][newW].x,
                y = pixels[i][newW].y;
            root[x][y].red = 210;
            root[x][y].green = 255;
            root[x][y].blue = 10;
        }
        // recorrer todo el heigth del QT
        for (int i = 0; i < pixels.size(); i++)
        {
            int x = pixels[newH][i].x,
                y = pixels[newH][i].y;
            root[x][y].red = 210;
            root[x][y].green = 255;
            root[x][y].blue = 10;
        }

        for (int k = 0; k < 4; k++)
        {
            hijos[k]->draw(root);
        }
    }
    void print() {
        // //std::cout<<"Prof: " << prof << '\n';

        if (!hijos[0]) {
            std::cout<<"\nCuadrante X";
            for (auto row : pixels)
                for (auto pixel : row)
                {
                    std::cout << '(' << pixel.x << "," << pixel.y << ')';
                }
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
    Bitmap image;
    vector <vector <Pixel> > bmp;

    //read a file example.bmp and convert it to a pixel matrix
    image.open("input.bmp");

    //verify that the file opened was a valid image
    bool validBmp = image.isImage();

    if( validBmp == true )
    {
        QT quad(image);

        //quad.print();
        quad.draw(quad.getPixelMatrix());

         //take all the redness out of the top-left pixel
        //  bmp = image.toPixelMatrix();
        //  rgb = bmp[2][3];
        //  rgb.red = 255;

        // //put changed image back into matrix, update the bitmap and save it
        // bmp[2][3] = rgb;
        image.fromPixelMatrix(quad.getPixelMatrix());
        // image.fromPixelMatrix(bmp);
        image.save("output.bmp");
        // image.print();
    }
    // int h;
    // std::cout << "Depth: " << std::endl;
    // std::cin >> h;
    // float myMinx, myMaxx, myMiny, myMaxy;

    // //___________INPUT DE PRUEBA


    // MyVec arr[3] = { MyVec(2.1,1.1), MyVec(1.2,1.3), MyVec(50.4,5.2) };
    // myMinx = arr[0].x;
    // myMiny = arr[0].y;
    // myMaxx = arr[0].x;
    // myMaxy = arr[0].y;

    // /// Paso 0: Calcular el minX,minY,maxX,maxY
    // for (auto element : arr) {
    //     // std::cout<<element.x <<" " <<element.y<<'\n';
    //     if (element.x < myMinx)
    //         myMinx = element.x;
    //     else if (element.x > myMaxx)
    //         myMaxx = element.x;

    //     if (element.y < myMiny)
    //         myMiny = element.y;
    //     else if (element.y > myMaxy)
    //         myMaxy = element.y;
    // }
    // //std::cout <<"MyMinx: " << myMinx << ' ' << "MyMaxx: " << myMaxx << ' ' << "MyMiny: " << myMiny << ' ' << "MyMaxy: " << myMaxy << '\n';


    // QT* test = new QT(0, myMinx, myMaxx, myMiny, myMaxy);
    // /// Paso 1: Profundizar
    // test->profundizar(h);

    // //Paso 2: Ingresar datos
    // for (auto element : arr)
    //     test->insert(element.x, element.y);
    // test->print();

    return 0;
}