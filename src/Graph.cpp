#include "Graph.hh"
#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <algorithm>
# include <omp.h>

Graph::Graph() = default;

Graph::Graph(const VE &Edges, const VD& pg, const VD& bw, bool directed, double th) {
    E = static_cast<int>(Edges.size());
    this->directed = directed;
    adjacency = VVPID();
    mapping = VI();
    threshold = VD();
    if (directed) predecessors = VVPID();

    int last_node = 0;
    UMII MP;


    for (edge e : Edges) {
        if (MP.find(e.v) == MP.end()) {
            mapping.push_back(e.v);
            MP[e.v] = last_node++;
            adjacency.push_back(VPID());
            if (directed) predecessors.push_back(VPID());
            threshold.push_back(0);
        }
        if (MP.find(e.u) == MP.end())
        {
            mapping.push_back(e.u);
            MP[e.u] = last_node++;
            adjacency.push_back(VPID());
            if (directed) predecessors.push_back(VPID());
            threshold.push_back(0);
        }
        int v = MP[e.v];
        int u = MP[e.u];

        adjacency[v].push_back(make_pair(u, e.w));
        threshold[u] += e.w;

        if (not directed) {
            adjacency[u].push_back(make_pair(v, e.w));
            threshold[v] += e.w;
        }
        else predecessors[u].push_back(make_pair(v, e.w));
    }

    cout << endl;
    N = static_cast<int>(threshold.size());
    betweenness.assign(bw.begin(), bw.end());
    pagerank.assign(pg.begin(), pg.end());
}

void Graph::assign_thresholds(string mode){
  if(mode == "random_uniform"){
    //asignacion de umbrales aleatorios uniformemente distribuido
    int n = this->threshold.size();
    // uniformly random number generator using mersenne twister engine
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> dist(0.0,1.0);

    for (double &v : this->threshold)
        v = int(v * dist(generator)) + 1;
  }
  else if(mode == "random_uniform_0-0.5"){
    //asignacion de umbrales aleatorios uniformemente distribuido
    int n = this->threshold.size();
    // uniformly random number generator using mersenne twister engine
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> dist(0.0,0.5);

    for (double &v : this->threshold)
        v = int(v * dist(generator)) + 1;
  }
  else if(mode == "random_uniform_0.5-1"){
    //asignacion de umbrales aleatorios uniformemente distribuido
    int n = this->threshold.size();
    // uniformly random number generator using mersenne twister engine
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> dist(0.5,1.0);

    for (double &v : this->threshold)
        v = int(v * dist(generator)) + 1;
  }
  else if(mode == "random_normal"){
    //asignacion de umbrales aleatorios siguiendo una distribucion de probabilidad normal
    int n = this->threshold.size();
    //generator using mersenne twister engine
    std::random_device rd;
    std::mt19937 generator(rd());
    //distribucion normal centrada en el 0.5 y con desviacion estandar de 1/6
    std::normal_distribution<double> dist(0.5,0.166666667);
    //los valores que den mas o menos de 1 o 0 seran redondeados. aprox el 0.3%
    for (double &v : this->threshold){
        double x = dist(generator);
        if(x < 0.0) x = 0.0;
        if(x > 1.0) x = 1.0;
        v = int(v * x) + 1;
      }
  }

}

void Graph::assign_thresholds(string mode, string filename, bool cpl){
  string path;
  if(mode == "fltr"){
    path = "../Experiments/old_FLTR/";
  }else if(mode == "icr"){
    path = "../Experiments/ICR/";
  }else if(mode == "betweenness"){
    path = "../Experiments/betweenness/";
  }else if(mode == "pagerank"){
    path = "../Experiments/pagerank/";
  }
  if(cpl == false){
    ifstream file(path + filename);
    string line;
    getline(file,line); //discard headers
    while(getline(file,line)){
      //ir linea por linea
      istringstream s(line);
      string field;
      getline(s,field,',');
      int node = stoi(field);
      //get reverse mapping of node number
      vector<int>::iterator idx = find(this->mapping.begin(),this->mapping.end(),node);
      if(idx != this->mapping.end())
        node = distance(this->mapping.begin(),idx);
      else
        cout << node << " not found" << endl;

      getline(s,field,',');
      double rank = stod(field);

      this->threshold[node]=int(this->threshold[node] * rank) + 1;
    }
  }
  else{
    ifstream file(path + filename);
    string line;
    getline(file,line); //discard headers
    while(getline(file,line)){
      //ir linea por linea
      istringstream s(line);
      string field;
      getline(s,field,',');
      int node = stoi(field);
      //get reverse mapping of node number
      vector<int>::iterator idx = find(this->mapping.begin(),this->mapping.end(),node);
      if(idx != this->mapping.end())
        node = distance(this->mapping.begin(),idx);
      else
        cout << node << " not found" << endl;

      getline(s,field,',');
      double rank = 1.0 - stod(field);

      this->threshold[node]=int(this->threshold[node] * rank) + 1;
    }
  }
}

