#ifndef CLASS_H 
#define CLASS_H 

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#define N 1000
#define nP 9.0
#define M 0.1
#define L 0.55
using namespace std;

struct Fitness_Alias {
    float fitness;
    unsigned alias;
};

class I_GA {
    private:
        vector<int> pointlist;
        unsigned int listsize;
        unsigned int pointrand() {
            int i = rand() % listsize;
            return pointlist[i];
        }
        void copyChromosone(int (&arr_dest)[100], int (&arr_source)[100]);
        void randomize(unsigned arr[], int n); //Yates-Fischer
        int generate();
        int b_chromosone[N][100];
        int pool[(int)((1+nP)*N)][100];
        vector<Fitness_Alias> alias_best;
        vector<float> logger;
        float fitness[(int)((1+nP)*N)];

        void ArithmeticCrossover2pool(int arr1[100], int arr2[100], unsigned index);
        void brew();
        void mutate();
        void storeFittest();
        vector<double> averageLogger;
        vector<double> timeLogger;

    public:
        I_GA() {
            srand (time(NULL));
            for (unsigned int i = 0; i < N; ++i) {
                for (unsigned int j = 0; j < 100; ++j) {
                    b_chromosone[i][j] = generate();
                    
                    //cout << b_chromosone[i][j][k] << " ";
                }
            }

            for (unsigned int k = 1; k < N; ++k) {
                if ((int)(N-10*k) > 0) {
                    for (unsigned int l = 0; l < (unsigned int)(N-10*k)*(N-10*k); ++l) {
                    pointlist.push_back(k);
                    }
                }
                else pointlist.push_back(k);
            }
            listsize = pointlist.size();

            logger.push_back(INT32_MAX);
        };
        void reinitialize() {
            for (unsigned int i = 0; i < N; ++i) {
                for (unsigned int j = 0; j < 100; ++j) {
                    b_chromosone[i][j] = generate();
                    //cout << b_chromosone[i][j][k] << " ";
                }
            }
            logger.clear();
        }
        void search(unsigned times) {
            auto t1 = std::chrono::high_resolution_clock::now();
            for (unsigned i = 0; i < times; ++i) {
                brew();
                mutate();
                storeFittest();
                //inference();
            }
            auto t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
            //std::cout << "***Execution time: " << duration <<  " micro seconds." << '\n';
            //cout << "Final cost: " << logger.back() << '\n';
            averageLogger.push_back(logger.back());
            timeLogger.push_back(duration);
            //cout << "Average cost: " << averageLogger.back() << '\n';
        }
        void showOutput() {
            cout << "Average cost: " << avg1(averageLogger) << '\n';
            cout << "Average time: " << avg1(timeLogger)/1000000 << " sec." << '\n';
        }

        double avg1(std::vector<double> v) {
            auto n = v.size(); 
            float average = 0.0f;
            if ( n != 0) {
                average = accumulate( v.begin(), v.end(), 0.0) / n; 
            }
            return average;
        }

        float SCH(int input[100]);
        void inference() {
            //prpool();
            brew();
            prpool0();
            mutate();
            prpool0();
            prpool18();
            storeFittest();
            print_AliasBest();
            cout << '\n';
        }
        void print_Int0();
        void prpool0();
        void prpool18();
        void print_AliasBest();
        void output_csv();
};
  
#endif 