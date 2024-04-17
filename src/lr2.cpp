/**
 * 5. Вычислить количество слов в строках, для каждого потока распечатать строки с большим значением.
 * Организовать решение задачи с использованием потоков: главный поток управления запускает N дочерних потоков,
 * каждый из которых имеет номер k и ищет образец в строке k – 1, k – 1 + N, k – 1 + 2N и т.д.
*/
#include "arch.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <thread>
#include <ctime>
#include <omp.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

#define N (4)

static
u32 words_in_string(const string& s)
{
    std::stringstream ss(s);
    string bogus;
    u32 cnt = 0;
    for(;ss; cnt++){
        ss >> bogus;
    }
    return cnt;
}

static
void lines_worker(const vector<string>& lines,
                  const u8 k,
                  const u8 n)
{
    /* k + i * N */
    const u32 size = lines.size();
    u32 cnt_max = 0;
    for(u32 i = 0, index = (k + i * n); index < size; i++, index = (k + i * n)){
        const string& s = lines[index];
        const u32 cnt = words_in_string(s);
        if(cnt_max < cnt){
            cnt_max = cnt;
        }
    }
    cout << "thd_id: " << (u16)k << " max_cnt: " << cnt_max << endl;
}

static
void single_thread(const vector<string>& lines)
{
    cout << "\n\n\nsingle thread" << endl;
    const clock_t c_start = clock();
    lines_worker(lines, 0, 1);
    const clock_t c_end = clock();
    cout << endl;
    cout << "single_time = " << (1000.0 * (c_end - c_start) / CLOCKS_PER_SEC) << " ms" << endl;
}

static
void multi_thread(const vector<string>& lines)
{
    cout << "\n\n\nmulti thread" << endl;
    vector<thread*> thds;
    const clock_t c_start = clock();
    
    for(u8 i = 0; i < N; i++){
        thread* t = new thread(lines_worker, lines, i, N);
        thds.push_back(t);
    }
    for(auto t: thds){
        if(t->joinable()){
            t->join();
        }
        delete t;
    }

    const clock_t c_end = clock();
    cout << endl;
    cout << "multi_time = " << (1000.0 * (c_end - c_start) / CLOCKS_PER_SEC) << " ms" << endl;
}

static
void omp_thread(const vector<string>& lines){
    cout << "\n\n\nomp thread" << endl;
    const clock_t c_start = clock();
    #pragma omp parallel
    {
        #pragma omp for
        for(u8 i = 0; i < N; i++){
            lines_worker(lines, i, N);
        }
    }
    const clock_t c_end = clock();
    cout << endl;
    cout << "omp_time = " << (1000.0 * (c_end - c_start) / CLOCKS_PER_SEC) << " ms" << endl;
}

int lr2_main(void)
{
    cout << "======================================================================" << endl;
    cout << "\t\t\t\t\tlr2_main" << endl;
    cout << "======================================================================" << endl;
    ifstream in("text.txt");

    vector<string> lines;

    for(;in;){
        string s;
        std::getline(in, s);
        lines.push_back(s);
    }

    /* run in single thread */
    single_thread(lines);

    /* run in multi thread */
    multi_thread(lines);

    /* run in omp thread */
    omp_thread(lines);
    return 0;
}