#include <iostream>
#include <chrono>
#include "Class.h"
using namespace std;

int main() {
    cout << "REAL VALUE GA" << '\n';
    I_GA algo;
    //algo.inference();
    unsigned times = 5;
    cout << "Average result over " << times << " runs." << '\n';
    for (unsigned i = 0; i < times; ++i) {
        algo.search(500);
        algo.reinitialize();
    }
    cout << '\n';
    algo.showOutput();
    
    return 0;
}