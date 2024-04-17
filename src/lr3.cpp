/**
 *5. Задан массив действительных чисел B(5,5). Найти минимальный элемент каждого столбца.
*/
#include "arch.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <thread>
#include <ctime>
#include <omp.h>

using namespace std;

#define FRAND_MIN   (1)
#define FRAND_MAX   (50)

#define ROW_CNT     (5)
#define COLUMN_CNT  (5)

static
void minimal_in_column(const f16 ** const B, const u8 row_cnt, const u8 index, f16 * const minimal){
    for(u8 i = 1; i < row_cnt; i++){
        const f16 value = B[i][index];
        const f16 now = *minimal;
        if(value < now){
            *minimal = value;
        }
    }
}

/**
 * find minimal column value for some matrix
*/
static
void single_thread(const f16 ** const B, const u8 row_cnt, const u8 column_cnt){
    cout << "\n\n\nsingle thread" << endl;

    /* init minimals for evry column by 1st line */
    f16* minimals = new f16[column_cnt];
    const f16* first_line = B[0];
    for(u8 j = 0; j < column_cnt; j++){
        minimals[j] = first_line[j];
    }
    
    const std::clock_t c_start = std::clock();
    
    {
        for(u8 i = 1; i < row_cnt; i++){
            for(u8 j = 0; j < column_cnt; j++){
                {
                    const f16 value = B[i][j];
                    const f16 now = minimals[j];
                    if(value < now){
                        minimals[j] = value;
                    }
                }
            }    
        }
    }
    
    const std::clock_t c_end = std::clock();
    
    cout << endl << endl << "results" << endl;
    for(u8 j = 0; j < column_cnt; j++){
        cout << minimals[j] << '\t';
    }
    cout << endl;

    delete[] minimals;
    
    
    std::cout << "single_time = " << (1000.0 * (c_end - c_start) / CLOCKS_PER_SEC) << " ms" << std::endl;
}

/**
 * find minimal column value for some matrix
*/
static
void omp_thread(const f16 ** const B, const u8 row_cnt, const u8 column_cnt){
    cout << "\n\n\nomp thread" << endl;

    /* init minimals for evry column by 1st line */
    f16* minimals = new f16[column_cnt];
    const f16* first_line = B[0];
    for(u8 j = 0; j < column_cnt; j++){
        minimals[j] = first_line[j];
    }
    
    const std::clock_t c_start = std::clock();
    
    #pragma omp parallel
    {
        #pragma omp for
        for(u8 i = 0; i < column_cnt; i++){
            minimal_in_column(B, row_cnt, i, &minimals[i]);
            // for(u8 j = 0; j < column_cnt; j++){
            //     /* critical for save access to minimals */
            //     #pragma omp critical
            //     {
            //         const f16 value = B[i][j];
            //         const f16 now = minimals[j];
            //         if(value < now){
            //             // const u16 id = omp_get_thread_num();
            //             // cout << "thd_id: " << id << " found new minimal value in: " << (u16)j << " column was: " << now << " new: " << value << endl;
            //             minimals[j] = value;
            //         }
            //     }
            // }    
        }
    }
    
    const std::clock_t c_end = std::clock();
    
    cout << endl << endl << "results" << endl;
    for(u8 j = 0; j < column_cnt; j++){
        cout << minimals[j] << '\t';
    }
    cout << endl;

    delete[] minimals;
    
    
    std::cout << "omp_time = " << (1000.0 * (c_end - c_start) / CLOCKS_PER_SEC) << " ms" << std::endl;
}

/**
 * create random float value
*/
static inline
f16 rand_f16(void){
    const f16 hi = (f16)(FRAND_MIN + rand() % FRAND_MAX);
    const f16 lo = (f16)(FRAND_MIN + rand() % FRAND_MAX);
    return hi + (lo / 10);
}

int lr3_main(void){
    cout << "======================================================================" << endl;
    cout << "\t\t\t\t\tlr3_main" << endl;
    cout << "======================================================================" << endl;
    /* init B matrix with random values and show it*/
    f16** B = new f16*[ROW_CNT];
    for(u32 i = 0; i < ROW_CNT; i++){
        B[i] = new f16[COLUMN_CNT];
        for(u32 j = 0; j < COLUMN_CNT; j++){
            const f16 value = rand_f16();
            B[i][j] = value;
            std::cout << (value) << '\t';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;

    single_thread((const f16 ** const )B, ROW_CNT, COLUMN_CNT);
    omp_thread((const f16 ** const )B, ROW_CNT, COLUMN_CNT);

    return 0;
}