#ifndef C_PROCESS_DATA_H
#define C_PROCESS_DATA_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include "Graph.hh"
# include "Statistics.hh"
#include <chrono>
#include <utility>

using namespace std;

typedef pair<int,int> PII;

typedef pair<int,double> PID;
typedef vector<PID> VPID;


typedef vector<int> VI;
typedef vector<double> VD;

typedef struct
{
    string exp;
    std::chrono::duration<double> time;
    double std;
} PST;

typedef vector<PST> VPST;



enum Data
{
    HIGGS, ARXIV, DINING_TABLE, DOLPHINS, HUMAN_BRAIN, GNUTELLA, EPINIONS, WIKIPEDIA, CAIDA, AMAZON, ENRON
};

class Process_Data
{
private:
    string pHiggs = "higgs-retweet_network.txt";
    string pArxiv = "CA-GrQc.txt";
    string pDining_Table = "Dining-table_partners.txt";
    string pDolphins = "dolphins.txt";
    string pHuman_Brain = "budapest_connectome_3.0_209_0_median.txt";
    string pEpinions = "soc-Epinions1.txt";
    string pWikipedia = "Wiki-Vote.txt";
    string pCaida = "as-caida20071105.txt";
    string pEnron = "Email-Enron.txt";
    string pAmazon = "com-amazon.ungraph.txt";
    string pGnutella = "p2p-Gnutella31.txt";

    string data_path = "../data/networks/";
    string data_metrics = "../data/centralities/";
    string outpath = "../data/results/";

    string result_header = "Network,N,InitialProp,InfluenceProp,InfluenceTargetProp,Rounds,MinDegreeIni,MaxDegreeIni,AvgDegreeIni,MinPageIni,MaxPageIni,AvgPageIni,MinBtwIni,MaxBtwIni,AvgBtwIni,MinDegreeTar,MaxDegreeTar,AvgDegreeTar,MinPageTar,MaxPageTar,AvgPageTar,MinBtwTar,MaxBtwTar,AvgBtwTar";

    void read_file(VE &V,string fn, bool weighted);
    void read_file(VE &V, string fn, bool weighted, bool ignore);
    void read_file(VE &V, VD& pg, VD& bw, string name, string fn, bool weighted, bool ignore);

    // void generate_graph(Graph &G, const VE &V, bool directed,double th);
    void generate_graph(Graph &G, const VE &V, const VD& pg, const VD& bw, bool directed, double th);

    bool is_empty(ifstream &file);

public:

    void read_graph(Graph &G, Data data,double th);
    //void read_graph(Graph &G, Data data, bool ignore);
    void create_file(string path);
    void write_statistics(string path, const Graph& G, const Statistics& stats, string network);
    void write_thresholds(string path, const Statistics& stats);
    void write_ranking(const VD &R, const VI &M,string subpath,string fn,string field);
    void write_times(const VPST &V, string subpath, string fn, bool append);
    string get_name_data_set(Data data);

};


#endif //C_PROCESS_DATA_H
