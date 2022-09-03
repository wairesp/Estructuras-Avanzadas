// File MyNS.h
namespace MyNS
{
    int gnCount; // Global counter
// Some small function implemented inline
    double Addition(double a, double b) // Global function
    { return a+b; }
// Some prototypes
    long factorial(int n)
    {
        if(n<0)
            return(-1); /*Wrong value*/
        if(n==0)
            return(1);  /*Terminating condition*/
        else
        {
            return(n*factorial(n-1));
        }

    }// Global function
    class CTest
    {
    public:
        CTest();
        void Method1();
    };
// More variables, functions, methods, classes, structs go here
};