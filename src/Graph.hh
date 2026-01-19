# ifndef GRAPH_HH
# define GRAPH_HH

# include <vector>
# include <map>
# include <unordered_map>
# include <unordered_set>
# include <queue>
# include <string>

using namespace std;

struct Info {
    double influence;
    bool influenced;
    int last_spread_level;

    Info(): influence(0), influenced(false), last_spread_level(-1) {}
};

typedef Info Info;
typedef vector<Info> VInfo;

struct edge
{
  int v;
  int u;
  double w;

  edge(int v, int u, double w) : v(v), u(u), w(w){}
};

typedef edge edge;
typedef vector<edge> VE;

typedef vector<int> VI;
typedef vector<double> VD;

typedef pair<int,double> PID;
typedef vector<PID> VPID;
typedef vector<VPID> VVPID;

typedef unordered_set<int> USI;
typedef queue<int> QI;

typedef unordered_map<int, int> UMII;

using uint = unsigned int;


// using VE = vector<edge>;

// using PID = pair<int, double>;
// using VI = vector<int>;
// using VD = vector<double>;
// using VPID = vector<PID>;
// using VVPID = vector<VPID>;
// using USI = unordered_set<int>;
// using QI = queue<int>;

class Graph {

public:
    uint N, E = 0;
    bool directed = false;

    VD threshold;
    VI mapping;
    VVPID adjacency;
    VVPID predecessors;

    VD betweenness;
    VD pagerank;

    Graph();
    Graph(const VE &Edges, const VD& pg, const VD& bw, bool directed, double th);
    
    //once a graph is created, we have to assign the values for the threshold vector
    void assign_thresholds(string mode);
    void assign_thresholds(string mode, string filename, bool cpl);
    void assign_thresholds(double th);
    void assign_thresholds(VD& ths);
    void assign_threshold(uint v, double th);

    bool is_subset(const USI& set_a, const USI& set_b) const;
    uint intersection_size(const USI& set_a, const USI& set_b) const;
    uint in_degree(uint v) const;
    uint out_degree(uint v) const;
    void expand_influence(USI& initial_set, USI& influenced_nodes);

    Graph stochastic() const;
    VI dangling_nodes() const;

    void print() const;
};

# endif