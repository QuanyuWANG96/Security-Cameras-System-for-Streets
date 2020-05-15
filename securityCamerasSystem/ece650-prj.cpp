#include "ece650.h"


int getVertexNum(std::string line) {
    std::istringstream line_s(line);
    int num;
    std::string b;
    int v;
    while (!line_s.eof()) {
        line_s >> b;
        if (std::istringstream(b) >> num) {
            v = num;
        }
    };
    return v;
}

vector <vector<int >> Adj(vector<int> edge, int V) {
    vector <vector<int>> adj(V);
    for (int i = 0; i < edge.size(); i = i + 2) {
        adj[edge[i]].push_back(edge[i + 1]);
        adj[edge[i + 1]].push_back(edge[i]);
    }
    return (adj);
}

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

Minisat::lbool RESULT::SAT(int kValue, vector<int> &result) {
    this->vertexCover.clear();
    this->solver.reset(new Minisat::Solver());
    vector<vector<Minisat::Lit>> lits(this->g->maxVertex);
    Minisat::vec<Minisat::Lit> empty;

    // init
    for (int i = 0; i < this->g->maxVertex; i++) {
        for (int j = 0; j < kValue; j++) {
            Minisat::Lit lit;
            lit = Minisat::mkLit(this->solver->newVar());
            lits[i].push_back(lit);
        }
    }

    // 1. At least one vertex is the ith vertex in the vertex cover.
    for (int i = 0; i < kValue; i++) {
        Minisat::vec<Minisat::Lit> snap(this->g->maxVertex);
        for (int j = 0; j < this->g->maxVertex; j++) {
            snap[j] = lits[j][i];
        }
        this->solver->addClause(snap);
    }

    // 2. No one vertex can appear twice in a vertex cover.
    if (this->g->maxVertex > 1) {
        for (int i = 0; i < kValue; i++) {
            for (int j = 0; j < this->g->maxVertex- 1; j++) {
                for (int k = j + 1; k < this->g->maxVertex; k++) {
                    this->solver->addClause(~lits[j][i], ~lits[k][i]);
                }
            }
        }
    }

    // 3. No more than one vertex appears in the mth position of the vertex cover.
    if (kValue > 1) {
        for (int i = 0; i < this->g->maxVertex; i++) {
            for (int j = 0; j < kValue - 1; j++) {
                for (int k = j + 1; k < kValue; k++) {
                    this->solver->addClause(~lits[i][j], ~lits[i][k]);
                }
            }
        }
    }

    // 4. Every edge is incident to at least one vertex in the vertex cover.
    std::vector<int> edge = this->g->edge;
    for (int i = 0; i < edge.size() - 1;) {
        Minisat::vec <Minisat::Lit> edgeLiterals;
        for (std::size_t j = 0; j < kValue; j++) {
            edgeLiterals.push(lits[edge[i]][j]);
            edgeLiterals.push(lits[edge[i + 1]][j]);
        }
        // rule 4
        this->solver->addClause(edgeLiterals);
        i = i + 2;
    }

    // solver
    // XXX use solveLimited(). Handle ALL 3 possible results: l_True, l_False, and l_Undef
    Minisat::lbool rSolver = this->solver->solveLimited(empty);

    if (rSolver==Minisat::l_True) {
//        result.resize(kValue);
        for (int i = 0; i <this->g->maxVertex ; i++) {
            for (int j = 0; j < kValue; j++) {
                if (Minisat::toInt(this->solver->modelValue(lits[i][j])) == 0) {
                    this->vertexCover.push_back(i);
//                        std::cout << i << "  ";
                }
            }
        }
        return Minisat::l_True;
    } else {
        return rSolver;
    }
}

vector<int> RESULT::getResultCnfSat() {
    vector<int> res;
    for (int i = 1; i <= this->g->maxVertex; i++) {
        Minisat::lbool rSolver = this->SAT(i, res);
        if (rSolver==Minisat::l_True) {
//            cout<< "TRUE%%%"<<endl;
            break;
        }
        if (rSolver==Minisat::l_Undef){
//            cout<< "UNDEFINE%%%"<<endl;
            break;
        }
//        else {cout<< "FALSE%%%"<<endl;}
    }
    return res;
}

