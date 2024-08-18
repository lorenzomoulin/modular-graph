#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

#define DIRECIONADO 1
#define NAO_DIRECIONADO 0

typedef vector<vector<pair<int, int>>> grafo;

grafo lista_adj;

vector<int> funcoes;

int n_arestas, n_vertices;

bool b_direcionado;

string direcionado;

void leitura_direcionado() {
    for (int i = 0; i < n_arestas; ++i) {
        int id, a, b, p;
        cin >> id >> a >> b >> p;
        lista_adj[a].push_back({b, p});
    } 
}

void leitura_nao_direcionado() {
    for (int i = 0; i < n_arestas; ++i) {
        int id, a, b, p;
        cin >> id >> a >> b >> p;
        lista_adj[a].push_back({b, p});
        lista_adj[b].push_back({a, p});
    } 
}

int main () {
    string the_string;
    getline(cin, the_string);
    istringstream iss(the_string);
    for (int funcao; iss >> funcao; )
    {
        funcoes.push_back(funcao);
    }
    
    cin >> n_vertices >> n_arestas;
    cin >> direcionado;
    lista_adj.resize(n_vertices);
    if (direcionado.compare("direcionado") == 0) {
        b_direcionado = DIRECIONADO;
        leitura_direcionado();
    }
    else {
        b_direcionado = NAO_DIRECIONADO;
        leitura_nao_direcionado();
    }

    
    return 0;
}