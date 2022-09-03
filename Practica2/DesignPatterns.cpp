#include <vector>
#include <list>
#include <iostream>

using namespace std;
template <typename Container> void Write(Container &ds, ostream &os)
{
    // typename Container::iterator iter = ds.begin();
    auto iter = ds.begin();
    for( ; iter != ds.end() ; iter++ )
        os << *iter << "\n";
    cout <<endl;
}

int main(int argc, char* argv[])
{
    list<float> list;
    vector<float> vector;
    for( int i = 0 ; i < 10 ; i++ )
    {
        list .push_back(i );
        vector.push_back(i + 50);
    }
    Write(list, cout);
    Write(vector, cout);
    return 0;
}