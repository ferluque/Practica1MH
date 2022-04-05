//
// Created by Fernando on 30/03/2022.
//

#ifndef MDD_PROBLEM_H
#define MDD_PROBLEM_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

class Problem {
private:
    int n,m;
    vector<std::vector<float>> d;
    vector<int> N;

public:
    /**
     * Constructor que inicializa los valores de n,m y d a razón de lo que haya en el fichero de parámetro
     * @param file El fichero del que extraerlos
     */
    Problem(string file) {
        ifstream input;
        input.open(file);
        if (!input.is_open()) {
            cerr << "Error abriendo el archivo" << endl;
            exit(-1);
        }
        input >> n;
        input >> m;

        d.resize(n);
        for (int i = 0; i < n; i++)
            d[i].resize(n);

        // Leo los datos de la matriz superior
        for (int i = 0; i < (n - 1); i++) {
            for (int j = i + 1; j < n; j++) {
                int i_i, j_i;
                float d_i;
                input >> i_i >> j_i >> d_i;
                d[i][j] = d_i;
                input.ignore();
            }
        }

        // Copio por debajo de la diagonal principal los mismos datos (matriz simétrica)
        // Inicializo con 0s la matriz inferior (i>j)
        for (int i = 0; i < n; i++)
            for (int j = i; j >= 0; j--)
                d[i][j] = d[j][i];
        input.close();

        N.resize(n);
        for (int i=0; i<n; i++)
            N[i] = i;
    };

    /**
     * Extrae un punto de la lista de restantes
     * @param u El punto a extraer
     */
    void extract(int u) {
        N.erase(find(N.begin(), N.end(), u));
    };

    void insert(int u) {
        N.push_back(u);
    }

    int get_n() const {return n;};
    int get_m() const {return m;};
    const vector<int>& get_N() const {return N;};
    const vector<vector<float>>& get_d() const {return d;};
};

#endif //MDD_PROBLEM_H
