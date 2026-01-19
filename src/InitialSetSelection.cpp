/**
 * @file InitialSetSelection.cpp
 * @author Jaya García
 * @brief Implementation of the InitialSetSelection class
 * @version 0.1
 * @date 2026-01-18
 * 
 * @copyright Copyright (c) 2026
 * 
 */

# include "InitialSetSelection.hh"
# include <iostream>
# include <algorithm>
# include <omp.h>

InitialSetSelection::InitialSetSelection(Graph& H) : InfluenceMaximization(H) {
    // Initialize graph
    NP = G.N - num_target;  // #players = |V| - |T|
}

void InitialSetSelection::select_initial_configuration(USI& set) {
    NP = G.N - target_set.size();
    for (uint u = 0; u < G.N; ++u) {
        if (nodes_type[u] != TARGET) {
            if (set.find(u) != set.end()) {
                strategy_profile[u] = 1;
            }
            else {
                strategy_profile[u] = 0;
            }
        }
    }
}

void InitialSetSelection::select_initial_configuration(string mode) {
    NP = G.N - target_set.size();
    if (mode == "empty") {
        for (uint u = 0; u < G.N; ++u)
            if (nodes_type[u] != TARGET)
                strategy_profile[u] = 0;
    }
    else if (mode == "complete") {
        for (uint u = 0; u < G.N; ++u)
            if (nodes_type[u] != TARGET)
                strategy_profile[u] = 1;
    }
    else if (mode == "random") {
        std::random_device rd;
        std::uniform_int_distribution<int> distribution(1, NP);
        int num_participants = distribution(generator);
        
        VI nodes(G.N);
        for (uint u = 0; u < G.N; ++u)
            nodes[u] = u;
        
        std::shuffle(nodes.begin(), nodes.end(), generator);

        uint p = 0;
        for (uint u = 0; u < G.N; ++u) {
            if (nodes_type[u] != TARGET) {
                if (p < num_participants)
                    strategy_profile[u] = 1;
                else
                    strategy_profile[u] = 0;
                ++p;
            }
        }
    }
}

void InitialSetSelection::select_initial_configuration(string mode, std::mt19937& gen) {
    NP = G.N - target_set.size();
    if (mode == "empty") {
        for (uint u = 0; u < G.N; ++u)
            if (nodes_type[u] != TARGET)
                strategy_profile[u] = 0;
    }
    else if (mode == "complete") {
        for (uint u = 0; u < G.N; ++u)
            if (nodes_type[u] != TARGET)
                strategy_profile[u] = 1;
    }
    else if (mode == "random") {
        std::random_device rd;
        std::uniform_int_distribution<int> distribution(1, NP);
        int num_participants = distribution(gen);
        
        VI nodes(G.N);
        for (uint u = 0; u < G.N; ++u)
            nodes[u] = u;
        
        std::shuffle(nodes.begin(), nodes.end(), gen);

        uint p = 0;
        for (uint u = 0; u < G.N; ++u) {
            if (nodes_type[u] != TARGET) {
                if (p < num_participants)
                    strategy_profile[u] = 1;
                else
                    strategy_profile[u] = 0;
                ++p;
            }
        }
    }
}

double InitialSetSelection::compute_cost(int u, int action) {
    // c_u(s) = |T| - |F(I_s) \cap T| + \alpha s_u
    USI played_set;
    for (auto& s: strategy_profile)
        if (s.second)
            played_set.insert(s.first);
    if (action) played_set.insert(u);
    else played_set.erase(u);

    USI influence_expansion;
    G.expand_influence(played_set, influence_expansion);
    uint influence_size = G.intersection_size(influence_expansion, target_set);
    double cost = num_target - influence_size + alpha*action;
    return cost;
}

int InitialSetSelection::best_response(int u) {
    double non_participation_cost;
    double participation_cost;

    // Try first when the agent does not participate: s_v = 0
    # pragma omp parallel
    # pragma omp single
    {
        #pragma omp task
        {
        non_participation_cost = compute_cost(u, 0);
        }

        // Try second when the agent does participate: s_v = 1
        #pragma omp task
        {
        participation_cost = compute_cost(u, 1);
        }
    }
    return (non_participation_cost < participation_cost) ? 0 : 1;
}

uint InitialSetSelection::game_dynamics() {
    bool some_improved = true;
    uint n_rounds = 0;
    VI player_nodes = VI(NP, 0);
    uint index = 0;
    for (uint v = 0; v < G.N; ++v) {
        if (nodes_type[v] == PLAYER) {
            player_nodes[index] = v;
            ++index;
        }
    }

    while (some_improved) {
        some_improved = false;
        // cout << "Round: " << n_rounds << endl;
        for (auto& v : player_nodes) {
            
            // print_profile();
            int br = best_response(v);
            // If the strategy profile is different from
            // the best response then agent can 
            if (strategy_profile[v] != br) {
                strategy_profile[v] = br;
                some_improved = true;
            }
        }
        ++n_rounds;
    }
    // Update the initial set instance
    for (auto& s: strategy_profile)
        if (s.second)
            initial_set.insert(s.first);
    G.expand_influence(initial_set, final_influence);
    return n_rounds;
}

void InitialSetSelection::print_profile() const {
    cout << "Strategy profile: ";
    print(nodes_type);
    cout << "                  ";
    VI strategy(G.N, -1);
    for (auto& s: strategy_profile)
        strategy[s.first] = s.second;
    for (auto& i: strategy) {
        if (i == -1)
            cout << "- ";
        else
            cout << i << " ";
    }
    cout << endl;
}