vector<int> APPROX_VC_1(int V, vector <vector<int>> adj) {
    vector<int> mincover;
    vector <vector<int >> adj_new;
    int next;
    int max_degree = -1;
    while (max_degree != 0) {
        max_degree = adj[0].size();
        next = 0;
        for (int i = 0; i < adj.size(); i++) {
            if (adj[i].size() > max_degree) {
                max_degree = adj[i].size();
                next = i;
            }
        }
        for (int i = 0; i < adj[next].size(); i++) {
            int x = adj[next][i];
            for (int j = 0; j < adj[x].size(); j++) {
                if (adj[x][j] == next) {
                    swap(adj[x][j], adj[x][adj[x].size() - 1]);
                    adj[x].pop_back();
                    break;
                }
            }
        }
        adj[next].clear();
        mincover.push_back(next);
    }
    mincover.pop_back();
    return mincover;
}

std::vector<int> APPROX_VC_2(int maxVertex, std::vector<int> edge) {
    std::vector<int> vertexCover;
    std::vector<int> visit(maxVertex, 0);
    int count = 0;
    if (edge.size() == 0) return vertexCover;
    while (count < edge.size() / 2) {
        for (int i = 0; i < edge.size(); i = i + 2) {
            count = count + 1;
            if (visit[edge[i]] == 1 || visit[edge[i + 1]] == 1) {
                continue;
            }
            vertexCover.push_back(edge[i]);
            vertexCover.push_back(edge[i + 1]);
            visit[edge[i]] = 1;
            visit[edge[i + 1]] = 1;
        }
    }
    return vertexCover;
}

void output(vector<int> vertexCover, string type) {
    if (!vertexCover.empty()) {
        std::cout << type << ": ";
        sort(vertexCover.begin(), vertexCover.end());
        for (int i = 0; i < vertexCover.size(); i++) {
            std::cout << vertexCover[i] << " ";
        }
        std::cout << std::endl;
    } else std::cout << "CNF-SAT-VC: time out"<<std::endl;
    return;
}

//void outRunTime(RESULT *res){
//    for(int i = 0; i<3; i++){
//        string file = "runTime/" + res[i].methodType + ".txt";
//        ofstream filestream(file,ios::app);
//        if (filestream.is_open())
//        {
//            filestream << res[i].g->maxVertex << "  " << res[i].runTime<<"  " << res[i].vertexCover.size()<< "\n";
//        }
//        else cout <<"File opening is fail.";
//    }
//}

void errp(char *s, int code) {
    fprintf(stderr, "Error: %s -- %s\n", s, strerror(code));
}

void thr_sleep(time_t sec, long nsec) {
    struct timeval now;
    struct timezone tz;
    struct timespec ts;
    int retcode;

    pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    retcode = pthread_mutex_lock(&m);
    if (retcode) {
        fprintf(stderr, "Error: mutex_lock\n");
        exit(0);
    }

    gettimeofday(&now, &tz);
    ts.tv_sec = now.tv_sec + sec + (nsec / 1000000000L);
    ts.tv_nsec = now.tv_usec * 1000 + (nsec % 1000000000L);
    if (ts.tv_nsec > 1000000000L) {
        (ts.tv_sec)++;
        (ts.tv_nsec) -= 1000000000L;
    }

    retcode = pthread_cond_timedwait(&cond, &m, &ts);
    if (retcode != ETIMEDOUT) {
        if (retcode == 0) {
            fprintf(stderr, "pthread_cond_timedwait, (zero)\n");
        } else {
            fprintf(stderr, "pthread_cond_timedwait, %s\n", strerror(retcode));
            exit(0);
        }
    }

    retcode = pthread_mutex_unlock(&m);
    if (retcode) {
        fprintf(stderr, "mutex_unlock, %s\n", strerror(retcode));
        exit(0);
    }

    retcode = pthread_cond_destroy(&cond);
    if (retcode) {
        fprintf(stderr, "mutex_cond_destroy, %s\n", strerror(retcode));
        fflush(stdout);
    }

    retcode = pthread_mutex_destroy(&m);
    if (retcode) {
        fprintf(stderr, "mutex_destroy, %s\n", strerror(retcode));
        exit(0);
    }
}

