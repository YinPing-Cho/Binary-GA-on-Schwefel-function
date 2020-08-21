#include "Class.h"
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <algorithm>    // std::sort
#include <fstream>

using namespace std;

void B_GA::copyArray(bool (&arr1)[10], bool (&arr2)[10]) {
    for (unsigned i = 0; i < 10; ++i) {
        arr1[i] = arr2[i];
    }
}

void B_GA::copyChromosone(bool (&arr1)[D][10], bool (&arr2)[D][10]) {
    for (unsigned i = 0; i < D; ++i) {
        copyArray(arr1[i], arr2[i]);
    }
}

template <class T>
void B_GA::swap (T *a, T *b) {  
    T temp = *a;  
    *a = *b;  
    *b = temp;  
}

void B_GA::randomize (unsigned arr[], int n) {
    for (int i = n - 1; i > 0; i--) {  
        int j = rand() % (i + 1);   
        swap(&arr[i], &arr[j]);  
    }  
}  

bool B_GA::generate() {
    int i = rand() % 2;
    if (i == 1) return true;
    else return false;
}

void B_GA::B2Int() {
    for (unsigned i = 0; i < (unsigned)((1+nP)*N); ++i) {
        for (unsigned j = 0; j < D; ++j) {
            int_pool[i][j] = 0;
            int temp;
            for (unsigned k = 0; k < 10; k++) {
                temp = pool[i][j][k];
                int_pool[i][j] |= temp << (10 - k - 1);
            }
            int_pool[i][j] -= 512;
        }
        fitness[i] = SCH(int_pool[i]);
    }
}
/*
void B_GA::TwoPCrossover2pool(bool arr1[10][10], bool arr2[10][10], unsigned pos1, unsigned pos2, unsigned index) {
    if (pos2 < pos1) {
        swap(&pos1, &pos2);
    }

    for (unsigned i = 0; i < 10; ++i) {
        if (i >= pos1 && i < pos2) {
            copyArray(pool[index][i], arr2[i]);
        }
        else copyArray(pool[index][i], arr1[i]);
    }
}
*/
void B_GA::TwoPCrossover2pool(bool (&arr_dest)[D][10], bool (&arr_s1)[D][10], bool (&arr_s2)[D][10], unsigned pos1, unsigned pos2) {
    if (pos2 < pos1) {
        swap(&pos1, &pos2);
    }

    for (unsigned i = 0; i < 10; ++i) {
        if (i >= pos1 && i < pos2) {
            copyArray(arr_dest[i], arr_s2[i]);
        }
        else copyArray(arr_dest[i], arr_s1[i]);
    }
}
/*
void B_GA::brew() {
    for (unsigned i = 0; i < N; ++i) {
        copyChromosone(pool[(int)(N*nP+i)], b_chromosone[i]);
    }
    randomize(map, 10);

    float a = 0;
    float b = 1;
    for (unsigned i = 0; i < (int)(N*nP); ++i) {
        //a += 0.1*N/(N*nP);
        //b += 0.2*N/(N*nP);
        if (i > (N*nP)/2) {a=1; b=2;}
        TwoPCrossover2pool(b_chromosone[(unsigned)a], b_chromosone[(unsigned)b], map[i%10], map[map[i%10]], i);
    }
}
*/
void B_GA::mutate() {
    for (unsigned i = 0; i < (unsigned)((1+nP)*N); ++i) {
        for (unsigned j = 0; j < D; ++j) {
            for (unsigned k = 0; k < 10; k++) {
                if (rand() % 10 > (1-M)*10-1) pool[i][j][k] = 1 - pool[i][j][k];
            }
        }
    }
}

void B_GA::brew() {
    for (unsigned i = 0; i < N; ++i) {
        copyChromosone(pool[(int)(N*nP+i)], b_chromosone[i]);
    }
    randomize(map, D);
    float a = 0;
    float b = 1;
    for (unsigned i = N; i < (int)(N*nP); ++i) {
        //a += 0.1*N/(N*nP);
        //b += 0.2*N/(N*nP);
        //if (i > (N*nP)/2) {a=1; b=2;}

        TwoPCrossover2pool(pool[i], b_chromosone[(unsigned)a], b_chromosone[(unsigned)b], map[i%10], map[map[i%10]]);
    }
}

bool ByFitness(Fitness_Alias &a, Fitness_Alias &b) {
    return a.fitness < b.fitness;
}

void B_GA::storeFittest() {
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

void B_GA::output_csv() {
    std::ofstream myfile;
    myfile.open ("BinaryGA.csv");
    for (auto it = logger.begin(); it != logger.end(); ++it) {
        myfile << *it << '\n';
    }
    myfile.close();
    cout << "Done." << '\n';
}

float B_GA::SCH (int input[D]){
    float value = 418.9829*D;
    for (unsigned i = 0; i < D; ++i) {
        value -= input[i] * sin(sqrt(abs(input[i])));
    }
    return value;
}

void B_GA::print_B0() {
    cout << "Binary 0: ";
    for (unsigned j = 0; j < D; ++j) {
        string b_num = "0x";
        for (unsigned k = 0; k < 10; ++k) {
            b_num += to_string(b_chromosone[0][j][k]);
        }
        cout << b_num << " ";
    }
    cout << '\n';
}

void B_GA::print_Int0() {
    cout << "Int 0   : ";
    for (unsigned j = 0; j < D; ++j) {
        string b_num = "0x";
        b_num += to_string(int_pool[0][j])+" ";
        cout << b_num << " ";
    }
    cout << '\n';
}

void B_GA::print_pool0() {
    cout << "Pool 0  : ";
    for (unsigned j = 0; j < D; ++j) {
        string b_num = "0x";
        for (unsigned k = 0; k < 10; ++k) {
            b_num += to_string(pool[0][j][k]);
        }
        cout << b_num << " ";
    }
    cout << '\n';
}

void B_GA::print_pool18() {
    cout << "Pool 19 : ";
    for (unsigned j = 0; j < D; ++j) {
        string b_num = "0x";
        for (unsigned k = 0; k < 10; ++k) {
            b_num += to_string(pool[18][j][k]);
        }
        cout << b_num << " ";
    }
    cout << '\n';
}

void B_GA::print_AliasBest() {
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