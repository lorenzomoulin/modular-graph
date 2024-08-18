#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

#define DIRECIONADO 1
#define NAO_DIRECIONADO 0

typedef vector<vector<pair<int, int>>> grafo;

grafo lista_adj, grafo_associado;

vector<int> funcoes, visitado;

int n_arestas, n_vertices;

bool b_direcionado;

string direcionado;

void leitura_direcionado() {
    for (int i = 0; i < n_arestas; ++i) {
        int id, a, b, p;
        cin >> id >> a >> b >> p;
        lista_adj[a].push_back({b, p});
        grafo_associado[a].push_back({b, p});
        grafo_associado[b].push_back({a, p});
    } 
}

void leitura_nao_direcionado() {
    for (int i = 0; i < n_arestas; ++i) {
        int id, a, b, p;
        cin >> id >> a >> b >> p;
        lista_adj[a].push_back({b, p});
        lista_adj[b].push_back({a, p});
        grafo_associado[a].push_back({b, p});
        grafo_associado[b].push_back({a, p});
    } 
}

void dfs(int u) {
    visitado[u] = 1;
    for (auto &v: grafo_associado[u]) {
        if (!visitado[v.first])
            dfs(v.first);
    }
}

bool conexo() {
    dfs(0);
    for (int i = 1; i < n_vertices; ++i) {
        if (!visitado[i]) return false;
    }
    return true;
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
    grafo_associado.resize(n_vertices);
    visitado.resize(n_vertices);
    for (int i = 0; i < n_vertices; ++i) visitado[i] = 0;
    if (direcionado.compare("direcionado") == 0) {
        b_direcionado = DIRECIONADO;
        leitura_direcionado();
    }
    else {
        b_direcionado = NAO_DIRECIONADO;
        leitura_nao_direcionado();
    }

    cout << conexo() << endl;
    
    return 0;
}