void *SAT_vc(void *arg) {
    RESULT *result = (RESULT *) arg;
    clockid_t cid;
    int s = pthread_getcpuclockid(pthread_self(), &cid);
    if (s != 0){
        errp("pthread_get cpu clockid error", s);
    }
    result->getResultCnfSat();
    struct timespec tm;
    clock_gettime(cid,&tm);
    if(result->vertexCover.size() == 0){
        result->runTime = 0;
    } else{
        result->runTime = (unsigned long int)  tm.tv_sec * 1000 * 1000 + tm.tv_nsec / 1000;
    }
    return nullptr;
}

void *VC1_vc(void *arg) {
    RESULT *result = (RESULT *) arg;
    clockid_t cid;
    int s = pthread_getcpuclockid(pthread_self(), &cid);
    if (s != 0) {
        errp("pthread_get cpu clockid error", s);
    }
    result->vertexCover = APPROX_VC_1(result->g->maxVertex, result->g->adj);
    struct timespec tm;
    clock_gettime(cid, &tm);
    result->runTime = tm.tv_sec * 1000 * 1000 + tm.tv_nsec / 1000;
//    result->methodType = "APPROX-VC-1";
//    cout << "VC1-runtime"<<tm.tv_sec * 1000*1000 + tm.tv_nsec / 1000 <<endl;
    return nullptr;
}

void *VC2_vc(void *arg) {
    RESULT *result = (RESULT *) arg;
    clockid_t cid;
    int s = pthread_getcpuclockid(pthread_self(), &cid);
    if (s != 0) {
        errp("pthread_get cpu clockid error", s);
    }
    result->vertexCover = APPROX_VC_2(result->g->maxVertex, result->g->edge);
    struct timespec tm;
    clock_gettime(cid, &tm);
    result->runTime = tm.tv_sec * 1000 * 1000 + tm.tv_nsec / 1000;
//    result->methodType = "APPROX-VC-2";
//    cout << "VC2-runtime"<<tm.tv_sec * 1000 * 1000 + tm.tv_nsec / 1000 <<endl;
    return nullptr;
}

int main(int argc, char **argv) {
    Graph g;
    RESULT res[3];

    while (!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);

        if (line[0] == 'V') {
            g.edge.clear();
            for (int i = 0; i < 3; i++) { res[i].vertexCover.clear(); }
            g.maxVertex = getVertexNum(line);

        } else if (line[0] == 'E') {
            if (g.maxVertex > 0) {
                g.edge = getEdge(line, g.maxVertex);
                g.adj = Adj(g.edge, g.maxVertex);
                if (g.edge.empty()) continue;

                pthread_t children[3];
                for (int i = 0; i < 3; i++) { res[i].g = &g; }

                pthread_create(&children[0], NULL, SAT_vc, (void *) &res[0]);
                pthread_create(&children[1], NULL, VC1_vc, (void *) &res[1]);
                pthread_create(&children[2], NULL, VC2_vc, (void *) &res[2]);

                thr_sleep(5,0);
                (res[0].solver)->interrupt();

                for (int i = 1; i < 3; i++) {
                    pthread_join(children[i], NULL);
                }

                res[0].methodType ="CNF-SAT-VC";
                res[1].methodType ="APPROX-VC-1";
                res[2].methodType ="APPROX-VC-2";

                output(res[0].vertexCover, res[0].methodType);
                output(res[1].vertexCover, res[1].methodType);
                output(res[2].vertexCover, res[2].methodType);
//                outRunTime(res);
            }
        }
    }
    return 0;
}