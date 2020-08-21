#include <iostream>
#include <chrono>
#include "Class.h"
using namespace std;

int main() {
    cout << "BINARY GA" << '\n';
    B_GA algo;
    //algo.inference();
    unsigned times = 5;
    unsigned generations = 500;
    cout << "Average result over " << times << " runs." << '\n';
    cout << "Iterating for " << generations << " generations per run." << '\n';
    for (unsigned i = 0; i < times; ++i) {
        algo.search(generations);
        algo.reinitialize();
    }
    cout << '\n';
    algo.showOutput();
    
    return 0;
}