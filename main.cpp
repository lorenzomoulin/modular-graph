#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>

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
        // cout << id<< " " << a << " "<< b  << " " << p << endl;
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

bool bipartido() {
    vector<int> side(n_vertices, -1);
    bool eh_bipartido = true;
    queue<int> q;
    for (int src = 0; src < n_vertices; ++src) {
        if (side[src] == -1) {
            q.push(src);
            side[src] = 0;
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                for (auto p : lista_adj[v]) {
                    int u = p.first;
                    if (side[u] == -1) {
                        side[u] = side[v] ^ 1;
                        q.push(u);
                    } else {
                        eh_bipartido &= side[u] != side[v];
                    }
                }
            }
        }
    }
    return eh_bipartido;
}

bool euleriano() {
    if (!conexo()) return false;
    if (!b_direcionado) {
        for (int i = 0; i < n_vertices; ++i) {
            if (lista_adj[i].size() % 2 != 0) return false;
        }
        return true;
    }
    vector<int> indeg(n_vertices, 0), outdeg(n_vertices, 0);
    
    for (int i = 0; i < n_vertices; ++i) {
        
        outdeg[i] = lista_adj[i].size();
        for (int j = 0; j < lista_adj[i].size(); ++j) {
            int v = lista_adj[i][j].first;
            indeg[v]++;
        }
        
    }
    for (int i = 0; i < n_vertices; ++i) {
        if (indeg[i] != outdeg[i]) return false;
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
    sort(funcoes.begin(), funcoes.end());

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

    // cout << "Direcionado: " << b_direcionado << endl;

    //ordenando os vertices vizinhos de cada vertice
    for (int i = 0; i < n_vertices; ++i) sort(lista_adj[i].begin(), lista_adj[i].end());
    // cout << conexo() << endl;
    for (int i = 0; i < funcoes.size(); ++i) {
        cout << "f: " <<  funcoes[i] << endl;
        switch (funcoes[i])
        {
            case 0:
                cout << conexo() << endl;
                break;
            case 1:
                cout << bipartido() << endl;
                break;
            case 2:
                cout << euleriano() << endl;
                break;
            default:
                break;
        }
    }
    return 0;
}