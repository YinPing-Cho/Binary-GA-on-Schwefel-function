#include "Class.h"
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <algorithm>    // std::sort
#include <fstream>

using namespace std;

void I_GA::copyChromosone(int (&arr_dest)[100], int (&arr_source)[100]) {
    for (unsigned i = 0; i < 100; ++i) {
        arr_dest[i] = arr_source[i];
    }
};

int I_GA::generate() {
    int i = rand() % 1024;
    return i - 512;
}

void I_GA::ArithmeticCrossover2pool(int arr1[100], int arr2[100], unsigned index) {
    for (unsigned i = 0; i < 100; ++i) {
        pool[index][i] = arr1[i] * L + (1-L)*arr2[i]; 
    }
}

void I_GA::brew() {
    for (unsigned i = 0; i < N; ++i) {
        copyChromosone(pool[(int)(N*nP+i)], b_chromosone[i]);
    }

    float a = 0;
    float b = 1;
    for (unsigned i = N; i < (int)(N*(1+nP)); ++i) {
        a = pointrand();
        b = pointrand();
        ArithmeticCrossover2pool(b_chromosone[(unsigned)a], b_chromosone[(unsigned)b], i);
    }
}

void I_GA::mutate() {
    for (unsigned i = 0; i < (unsigned)((1+nP)*N); ++i) {
        for (unsigned j = 0; j < 100; ++j) {
            if (rand() % 100 > (1-M)*100-1) pool[i][j] = generate();
        }
        fitness[i] = SCH(pool[i]);
    }
}

bool ByFitness(Fitness_Alias &a, Fitness_Alias &b) {
    return a.fitness < b.fitness;
}
void I_GA::storeFittest() {
    alias_best.clear();
    
    for (unsigned i = 0; i < (unsigned)((1+nP)*N); ++i) {
        Fitness_Alias pair;
        pair.fitness = fitness[i];
        pair.alias = i;
        alias_best.push_back(pair);
    }
    std::sort(alias_best.begin(), alias_best.end(), ByFitness);
    while (alias_best.size() > N) alias_best.pop_back();
    logger.push_back(min(logger.back(), alias_best[0].fitness));

    int count = 0;
    for (auto it = alias_best.begin(); it != alias_best.end(); ++it) {
        copyChromosone(b_chromosone[count], pool[(*it).alias]);
        count++;
    }
}

void I_GA::output_csv() {
    std::ofstream myfile;
    myfile.open ("BinaryGA.csv");
    for (auto it = logger.begin(); it != logger.end(); ++it) {
        myfile << *it << '\n';
    }
    myfile.close();
    cout << "Done." << '\n';
}

float I_GA::SCH (int input[100]){
    float value = 418.9829*100;
    for (unsigned i = 0; i < 100; ++i) {
        value -= input[i] * sin(sqrt(abs(input[i])));
    }
    return value;
}

void I_GA::print_Int0() {
    cout << "Int 0   : ";
    for (unsigned j = 0; j < 100; ++j) {
        string b_num = "0x";
        b_num += to_string(pool[0][j])+" ";
        cout << b_num << " ";
    }
    cout << '\n';
}

void I_GA::prpool0() {
    cout << "Pool 0  : ";
    string b_num = "0x";
    for (unsigned j = 0; j < 100; ++j) {
        b_num += "_"+to_string(pool[0][j]);
        cout << b_num << " ";
    }
    cout << '\n';
}

void I_GA::prpool18() {
    cout << "Pool 19 : ";
    string b_num = "0x";
    for (unsigned j = 0; j < 100; ++j) {
        b_num += "_"+to_string(pool[18][j]);
        cout << b_num << " ";
    }
    cout << '\n';
}

void I_GA::print_AliasBest() {
    cout << "Fitness: ";
    for (auto it = alias_best.begin(); it != alias_best.end(); ++it) {
        cout << (*it).fitness << " ";
    }
    cout << '\n';
    cout << "Alias  : ";
    for (auto it = alias_best.begin(); it != alias_best.end(); ++it) {
        cout << (*it).alias << " ";
    }
    cout << '\n';
}