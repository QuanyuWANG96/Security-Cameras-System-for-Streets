// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>

std::vector<std::vector<int>> getAdj( int a,  std::vector<int> edge) {
    std::vector<int>::size_type i = 0;
    std::vector<std::vector<int>> adj;
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

//
//std::vector<int> split( const std::string &s, const std::string &seperator) {
//    std::vector<std::string> result;
//    std::vector<int> edge;
//    typedef std::string::size_type string_size;
//
//    string_size i = 0;
//
//    while (i != s.size()) {
//        int flag = 0;
//        while (i != s.size() && flag == 0) {
//            flag = 1;
//            for (string_size x = 0; x < seperator.size(); ++x)
//                if (s[i] == seperator[x]) {
//                    ++i;
//                    flag = 0;
//                    break;
//                }
//        }
//
//        flag = 0;
//        string_size j = i;
//        while (j != s.size() && flag == 0) {
//            for (string_size x = 0; x < seperator.size(); ++x)
//                if (s[j] == seperator[x]) {
//                    flag = 1;
//                    break;
//                }
//            if (flag == 0)
//                ++j;
//        }
//        if (i != j) {
//            edge.push_back(std::stoi(s.substr(i, j - i)));
//            i = j;
//        }
//    }
//    return edge;
//}


std::vector<int> BFS( std::vector<std::vector<int>> adj,  int s,  int v) {
    std::vector<int> path;
    std::vector<int> color(adj.size(), 0);
    std::vector<int> pi(adj.size(), adj.size()+1);
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
                    flag=1;
                    break;
                }
            }
        }
        if (flag==1){
            break;
        }
    }
    if (s == v) {
        path.push_back(s);
        path.push_back(s);
    } else if (pi[v] == adj.size()+1) {
        std::cout << "Error : no path exist between the vertices"<< std::endl;

    } else {
        for (std::vector<int>::size_type j = v; j != s; j = pi[j]) {
            path.push_back(j);
        }
        path.push_back(s);
    }

    return path;
}


int main(int argc, char** argv) {
    int maxVertex=-1;
    int s,v;
    std::vector<int> edge ;
    std::vector<std::vector<int>> adj;
    std::vector<int> path;

    while (!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);
//        std::cout <<"line   "<< line <<"****************"<< std::endl;
        std::string res;
        res = line.substr(2, line.length());

        if(line.length() == 0){
            continue;
        }
        if(line.find("V 0") != std::string::npos || line.find("E {}") != std::string::npos){
            continue;
        }
        if(line.find("s") == std::string::npos){
            std::cout << line << std::endl;
        }


        if( line[0] == 'V'){
            maxVertex = std::stoi(res);
            edge.clear();
            
        } else if (line[0] == 'E'){
            if (maxVertex<0){
                std::cerr << "Error : can not find vertex"<< std::endl;
                continue;
            }
            if(maxVertex > 0){

                std::string b;
                int num;

                std::string temp;
                for(int m =0; res[m] != '\0'; m++){
                    if(res[m] == ',' or res[m] == '<'  or res[m] == '>'){
                        temp.append(" ");
                    }else temp=temp+res[m];

                }

                std::istringstream line_s(temp);
                while(!line_s.eof()){
                    line_s >> b;

                    if(std::istringstream(b) >> num){
                        edge.push_back(num);
                    }
                }
//                edge = split(res, ",{}<>");
                for (std::vector<int>::size_type i = 0; i<edge.size() ; i++){
//                    std::cout << edge[i] <<"  ";
                    if (edge[i]>maxVertex or edge[i] <0){
                        std::cerr << "Error : Invalid vertice: "<< edge[i] << std::endl;
                        break;
                    }
                }
            }

        } else if (line[0] == 's'){
//            std::cerr << line << std::endl;
            if (maxVertex<0 ){
                std::cerr << "Error : can not find graph"<< std::endl;
                continue;
            }
            adj = getAdj(maxVertex, edge);

            std::vector<int> m ;
            std::string b;
            int num;
            std::istringstream line_s(line);
            while(!line_s.eof()){
                line_s >> b;
                if(std::istringstream(b) >> num){
                    m.push_back(num);
                }
            }
            s= m[0];
            v = m[1];
//            std::cout <<s<<  " "<<v<<std::endl;
            if (s<maxVertex and s>=0 and v<maxVertex and v>=0){
                path = BFS(adj,s,v);
                if (path.size()>0){
                    for (int j = path.size()-1; j >0; j--) {
                        std::cout << path[j] << "-";
                    }
                    std::cout << path[0] << std::endl;
                    path.clear();
                }
            } else{
                std::cerr << "Error : vertex out of bounds"<< std::endl;
                continue;
            }
        }

    }
//    exit(0);
return 0;
}
