#ifndef CLASS_H 
#define CLASS_H 

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <vector>
#include <chrono>
#include <numeric> 
#define N 100 //number of parents
#define nP 9.0 //maximum size of populatioin
#define M 0.1 //mutation rate
#define D 10 //dimension of Schwefel function
using namespace std;

struct Fitness_Alias {
    float fitness;
    unsigned alias;
};

class B_GA {
    private:
        void copyArray(bool (&arr1)[10], bool (&arr2)[10]);
        void copyChromosone(bool (&arr1)[D][10], bool (&arr2)[D][10]);
        //unsigned times = 0;

        template <class T>
        void swap(T *a, T *b);
        void randomize(unsigned arr[], int n); //Yates-Fischer
        bool generate();
        bool b_chromosone[N][D][10];
        int int_pool[(int)((1+nP)*N)][D];
        bool pool[(int)((1+nP)*N)][D][10];
        vector<Fitness_Alias> alias_best;
        vector<float> logger;
        float fitness[(int)((1+nP)*N)];
        void B2Int();

        //void TwoPCrossover2pool(bool arr1[10][10], bool arr2[10][10], unsigned pos1, unsigned pos2, unsigned index);
        void TwoPCrossover2pool(bool (&arr_dest)[D][10], bool (&arr_s1)[D][10], bool (&arr_s2)[D][10], unsigned pos1, unsigned pos2);
        void brew();
        void mutate();
        void storeFittest();
        unsigned map[D];
        vector<double> averageLogger;
        vector<double> timeLogger;

    public:
        B_GA() {
            srand (time(NULL));
            for (unsigned int i = 0; i < N; ++i) {
                for (unsigned int j = 0; j < D; ++j) {
                    for (unsigned int k = 0; k < 10; ++k) {
                        b_chromosone[i][j][k] = generate();
                        //cout << b_chromosone[i][j][k] << " ";
                    }
                }
            }
            for (unsigned i = 0; i < D; ++i) {
                map[i] = i;
            }
            logger.push_back(INT32_MAX);
        };
        void reinitialize() {
            for (unsigned int i = 0; i < N; ++i) {
                for (unsigned int j = 0; j < D; ++j) {
                    for (unsigned int k = 0; k < 10; ++k) {
                        b_chromosone[i][j][k] = generate();
                        //cout << b_chromosone[i][j][k] << " ";
                    }
                }
            }
        }
        void search(unsigned times) {
            auto t1 = std::chrono::high_resolution_clock::now();
            for (unsigned i = 0; i < times; ++i) {
                brew();
                mutate();
                B2Int();
                storeFittest();
                //inference();
            }
            auto t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
            //std::cout << "***Execution time: " << duration <<  " micro seconds." << '\n';
            //cout << "Final cost: " << logger.back() << '\n';
            //times++;
            //cout << "Run " << times << " cplt. ";
            averageLogger.push_back(logger.back());
            timeLogger.push_back(duration);
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

        float SCH(int input[10]);
        void inference() {
            print_B0();
            //print_pool();
            brew();
            print_pool0();
            mutate();
            B2Int();
            print_Int0();
            print_pool0();
            print_pool18();
            storeFittest();
            print_B0();
            print_AliasBest();
            cout << '\n';
        }
        void print_B0();
        void print_Int0();
        void print_pool0();
        void print_pool18();
        void print_AliasBest();
        void output_csv();
};
  
#endif 