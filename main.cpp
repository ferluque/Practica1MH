#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "random.hpp"
#include <filesystem>

using namespace std;
using Random = effolkronium::random_static;

int n, m;
vector<vector<float>> d;
float total=0.0;
vector<float> valores;
const string sep = "=====================================================";

void read_input(string);

void print_dist();

vector<int> greedy();

float max_delta(vector<int>);

float min_delta(vector<int>);

float delta(vector<int>, int);

template<class t>
ostream &operator<<(ostream &, const vector<t>);

int main() {
    string file = "GKD-b_1_n25_m2.txt";
    string path = "..\\datos";

    ofstream salida;
    salida.open("..\\salida.csv");

    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        file = entry.path().u8string();

        salida << file << ";";

        read_input("..\\datos\\GKD-b_1_n25_m2.txt");

        int num_ejecuciones = 50;

        auto momentoInicio = std::clock();
        for (int i = 0; i < num_ejecuciones; i++)
            vector<int> solution = greedy();
        auto momentoFin = std::clock();

        // Calculo el tiempo que le ha tomado al algoritmo ejecutarse
        auto tiempo = 1000.0*(momentoFin - momentoInicio)/CLOCKS_PER_SEC;

        float media = total / num_ejecuciones;
        //cout << "Media: " << total / num_ejecuciones << endl;
        salida << media << ";";
        float suma_cuadrados = 0.0;
        for (int i = 0; i < num_ejecuciones; i++) {
            suma_cuadrados += pow(valores[i] - media, 2);
        }
        //cout << "Desviación típica: " << suma_cuadrados / num_ejecuciones << endl;
        salida << suma_cuadrados/num_ejecuciones << ";";
        salida << tiempo/num_ejecuciones << endl;
        total = 0.0;
        valores.clear();
    }
}

vector<int> greedy() {
    vector<int> solution;
    // Mete un punto aleatorio en la solución
    int primero = Random::get(0, n); // No hacer esto
    //cout << "Cojo " << primero << " como primer punto" << endl;
    solution.push_back(primero);

    // Metemos m-1 puntos mas en la solución
    for (int i = 0; i < (m - 1); i++) {
        float better_diff;
        int better_option;
        bool primero_guardado = false;
        // Escogiendo en cada paso el que minimice diff
        // Tenemos en cuenta que se salta los j que ya están en solution (count...)
        for (int j = 0; j < n; j++) {
            if (!count(solution.begin(), solution.end(), j)) {
                // Para calcular los valores del diff metiendo cada uno de los puntos j restantes
                // Creamos una posible solución y calculamos su valor de diff una vez incluido j
                vector<int> posible(solution);
                posible.push_back(j);
                float diff = max_delta(posible) - min_delta(posible);

                // Guardamos el primer valor de diff que encontremos
                if (!primero_guardado) {
                    better_diff = diff;
                    better_option = j;
                    primero_guardado = true;
                }
                    // Una vez hayamos ya guardado el primero, actualizamos cuando sea mejor
                else {
                    if (diff < better_diff) {
                        better_diff = diff;
                        better_option = j;
                    }
                }
            }
        }
        // Actualizamos la solución con el mejor valor que hayamos tenido
        solution.push_back(better_option);
        /*
        cout << "La mejor opción es añadir " << better_option << endl;
        cout << "La solución queda " << solution << endl;
        cout << "El valor de dispersión es " << max_delta(solution)-min_delta(solution) << endl;
        cout << sep << endl;*/

    }
    float diff_final = max_delta(solution) - min_delta(solution);

    total += diff_final;
    valores.push_back(diff_final);
    return solution;
}

// Calcula el mayor delta del conjunto S
// explora los deltas de todos los puntos de S y va comparando los valores de su S
float max_delta(vector<int> S) {
    float max_delta = delta(S, 0);
    for (int i = 1; i < S.size(); i++) {
        float d = delta(S, i);
        if (d > max_delta)
            max_delta = d;
    }
    return max_delta;
}

// Calcula el minimo delta de S
float min_delta(vector<int> S) {
    float min_delta = delta(S, 0);
    for (int i = 1; i < S.size(); i++) {
        float d = delta(S, i);
        if (d < min_delta)
            min_delta = d;
    }
    return min_delta;
}

// Calcula el delta de u para el conjunto S
float delta(vector<int> S, int u) {
    float delta = 0.0;
    for (int i = 0; i < S.size(); i++)
        if (i != u)
            delta += d[u][S[i]];
    return delta;
}

void read_input(string file) {
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
}

void print_dist() {
    for (int i = 0; i < d.size(); i++) {
        cout << "Fila " << i << endl;
        for (int j = 0; j < d[i].size(); j++) {
            cout << d[i][j] << " ";
        }
        cout << endl;
    }
}

template<class t>
ostream &operator<<(ostream &out, const vector<t> v) {
    out << "(";
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << ", ";
    out << ")" << endl;
    return out;
}