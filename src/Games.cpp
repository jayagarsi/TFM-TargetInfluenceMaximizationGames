# include <iostream>
# include <math.h>
# include <algorithm>
# include <random>
# include <list>
# include <omp.h>
# include "InitialSetSelection.hh"
# include "ThresholdSelection.hh"
# include "Process_Data.hh"
# include "Graph.hh"
# include "Statistics.hh"

using namespace std;

const double PROPORTION_TARGET = 0.2;
const double PROPORTION_INITIAL = 0.2;
const string FIRST_MODEL_CONF = "complete";         // random/complete/empty
const string SECOND_MODEL_CONF = "empty";           // random/complete/empty
const bool thresholds_malicious = true;            // cooperative or malicious
const uint NUM_REPS = 5;
const uint seed = 2000;

const string outpath = "../data/results/";
const string result_header = "Network,N,InitialProp,InfluenceProp,InfluenceTargetProp,MinDegreeIni,MaxDegreeIni,AvgDegreeIni,MinPageIni,MaxPageIni,AvgPageIni,MinBtwIni,MaxBtwIni,AvgBtwIni,MinDegreeInf,MaxDegreeInf,AvgDegreeInf,MinPageInf,MaxPageInf,AvgPageInf,MinBtwInf,MaxBtwInf,AvgBtwInf";

void first_experiment(const list<Data>& datasets) {
    cout << "----- FIRST EXPERIMENT -----" << endl;
    Process_Data PD;
    VD ths = {0.25, 0.5, 0.75, 0.95};
    std::mt19937 generator = std::mt19937(seed);
    # pragma omp parallel for
    for (auto& th: ths) {
        string path = outpath + "first-experiment/" + FIRST_MODEL_CONF + "/th-" + to_string(th).substr(0,4) + "/";
        PD.create_file(path + "model-1.txt");
        PD.create_file(path + "model-2.txt");

        string filename;
        cout << " -> Fixed Threshold " << th << endl;
        for(auto ds: datasets) {
            Graph G;
            PD.read_graph(G, ds, th);
            filename = PD.get_name_data_set(ds);
            cout << "Working on " << filename << "..."<< endl;

            Statistics initial_state;
            Statistics final_state(G.N);
            for (uint i = 0; i < NUM_REPS; ++i) {
                // Re initialize thresholds
                G.assign_thresholds(th);

                // First model
                InitialSetSelection IS(G);
                IS.select_target_set(PROPORTION_TARGET, generator);
                IS.select_initial_configuration(FIRST_MODEL_CONF, generator);
                uint rounds = IS.game_dynamics();
                initial_state.update_metrics(G, IS.initial_set, IS.target_set, IS.final_influence, rounds);
                
                // Second model
                ThresholdSelection TS(G);
                TS.select_initial_set(IS.initial_set);
                TS.select_target_set(IS.target_set);
                TS.select_initial_configuration(SECOND_MODEL_CONF);
                rounds = TS.game_dynamics();
            
                final_state.update_metrics(G, TS.initial_set, TS.target_set, TS.final_influence, rounds);
            }
            initial_state.average_statistics(NUM_REPS);
            final_state.average_statistics(NUM_REPS);
            PD.write_statistics(path + "model-1.txt", G, initial_state, filename);
            PD.write_statistics(path + "model-2.txt", G, final_state, filename);
            PD.write_thresholds(path + "thresholds/" + PD.get_name_data_set(ds) + ".txt", final_state);
        }
        cout << "Done!"<< endl;
    }
}

void second_experiment(const list<Data>& datasets) {
    cout << "----- SECOND EXPERIMENT -----" << endl;
    Process_Data PD;
    string path = outpath + "second-experiment/";
    std::mt19937 generator = std::mt19937(seed);
    if (not thresholds_malicious)
        path += "cooperative/";
    else
        path += "malicious/";
    
    PD.create_file(path + "model-1.txt");
    PD.create_file(path + "model-2.txt");
    for (auto ds: datasets) {
        Graph G;
        PD.read_graph(G, ds, 0.5);
        string filename = PD.get_name_data_set(ds);
        cout << "Working on " << filename << "..."<< endl;
        
        Statistics original_state;
        Statistics final_state(G.N);
        for (uint i = 0; i < NUM_REPS; ++i) {

            ThresholdSelection TS(G, thresholds_malicious);
            
            TS.select_initial_set(PROPORTION_INITIAL, generator);
            TS.select_target_set(PROPORTION_TARGET, generator);
            TS.select_initial_configuration(SECOND_MODEL_CONF);
            uint rounds = TS.game_dynamics();
            original_state.update_metrics(G, TS.initial_set, TS.target_set, TS.final_influence, rounds);

            InitialSetSelection IS(G);
            IS.select_target_set(TS.target_set);
            IS.select_initial_configuration(TS.initial_set);
            rounds = IS.game_dynamics();
            final_state.update_metrics(G, IS.initial_set, IS.target_set, IS.final_influence, rounds);
        }
        original_state.average_statistics(NUM_REPS);
        final_state.average_statistics(NUM_REPS);
        PD.write_statistics(path + "model-2.txt", G, original_state, filename);
        PD.write_statistics(path + "model-1.txt", G, final_state, filename);
        PD.write_thresholds(path + "thresholds/" + PD.get_name_data_set(ds) + ".txt", final_state);
    }
    cout << "Done" << endl;
}

int main() {
    // Set of datasets
    list<Data> datasets({DINING_TABLE, DOLPHINS, HUMAN_BRAIN, ARXIV, WIKIPEDIA, CAIDA});//, ENRON, GNUTELLA, EPINIONS, HIGGS});
    // Run both experiments
    // first_experiment(datasets);
    // second_experiment(datasets);
    cout << "------ ALL FINISHED! ------" << endl;
}