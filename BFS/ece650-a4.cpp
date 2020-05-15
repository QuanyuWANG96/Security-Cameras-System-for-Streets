#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <queue>
#include <algorithm>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include "minisat/mtl/IntTypes.h"
#include "minisat/mtl/Vec.h"

std::vector<int> getEdge(std::string res, int maxVertex) {
    std::string b;
    int num;
    std::vector<int> edge;

    std::string temp;
    for (int m = 0; res[m] != '\0'; m++) {
        if (res[m] == ',' or res[m] == '<' or res[m] == '>') {
            temp.append(" ");
        } else temp = temp + res[m];
    }
    std::istringstream line_s(temp);
    while (!line_s.eof()) {
        line_s >> b;

        if (std::istringstream(b) >> num) {
            edge.push_back(num);
        }
    }
    for (std::vector<int>::size_type i = 0; i < edge.size(); i++) {
        if (edge[i] >= maxVertex or edge[i] < 0) {
            std::cerr << "Error : Invalid vertice: " << edge[i] << std::endl;
            edge.clear();
            break;
        }
    }
    return edge;
}

std::vector <std::vector<int>> getAdj(const int a, const std::vector<int> edge) {
    std::vector<int>::size_type i = 0;
    std::vector <std::vector<int>> adj;
    int m = a;
    for (int j = 0; j < m; j++) {
        std::vector<int> adj_l;

        for (; i < edge.size(); i = i + 2) {
            if (edge[i] == j) {
                adj_l.push_back(edge[i + 1]);
            }
            if (edge[i + 1] == j) {
                adj_l.push_back(edge[i]);
            }
        }
        i = 0;
        adj.push_back(adj_l);
        adj_l.clear();

    }
    return adj;
}

std::vector<int> BFS(const std::vector <std::vector<int>> adj, const int s, const int v) {
    std::vector<int> path;
    std::vector<int> color(adj.size(), 0);
    std::vector<int> pi(adj.size(), adj.size());
    std::queue<int> q_path;
    std::vector<int>::size_type i = 0;
    int flag = 0;
    color[s] = 1;
    q_path.push(s);

    while (!q_path.empty()) {
        int u = q_path.front();
        q_path.pop();

        for (i = 0; i < adj[u].size(); i++) {
            if (color[adj[u][i]] == 0) {
                color[adj[u][i]] = 1;
                pi[adj[u][i]] = u;
                q_path.push(adj[u][i]);
                if (adj[u][i] == v) {
                    flag = 1;
                    break;
                }
            }
        }
        if (flag == 1) {
            break;
        }
    }
    if (s == v) {
        path.push_back(s);
        path.push_back(s);
    } else if (pi[v] == adj.size()) {
        std::cout << "Error : no path exist between the vertices" << std::endl;
    } else {
        for (std::vector<int>::size_type j = v; j != s; j = pi[j]) {
            path.push_back(j);
        }
        path.push_back(s);
    }

    return path;
}

std::vector<int> findVertexCover(int N, std::vector<int> edge) {
    std::vector<int> minVertexCover;
    std::size_t n = N;
    for (std::size_t k = 1; k <= N; k++) {
        std::unique_ptr <Minisat::Solver> solver(new Minisat::Solver());
        Minisat::Var X[n][k];

        // create new variables
        for (std::size_t i = 0; i < n; i++) {
            for (std::size_t j = 0; j < k; j++) {
                X[i][j] = solver->newVar();
            }
        }

        for (std::size_t i = 0; i < k; i++) {
            Minisat::vec <Minisat::Lit> oneVerLiterals;
            for (std::size_t j = 0; j < n; j++) {
                oneVerLiterals.push(Minisat::mkLit(X[j][i]));
            }

            // rule 1 and rule 3
            solver->addClause(oneVerLiterals);
            for (size_t z = 0; z < oneVerLiterals.size(); ++z) {
                for (size_t j = z + 1; j < oneVerLiterals.size(); ++j) {
                    solver->addClause(~oneVerLiterals[z], ~oneVerLiterals[j]);
                }
            }
        }

        for (std::size_t i = 0; i < n; i++) {
            Minisat::vec <Minisat::Lit> noTwiceVer;
            for (std::size_t j = 0; j < k; j++) {
                noTwiceVer.push(Minisat::mkLit(X[i][j]));
            }
            //rule 2
            for (size_t z = 0; z < noTwiceVer.size(); ++z) {
                for (size_t j = z + 1; j < noTwiceVer.size(); ++j) {
                    solver->addClause(~noTwiceVer[z], ~noTwiceVer[j]);
                }
            }
        }

        for (int i = 0; i < edge.size() - 1;) {
            Minisat::vec <Minisat::Lit> edgeLiterals;
            for (std::size_t j = 0; j < k; j++) {
                edgeLiterals.push(Minisat::mkLit(X[edge[i]][j]));
                edgeLiterals.push(Minisat::mkLit(X[edge[i + 1]][j]));
            }
            // rule 4
            solver->addClause(edgeLiterals);
            i = i + 2;
        }

        bool res = solver->solve();

        if (res) {
            for (std::size_t i = 0; i < n; i++) {
                for (std::size_t j = 0; j < k; j++) {
                    if (Minisat::toInt(solver->modelValue(Minisat::mkLit(X[i][j]))) == 0) {
                        minVertexCover.push_back(i);
                    }
                }
            }
            break;
        }
    }
    return minVertexCover;
}

int main(int argc, char **argv) {
    int maxVertex = -1;
    std::vector<int> edge;
    std::vector<int> minVertexCover;
    std::vector <std::vector<int>> adj;
    int s, v;
    std::vector<int> path;

    while (!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);

        if (line[0] == 'V') {
            std::istringstream line_s(line);
            int num;
            std::string b;
            while (!line_s.eof()) {
                line_s >> b;
                if (std::istringstream(b) >> num) {
                    maxVertex = num;
                }
            }
            edge.clear();
            minVertexCover.clear();
            adj.clear();
            path.clear();

        } else if (line[0] == 'E') {
            if (maxVertex <= 0) {
                std::cerr << "Error : can not find vertex" << std::endl;
                continue;
            }
            if (maxVertex > 0) {
                edge = getEdge(line, maxVertex);
                if (edge.empty()) continue;
                minVertexCover = findVertexCover(maxVertex, edge);
                std::sort(minVertexCover.begin(), minVertexCover.end());
                for (int i = 0; i < minVertexCover.size(); i++) {
                    std::cout << minVertexCover[i] << " ";
                }
                std::cout << std::endl;
            }
        }  else if (line[0] == 's') {
            if (maxVertex < 0) {
                std::cerr << "Error : can not find graph" << std::endl;
                continue;
            }
            adj = getAdj(maxVertex, edge);
            std::vector<int> m;
            std::string b;
            int num;
            std::istringstream line_s(line);
            while (!line_s.eof()) {
                line_s >> b;
                if (std::istringstream(b) >> num) {
                    m.push_back(num);
                }
            }
            s = m[0];
            v = m[1];
            if (s < maxVertex and s >= 0 and v < maxVertex and v >= 0) {
                path = BFS(adj, s, v);
                if (path.size() > 0) {
                    for (int j = path.size() - 1; j > 0; j--) {
                        std::cout << path[j] << "-";
                    }
                    std::cout << path[0] << std::endl;
                    path.clear();
                }
            } else {
                std::cerr << "Error : vertex out of bounds" << std::endl;
                continue;
            }
        }
    }
    return 0;
}