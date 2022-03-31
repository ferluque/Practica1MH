//
// Created by fl156 on 29/03/2022.
//

#ifndef MDD_SOLUTION_H
#define MDD_SOLUTION_H

#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class Solution {
private:
    vector<int> selected;
    float diff;
    float max_delta;
    float min_delta;
    vector<float> deltas;

    void check_max_min(float new_delta) {
        if (new_delta > max_delta)
            max_delta = new_delta;
        if ((new_delta < min_delta)||(selected.size()==2))
            min_delta = new_delta;
    }

    /**
     * Actualiza el valor de diff con respecto al último valor de selected
     * @param d La matriz de distancias
     */
    void update_diff(const vector<vector<float>>& d){
        // Actualizamos los valores de delta anteriores para todos con el nuevo punto
        int nuevo = selected[selected.size()-1];

        deltas[0] += d[selected[0]][nuevo];
        max_delta = min_delta = deltas[0];
        for (int i=1; i<deltas.size(); i++) {
            deltas[i] += d[selected[i]][nuevo];
            check_max_min(deltas[i]);
        }

        // Añado el delta del punto nuevo
        deltas.push_back(delta(nuevo, d));
        check_max_min(deltas[deltas.size()-1]);

        diff = max_delta-min_delta;
    };

    /**
     * Calcula la dispersión de un punto u en el conjunto de seleccionados
     * @param u El punto
     * @param d La matriz de distancias
     * @return El valor de delta
     */
    float delta(int u, const vector<vector<float>>& d) {
        float de = 0.0;
        for (int i : selected)
            de+=d[i][u];
        return de;
    };
public:
    /**
     * Constructor que inicializa la solución con un punto inicial
     * @param p0 El punto inicial
     * @param d La matriz de distancias
     */
    Solution(int p0, const vector<vector<float>>& d) {
        selected.push_back(p0);
        min_delta = max_delta = 0.0;
        diff = 0.0;
        deltas.push_back(0.0);
    };
    /**
     * Constructor que inicializa la solución al vector de seleccionados que le pasamos (función utilizada en la bl)
     * @param s El vector de seleccionados
     * @param d La matriz de distancias
     */
    Solution(vector<int> s, const vector<vector<float>> d) {

    }
    /**
     * Constructor de copia
     * @param c
     */
    Solution(const Solution& c) {
        this->selected = c.selected;
        this->max_delta = c.max_delta;
        this->min_delta = c.min_delta;
        this->diff = c.diff;
        this->deltas = c.deltas;
    };
    /**
     * Devuelve el nuevo valor de la función objetivo (factorizada) tras la inclusión de un nuevo punto
     * @param u El punto nuevo
     * @param d La matriz de distancias
     * @return El valor de diff para selected union u
     */
    float get_new_diff(int u, const vector<vector<float>>& d) {
        Solution posible(*this);
        posible.add(u,d);
        return posible.get_diff();

    };
    /**
     * Añade un punto a la solución y actualiza el valor de diff
     * @param u
     * @param d
     */
    void add(int u, const vector<vector<float>>& d) {
        selected.push_back(u);
        update_diff(d);
    };
    float get_diff() const {return diff;};

    int get_size() const {return (int)selected.size();};

    const vector<int>& get_selected() {return selected;};

    void print_dist(string file_out, const vector<vector<float>>& d) {
        ofstream out;
        out.open(file_out);
        out << ";";
        for (int i: selected)
            out << i << ";";
        out << endl;
        for (int i: selected) {
            out << i << ";";
            for (int j: selected) {
                out << d[i][j] << ";";
            }
            out << endl;
        }
    }
};

ostream& operator<<(ostream& out, Solution s) {
    out << "Escogidos: (";
    vector<int> selected = s.get_selected();
    for (int i : selected) {
        out << i << ", ";
    }
    out << ")" << endl;
    out << "Valor de diff: " << s.get_diff() << endl;
    return out;
}

#endif //MDD_SOLUTION_H
