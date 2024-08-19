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

void dfs(int u, bool print) {
    visitado[u] = 1;
    if (print) cout << u << " ";
    for (auto &v: grafo_associado[u]) {
        if (!visitado[v.first])
            dfs(v.first, print);
    }
}


bool conexo() {
    dfs(0, false);
    for (int i = 1; i < n_vertices; ++i) {
        if (!visitado[i]) return false;
    }
    return true;
}

// para ser bipartido, o grafo deve ser colorivel com 2 cores
// ou seja, se um vertice V possui cor 1, todos os seus vizinhos devem ter cor 0
// para isso, basta fazer um BFS verificando se os vizinhos possuem cor diferente
// retorna 1 caso seja bipartido, 0 caso contrario
bool bipartido() {
    vector<int> side(n_vertices, -1);
    bool eh_bipartido = true;
    queue<int> q;
    for (int src = 0; src < n_vertices; ++src) {
        //se nao foi pintado
        if (side[src] == -1) {
            //entra na fila
            q.push(src);
            side[src] = 0;
            //BFS
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                for (auto p : lista_adj[v]) {
                    int u = p.first;
                    //se ainda nao foi pintado
                    if (side[u] == -1) {
                        //vizinho recebe cor oposta
                        side[u] = side[v] ^ 1;
                        q.push(u);
                    } else {
                        //se a cor for igual, eh_bipartido vai ser false pra sempre
                        eh_bipartido &= side[u] != side[v];
                    }
                }
            }
        }
    }
    return eh_bipartido;
}

//para grafo nao direcionado, basta checar se todos os vertices possuem grau par
// para direcionado, grau de entrada (indeg) deve ser igual ao grau de saida (outdeg)
// retorna 1 caso seja euleriano, 0 caso contrario
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

int conta_ciclo;

#define NAO_VISITADO -1
#define EXPLORADO -2
#define VISITADO -3

vector<int> vis, pai;

void dfs_cycle(int u, int chamou) { 
	vis[u] = EXPLORADO;
	for (int j = 0, v; j < (int)lista_adj[u].size(); j++) {
		v = lista_adj[u][j].first;
        if (v == chamou) continue; // para evitar loop infinito
		if (vis[v] == NAO_VISITADO) { 
			pai[v] = u; 
			dfs_cycle(v, u);
		}
		else if (vis[v] == EXPLORADO) {
			conta_ciclo++; // aresta de retorno, ou seja, ciclo
		}
	}
	vis[u] = VISITADO; 
}

//retorna verdadeiro se existe pelo menos um ciclo
bool ciclo() {
    vis.resize(n_vertices, NAO_VISITADO);
    pai.resize(n_vertices);
    for (int i = 0; i < n_vertices; ++i) {
        if (vis[i] == NAO_VISITADO) {
            dfs_cycle(i, i);
        }
    }
    return conta_ciclo > 0; 
}
// imprime as componentes conexas separadas por espaco (nao foi especificado como separar as componentes)
//ordem lexicografica
void componentes_conexas() {
    for (int i = 0; i < n_vertices; ++i) visitado[i] = 0;
    for (int i = 0; i < n_vertices; ++i) {
        if (!visitado[i]) {
            dfs(i, true);
        }
    }
    cout << endl;
} 


int main () {

    //leitura das funcoes a serem testadas
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

    //resetando o vetor de vertices visitados
    for (int i = 0; i < n_vertices; ++i) visitado[i] = 0;
    

    //controle para saber se eh direcionado ou nao
    if (direcionado.compare("direcionado") == 0) {
        b_direcionado = DIRECIONADO;
        leitura_direcionado();
    }
    else {
        b_direcionado = NAO_DIRECIONADO;
        leitura_nao_direcionado();
    }

    //ordenando os vertices vizinhos de cada vertice
    for (int i = 0; i < n_vertices; ++i) sort(lista_adj[i].begin(), lista_adj[i].end());

    for (int i = 0; i < funcoes.size(); ++i) {
        cout << "f: " <<  funcoes[i] << endl; //apagar futuramente
        //funcoes indexadas em 1, assim como no PDF
        switch (funcoes[i])
        {
            case 1:
                cout << conexo() << endl;
                break;
            case 2:
                cout << bipartido() << endl;
                break;
            case 3:
                cout << euleriano() << endl;
                break;
            case 4:
                cout << ciclo() << endl;
                break;
            case 5:
                componentes_conexas();
                break;
            default:
                break;
        }
    }
    return 0;
}