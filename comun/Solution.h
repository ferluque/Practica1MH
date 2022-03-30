//
// Created by fl156 on 29/03/2022.
//

#ifndef MDD_SOLUTION_H
#define MDD_SOLUTION_H

#include <vector>
using namespace std;

class Solution {
private:
    vector<int> selected;
    float diff;
    float max_delta;
    float min_delta;

    /**
     * Actualiza el valor de diff con respecto al último valor de selected
     * @param d La matriz de distancias
     */
    void update_diff(const vector<vector<float>>& d){
        float delta = this->delta(selected[selected.size()-1],d);
        if (delta>max_delta)
            max_delta = delta;
        if (delta<min_delta)
            min_delta = delta;
        diff = max_delta-min_delta;
    };

    /**
     * Calcula la dispersión de un punto u en el conjunto de seleccionados
     * @param u El punto
     * @param d La matriz de distancias
     * @return El valor de delta
     */
    const float delta(int u, const vector<vector<float>>& d) {
        float de = 0.0;
        for (auto it=selected.cbegin(); it!=selected.cend(); ++it)
            de+=d[*it][u];
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
        min_delta = delta(p0, d);
        max_delta = min_delta;
        diff = 0.0;
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
    Solution(Solution c) {
        this->selected = c.selected;
        this->max_delta = c.max_delta;
        this->min_delta = c.min_delta;
        this->diff = c.diff;
    };
    /**
     * Devuelve el nuevo valor de la función objetivo (factorizada) tras la inclusión de un nuevo punto
     * @param u El punto nuevo
     * @param d La matriz de distancias
     * @return El valor de diff para selected union u
     */
    float get_new_diff(int u, const vector<vector<float>>& d) {
        Solution posible(*this);
        posible.add(u, d);
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
    const float get_diff() {return diff;};
};

#endif //MDD_SOLUTION_H
