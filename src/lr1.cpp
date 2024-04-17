/**
 * 5. Задан массив B(15,15). Найти сумму элементов четных строк.
*/
#include "arch.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <thread>
#include <ctime>
#include <omp.h>

using namespace std;

#define ROW_CNT     (15)
#define COLUMN_CNT  (15)

#define UMIN        (0)
#define UMAX        (0xff)

static
void line_sum(const u8 * const line, const u32 clumn_cnt, u32* result)
{
    u32 sum = 0;
    const u8 * src = line;
    for(u32 i = 0; i < clumn_cnt; i++, src++){
        sum += *src;
    }
    *result = sum;
}

static
void single_thread(const u8 ** const B, const u32 row_cnt, const u32 clumn_cnt)
{
    cout << "\n\n\nsingle thread" << endl;
    u32* results = new u32[row_cnt];
    for(u8 i = 0; i < row_cnt; i++){
        results[i] = 0;
    }

    const clock_t c_start = clock();
    for(u32 i = 0; i < row_cnt; i+=2){
        line_sum(B[i], clumn_cnt, &results[i]);
    }
    const clock_t c_end = clock();

    delete[] results;

    cout << "single_time = " << (1000.0 * (c_end - c_start) / CLOCKS_PER_SEC) << " ms" << endl;
}

static
void multi_thread(const u8 ** const B, const u32 row_cnt, const u32 clumn_cnt)
{
    cout << "\n\n\nmulti thread" << endl;
    u32* results = new u32[row_cnt];
    for(u8 i = 0; i < row_cnt; i++){
        results[i] = 0;
    }

    vector<thread*> thds;

    const clock_t c_start = clock();
    for(u32 i = 0; i < row_cnt; i+=2){
        thread* t = new thread(line_sum, B[i], clumn_cnt, &results[i]);
        thds.push_back(t);
    }

    /* wait threads */
    for(auto t: thds){
        if(t->joinable()){
            t->join();
        }
        delete t;
    }
    const clock_t c_end = clock();

    delete[] results;

    cout << "multi_time = " << (1000.0 * (c_end - c_start) / CLOCKS_PER_SEC) << " ms" << endl;
}

static
void omp_thread(const u8 ** const B, const u32 row_cnt, const u32 clumn_cnt){
    cout << "\n\n\nomp thread" << endl;
    u32* results = new u32[row_cnt];
    for(u8 i = 0; i < row_cnt; i++){
        results[i] = 0;
    }

    const clock_t c_start = clock();
    #pragma omp parallel
    {
        #pragma omp for
        for(u32 i = 0; i < row_cnt; i+=2){
            line_sum(B[i], clumn_cnt, &results[i]);
        }
    }

    const clock_t c_end = clock();

    delete[] results;

    cout << "omp_time = " << (1000.0 * (c_end - c_start) / CLOCKS_PER_SEC) << " ms" << endl;
}

/**
 * create random float value
*/
static inline
f16 rand_u8(void){
    const u8 result = (u8)(UMIN + rand() % UMAX);
    return result;
}

int lr1_main(void)
{
    cout << "======================================================================" << endl;
    cout << "\t\t\t\t\tlr1_main" << endl;
    cout << "======================================================================" << endl;
    /* init B matrix with random values and show it*/
    u8** B = new u8*[ROW_CNT];
    for(u32 i = 0; i < ROW_CNT; i++){
        B[i] = new u8[COLUMN_CNT];
        for(u32 j = 0; j < COLUMN_CNT; j++){
            const u8 value = rand_u8();
            B[i][j] = value;
            cout << ((u16)value) << '\t';
        }
        cout << endl;
    }

    cout << endl << endl;

    /* run in single thread */
    single_thread((const u8 ** const)B, ROW_CNT, COLUMN_CNT);

    /* run in multi thread */
    multi_thread((const u8 ** const)B, ROW_CNT, COLUMN_CNT);

    /* run in omp thread */
    omp_thread((const u8 ** const)B, ROW_CNT, COLUMN_CNT);
    return 0;
}