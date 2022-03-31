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

    /**
     * Intercambia el vecino i por j y modifica valores correspondientes
     * @param i
     * @param j
     * @param d
     * @return Devuelve la solución con los puntos cambiados
     */
    Solution exchange(int i, int j, vector<vector<float>> d) {
        // EN primer lugar busco la posición de i en selected ( y en delta )
        int posi = -1;
        for (int k=0; (k<selected.size()) && (posi ==-1); k++)
            if (selected[k] == i)
                posi = k;

        // Saco i de selected y de delta
        selected.erase(selected.begin()+posi);
        deltas.erase(deltas.begin()+posi);

        // Meto j en selected y en delta
        selected.push_back(j);
        deltas.push_back(0.0);

        // Actualizo los valores de min_delta y max_delta
        // Para ello tengo que restar d[i][selected[k]] y sumar d[j][selected[k]] a los deltas
        deltas[0] = min_delta = max_delta = deltas[0] - d[i][selected[0]] + d[j][selected[0]];
        for (int k=1; k<deltas.size()-1; k++) {
            deltas[k] = deltas[k] - d[i][selected[k]] + d[j][selected[k]];
            check_max_min(deltas[k]);
        }
        deltas[deltas.size()-1] = delta(j, d);
        check_max_min(deltas[deltas.size()-1]);
        diff = max_delta - min_delta;
        return *this;
    }
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
        selected = s;
        deltas.resize(s.size());
        deltas[0] = max_delta = min_delta = delta(s[0], d);
        for (int i=1; i<selected.size(); i++) {
            deltas[i] = delta(selected[i], d);
            check_max_min(deltas[i]);
        }
        diff = max_delta - min_delta;
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
    /**
     * Mecanismo de generación de vecinos. Intercambia el elemento i por el j y devuelve la solución con el respectivo
     * valor de diff, max y min delta y deltas
     * @param i El elemento que saca
     * @param j El elemento que mete
     * @return El vecino
     */
    Solution neighbor(int i, int j, vector<vector<float>> d) {
        Solution vecino(*this);
        return vecino.exchange(i,j,d);
    }


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

template <class T>
ostream& operator<<(ostream& out, vector<T> v) {
    out << "(";
    for (auto it=v.begin(); it!=v.end();++it)
        out << *it << ",";
    out <<")" << endl;
    return out;
}

#endif //MDD_SOLUTION_H
