//
// Created by fl156 on 29/03/2022.
//

#include "Solution.h"
#include "Problem.h"
#include "random.hpp"
#include "utils.h"
#include <iostream>
#include <filesystem>

using namespace std;
using Random = effolkronium::random_static;

Solution greedy(Problem p);

int main () {

    string path = "..\\datos";
    ofstream salida;
    salida.open("..\\resultados\\greedy.csv");
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        string file = entry.path().u8string();
        salida << file << ";";

        // 5 ejecuciones
        int num_ejecuciones=10;
        vector<float> diffs(num_ejecuciones);
        Random::seed(10);
        auto tinicio = clock();
        for (int i=0; i<num_ejecuciones; i++) {
            Problem p(file);
            Solution S = greedy(p);
            diffs[i] = S.get_diff();
        }
        auto tfin = clock();
        // Damos la salida de los datos como media_coste;tiempo
        salida << avg(diffs) << ";" << double(tfin-tinicio)/(num_ejecuciones*CLOCKS_PER_SEC)*1000 << ";" << endl;
    }
/*
    Random::seed(1);
    Problem p("..\\datos\\GKD-b_6_n25_m7.txt");
    Solution S(greedy(p));
    cout << S;
    S.print_dist("..\\resultados\\matrizresultante.csv", p.get_d());
*/
}

Solution greedy(Problem p) {
    int primero = Random::get(0,p.get_n()-1);

    Solution S(primero, p.get_d());
    p.extract(primero);

    // Algoritmo greedy
    while (S.get_size() < p.get_m()) {
        vector<int> restantes = p.get_N();
        float better_diff = S.get_new_diff(restantes[0], p.get_d());
        int better_option = restantes[0];
        for (int i=1; i<restantes.size(); ++i){
            float diff_actual = S.get_new_diff(restantes[i], p.get_d());
            if (diff_actual<better_diff) {
                better_diff = diff_actual;
                better_option = restantes[i];
            }
        }
        S.add(better_option, p.get_d());
        p.extract(better_option);
    }
    // cout << S;
    return S;
}
