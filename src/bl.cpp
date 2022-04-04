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
Solution bl_random(Problem p);

int main() {
    /*
    string path = "..\\datos";
    ofstream salida;
    salida.open("..\\resultados\\bl.csv");
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        string file = entry.path().u8string();
        salida << file << ";";

        // 5 ejecuciones
        int num_ejecuciones=5;
        vector<float> diffs(num_ejecuciones);
        Random::seed(10);
        auto tinicio = clock();
        for (int i=0; i<num_ejecuciones; i++) {
            Problem p(file);
            Solution S = bl_random(p);
            diffs[i] = S.get_diff();
        }
        auto tfin = clock();
        // Damos la salida de los datos como media_coste;tiempo
        salida << avg(diffs) << ";" << double(tfin-tinicio)/(num_ejecuciones*CLOCKS_PER_SEC)*1000 << ";" << endl;
    }*/

    Problem p("..\\datos\\GKD-b_6_n25_m7.txt");
    cout << bl(p) << endl;
}

Solution bl(Problem p) {
    int contador = 0;

    int selected = Random::get(0, p.get_n()-1);
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

    // MODIFICACIÓN: Ahora estos vecinos no se exploran directamente sino que se almacenan en un vector
    // se baraja y se van explorando en el orden que nos de el aleatorio
    while (contador < 1e5) {
        // También almacenamos el cambio que supone la generación de ese vecino
        vector<pair<int,int>> vecinos;
        for (int i=0; (i<s.get_selected().size())&&(contador < 1e5); ++i) {
            int saca = s.get_selected()[i];
            for (int j=0; (j<p.get_N().size())&&(contador < 1e5); ++j) {
                int mete = p.get_N()[j];
                vecinos.push_back(pair<int,int>(saca, mete));
            }
        }
        // Una vez genere todos los vecinos los barajamos y los vamos sacando
        Random::shuffle(vecinos);
        for (auto it = vecinos.begin(); it!=vecinos.end(); ++it) {
            ++contador;
            Solution v = s.neighbor((*it).first, (*it).second, p.get_d());
            if (v.get_diff() < s.get_diff()) {
                s = v;
                p.insert((*it).first);
                p.extract((*it).second);
                break;
            }
        }
    }

    return s;
}

Solution bl_random(Problem p) {
    int contador = 0;

    int selected = Random::get(0, p.get_n()-1);
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
        int saca = s.get_selected()[Random::get(0,(int)s.get_selected().size()-1)];
        for (int j=0; (j<p.get_N().size())&&(contador < 1e5); ++j) {
            // Coge aleatoriamente uno de los que no están añadidos
            int mete = p.get_N()[(int)(Random::get(0,(int)p.get_N().size()-1))];
            Solution posible = s.neighbor(saca,mete,p.get_d());
            ++contador;
            if (posible.get_diff() < s.get_diff()) {
                s = posible;
                i = -1;
                j = p.get_N().size();
                p.insert(saca);
                p.extract(mete);
            }
        }
    }
    return s;
}