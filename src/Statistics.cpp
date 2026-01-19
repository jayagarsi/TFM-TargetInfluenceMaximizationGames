/**
 * @file Statistics.cpp
 * @author Jaya García
 * @brief Implementation of the Statistics class
 * @version 0.1
 * @date 2026-01-18
 * 
 * @copyright Copyright (c) 2026
 * 
 */

# include "Statistics.hh"
# include <iostream>

Statistics::Statistics() {}

Statistics::Statistics(uint N) {
    thresholds = VD(N, 0.0);
}

void Statistics::update_metrics(const Graph& G, USI& initial, USI& target, USI& influence_expansion, uint rounds) {
    uint influence_size = G.intersection_size(target, influence_expansion);
    pi_I += (double) initial.size()/G.N;
    pi_F += (double) influence_expansion.size()/G.N;
    pi_T += (double) influence_size/target.size();
    num_rounds += rounds;

    for (auto& m: METRICS) {
        for (auto& v: initial) {
            double value = get_metric(G, v, m, true);
            initial_metrics[m].add(value);
        }
        for (auto& v: influence_expansion) {
            double value = get_metric(G, v, m, false);
            influence_metrics[m].add(value);
        }
        initial_metrics[m].accumulate_global();
        influence_metrics[m].accumulate_global();
    }

    if (thresholds.size() > 0)
        for (uint u = 0; u < G.N; ++u) {
            uint degree = G.in_degree(u);
            if (degree == 0) 
                degree = 1;
            thresholds[u] += (double) G.threshold[u]/degree;
        }
}

void Statistics::average_statistics(uint num_reps) {
    pi_I /= num_reps;
    pi_F /= num_reps;
    pi_T /= num_reps;
    num_rounds /= num_reps;

    for (auto& m: METRICS) {
        initial_metrics[m].global_average(num_reps);
        influence_metrics[m].global_average(num_reps);
    }

    if (thresholds.size() > 0)
        for (uint u = 0; u < thresholds.size(); ++u)
            thresholds[u] /= num_reps;

}

double Statistics::get_metric(const Graph& G, uint node, Metric m, bool in_degree) {
    switch(m) {
        case DEGREE:
            return (in_degree) ? G.in_degree(node) : G.out_degree(node);
            break;
        case PAGERANK:
            return G.pagerank[node];
            break;
        case BETWENNESS:
            return G.betweenness[node];
            break;
        default:
            return 0.0;
            break;
    }
}

void Statistics::print() const {
    cout << "Proportion of initial nodes " << pi_I << endl;
    cout << "Proportion of influenced nodes " << pi_F << endl;
    cout << "Proportion of target influenced nodes " << pi_T << endl;
    for (auto& m: METRICS) {
        switch(m) {
            case DEGREE:
                cout << "Accumulated Degree:" << endl;
                break;
            case PAGERANK:
                cout << "Accumulated Pagerank:" << endl;
                break;
            case BETWENNESS:
                cout << "Accumulated Betweenness:" << endl;
                break;
        }
        cout << " - Min: " << initial_metrics[m].global_min << endl;
        cout << " - Max: " << initial_metrics[m].global_max << endl;
        cout << " - Avg: " << initial_metrics[m].global_avg << endl;
    }
}