void Graph::assign_thresholds(double th){
  for (double &v : this->threshold)
        v = int(v * th) + 1;
}

void Graph::assign_thresholds(VD& ths) {
    for (uint i = 0; i < threshold.size(); ++i)
        threshold[i] = ths[i];
}

void Graph::assign_threshold(uint v, double th) {
    threshold[v] = th;
}

bool Graph::is_subset(const USI& set_a, const USI& set_b) const {
    auto const end = set_b.end();
    for (const auto& elem: set_a)
        if (set_b.find(elem) == end)
            return false;
    return true;
}

uint Graph::intersection_size(const USI& set_a, const USI& set_b) const {
    uint size = 0;
    auto const end = set_b.end();
    for (const auto& elem: set_a)
        if (set_b.find(elem) != end)
            ++size;
    return size;
}

uint Graph::in_degree(uint v) const {
    return (directed ? predecessors[v].size() : adjacency[v].size());
}

uint Graph::out_degree(uint v) const {
    return adjacency[v].size();
}

void Graph::expand_influence(USI& initial_set, USI& influenced_nodes) {
    VInfo info(N, Info());
    QI Q;
    for (uint u : initial_set) {
        info[u].last_spread_level = 0;
        info[u].influenced = true;
        Q.push(u);
        influenced_nodes.insert(u);
    }

    while (not Q.empty()) {
        int v = Q.front();
        Q.pop();
        // # pragma omp parallel for
        for (auto edge : adjacency[v]) {
            // Edge (v, u) with weight w
            uint u = edge.first;
            double w = edge.second;
            if (not info[u].influenced) {
                info[u].influence += w*in_degree(u);
                if (info[u].influence >= threshold[u]) {
                    info[u].influenced = true;
                    // # pragma omp critical
                    // {
                    Q.push(u);
                    // }
                    influenced_nodes.insert(u);
                }
                info[u].last_spread_level = info[v].last_spread_level + 1;
            }
        }
    }
}

Graph Graph::stochastic() const {
  Graph G = Graph();
  G.N = this->N;
  G.E = this->E;
  G.directed = this->directed;
  G.mapping = this->mapping;

  VVPID adj;
  for (const VPID &V : this->adjacency)
  {
    VPID aux;
    double sw = 0;
    for (PID v : V) sw += v.second;

    if (sw > 0) for (PID v : V) aux.push_back(make_pair(v.first,v.second/sw));
    else for (PID v : V) aux.push_back(make_pair(v.first,0));
    adj.push_back(aux);
  }
  G.adjacency = adj;

  if (G.directed)
  {
    VVPID pred;
    for (const VPID &V : this->predecessors)
    {
      VPID aux;
      double sw = 0;
      for (PID v : V) sw += v.second;

      if (sw > 0) for (PID v : V) aux.push_back(make_pair(v.first,v.second/sw));
      else for (PID v : V) aux.push_back(make_pair(v.first,0));

      pred.push_back(aux);
    }
    G.predecessors = pred;
  }

  G.threshold = VD(G.N,1/2 + 1);

  return G;
}

VI Graph::dangling_nodes() const {
  VI res;
  for (int v = 0; v < N; ++v) if (adjacency[v].empty()) res.push_back(v);
  return res;
}

void Graph::print() const {
    if (N > 100) {
        cout << "Graph is too big to print!" << endl;
        return;
    }
    cout << "------------" << endl;
    cout << "ADJACENCIES" << endl;
    for (uint u = 0; u < N; ++u) {
        cout << u << ": ";
        if (adjacency[u].empty())
            cout << "-";
        for (uint v = 0; v < adjacency[u].size(); ++v) {
            cout << "(" << adjacency[u][v].first << ", " << adjacency[u][v].second << ")" << " ";
        }
        cout << endl;
    }
    // for (uint u = 0; u < N; ++u) {
    //     cout << u+1 << " ";
    //     uint num_adjacent = adjacency[u].size();
    //     VI V(N, 0);
    //     for (uint v = 0; v < adjacency[u].size(); ++v) {
    //         V[adjacency[u][v].first] = V[adjacency[u][v].second];
    //     }
    //     for (auto& vertex: V)
    //         cout << vertex << " ";
    //     cout << endl;
    // }
    cout << "THRESHOLDS" << endl;
    for (uint u = 0; u < N; ++u)
        cout << u << ": " << threshold[u] << " ";
    cout << endl << "------------" << endl;
}