#include "Process_Data.hh"
# include "Statistics.hh"

void Process_Data::read_file(VE &V, string fn, bool weighted)
{
  ifstream file(data_path + fn);

  V = VE();

  if (weighted)
  {
    int v, u;
    double w;
    while (file >> v >> u >> w) V.push_back(edge(v, u, fabs(w)));
  }
  else
  {
    int v, u;
    while (file >> v >> u) V.push_back(edge(v, u, 1));
  }
}

void Process_Data::read_file(VE &V, VD& pg, VD& bw, string name, string fn, bool weighted, bool ignore) {
    ifstream file(data_path + fn);
    // cout << data_path + fn << endl;

    V = VE();

    if (weighted) {
        int v, u;
        double w;
        if (not ignore) while (file >> v >> u >> w) V.push_back(edge(v, u, fabs(w)));
        else while (file >> v >> u >> w) V.push_back(edge(v, u, 1));
    }
    else {
        int v, u;
        while (file >> v >> u) V.push_back(edge(v, u, 1));
    }

    int node;
    double metric;
    char comma;

    ifstream pg_file(data_metrics + "pagerank/" + name);

    string header;
    getline(pg_file, header);      // remove header
    while (pg_file >> node >> comma >> metric) pg.push_back(metric);

    ifstream bw_file(data_metrics + "betweenness/" + name);
    getline(bw_file, header);
    while (bw_file >> node >> comma >> metric) bw.push_back(metric);
}

void Process_Data::read_file(VE &V, string fn, bool weighted, bool ignore) {
    ifstream file(data_path + fn);
    V = VE();

    if (weighted) {
        int v, u;
        double w;
        if (not ignore) while (file >> v >> u >> w) V.push_back(edge(v, u, fabs(w)));
        else while (file >> v >> u >> w) V.push_back(edge(v, u, 1));
    }
    else {
        int v, u;
        while (file >> v >> u) V.push_back(edge(v, u, 1));
    }
}

void Process_Data::generate_graph(Graph &G, const VE &V, const VD& pg, const VD& bw, bool directed,double th)
{
  G = Graph(V, pg, bw, directed, th);
}


void Process_Data::read_graph(Graph &G, Data data, double th)
{
  VE V;
  VD pg, bw;
  string name = get_name_data_set(data);
  switch (data)
  {
    case GNUTELLA:
      read_file(V, pg, bw, name, pGnutella, false, true);     // NON WEIGHTED
      generate_graph(G, V, pg, bw, true, th);
      break;
    case AMAZON:
      read_file(V, pg, bw, name, pAmazon, false, true);       // NON WEIGHTED
      generate_graph(G, V, pg, bw, false, th);
      break;
    case HIGGS:
      read_file(V, pg, bw, name, pHiggs, true, true);         // WEIGHTED
      generate_graph(G, V, pg, bw, true, th);
      break;
    case ARXIV:
      read_file(V, pg, bw, name, pArxiv, false, true);
      generate_graph(G, V, pg, bw, false, th);
      break;
    case DINING_TABLE:
      read_file(V, pg, bw, name, pDining_Table, true, true);      // WEIGHTED
      generate_graph(G, V, pg, bw, true,th);
      break;
    case DOLPHINS:
      read_file(V, pg, bw, name, pDolphins, false, true);
      generate_graph(G, V, pg, bw, false,th);
      break;
    case HUMAN_BRAIN:
      read_file(V, pg, bw, name, pHuman_Brain, true, true);      // WEIGHTED
      generate_graph(G, V, pg, bw, false, th);
      break;
    case EPINIONS:
      read_file(V, pg, bw, name, pEpinions, false, true);
      generate_graph(G, V, pg, bw, true, th);
      break;
    case ENRON:
      read_file(V, pg, bw, name, pEnron, false, true);
      generate_graph(G, V, pg, bw, false, th);
      break;
    case WIKIPEDIA:
      read_file(V, pg, bw, name, pWikipedia, false, true);
      generate_graph(G, V, pg, bw, true,th);
      break;
    case CAIDA:
      read_file(V, pg, bw, name, pCaida, true, true);        // WEIGHTED
      generate_graph(G, V, pg, bw, true,th);
      break;
  }
}

void Process_Data::create_file(string path) {
    ofstream file;
    file.open(path);
    file << result_header << endl;
    file.close();
}


void Process_Data::write_statistics(string path, const Graph& G, const Statistics& stats, string network) {
    ofstream file;
    file.open(path, std::ofstream::app);
    file << network << "," << to_string(G.N) << ",";
    file << to_string(stats.pi_I) << "," << to_string(stats.pi_F) << "," << to_string(stats.pi_T) << ",";
    file << to_string(stats.num_rounds);
    for (auto& m: METRICS) {
        file << "," << to_string(stats.initial_metrics[m].global_min);
        file << "," << to_string(stats.initial_metrics[m].global_max);
        file << "," << to_string(stats.initial_metrics[m].global_avg);
    }
    for (auto& m: METRICS) {
        file << "," << to_string(stats.influence_metrics[m].global_min);
        file << "," << to_string(stats.influence_metrics[m].global_max);
        file << "," << to_string(stats.influence_metrics[m].global_avg);
    }
    file << endl;
    file.close();
}

void Process_Data::write_thresholds(string path, const Statistics& stats) {
    ofstream file;
    file.open(path);
    file << "Node,Threshold" << endl;
    for (uint u = 0; u < stats.thresholds.size(); ++u) 
        file << to_string(u) << "," << to_string(stats.thresholds[u]) << endl;
    file.close();
}

string Process_Data::get_name_data_set(Data data)
{
  switch (data)
  {
    case GNUTELLA:
      return "Gnutella";
    case AMAZON:
      return "Amazon";
    case ENRON:
      return "ENRON";
    case HIGGS:
      return "Higgs";
    case ARXIV:
      return "ArXiv";
    case DINING_TABLE:
      return "Dining_Table";
    case DOLPHINS:
      return "Dolphins";
    case HUMAN_BRAIN:
      return "Human_Brain";
    case EPINIONS:
      return "Epinions";
    case WIKIPEDIA:
      return "Wikipedia";
    case CAIDA:
      return "Caida";
  }
  return "";
}

bool Process_Data::is_empty(ifstream &file)
{
  return file.peek() == fstream::traits_type::eof();
}
