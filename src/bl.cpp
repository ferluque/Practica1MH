//
// Created by fl156 on 29/03/2022.
//

#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "Solution.h"
#include "Problem.h"
#include "random.hpp"
#include "utils.h"

using namespace std;
using Random = effolkronium::random_static;

Solution bl(Problem p);

int main() {
    string path = "..\\datos";
    ofstream salida;
    salida.open("..\\resultados\\bl.csv");
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        string file = entry.path().u8string();
        salida << file << ";";

        // 5 ejecuciones
        int num_ejecuciones=5;
        vector<float> diffs(num_ejecuciones);
        auto tinicio = clock();
        for (int i=0; i<num_ejecuciones; i++) {
            Random::seed(i);
            Problem p(file);
            Solution S = bl(p);
            diffs[i] = S.get_diff();
        }
        auto tfin = clock();
        // Damos la salida de los datos como media_coste;tiempo
        salida << avg(diffs) << ";" << double(tfin-tinicio)/(num_ejecuciones*CLOCKS_PER_SEC)*1000 << ";" << endl;
    }
}

Solution bl(Problem p) {
    int contador = 0;

    Random::seed(2);
    int selected = Random::get(0, p.get_n());
    Solution s(selected, p.get_d());
    p.extract(selected);
    for (int i=1; i<p.get_m(); ++i) {
        selected = p.get_N()[Random::get(0, (int)(p.get_N().size()-1))];
        s.add(selected,p.get_d());
        p.extract(selected);
    }

    // Genero vecinos hasta encontrar el primer mejor
    // Puedo cambiar m posiciones
    // cada una con n-m elementos posibles
    for (int i=0; (i<s.get_selected().size())&&(contador < 1e5); ++i) {
        int saca = s.get_selected()[i];
//        cout << "Prueba a cambiar el " << saca << endl;
        for (int j=0; (j<p.get_N().size())&&(contador < 1e5); ++j) {
            int mete = p.get_N()[j];
//            cout << "Por el " << mete << endl;
            Solution posible = s.neighbor(saca,mete,p.get_d());
//            cout << "Posible: "<<endl;
//            cout <<posible;
//            cout << "Restantes: " << p.get_N() << endl;
            ++contador;
            if (posible.get_diff() < s.get_diff()) {
                s = posible;
                i = -1;
                j = p.get_N().size();
                p.insert(saca);
                p.extract(mete);
//                cout << s<< "Restantes: " << p.get_N() << endl << "===================="<<endl;
            }
        }
    }
    return s;
}