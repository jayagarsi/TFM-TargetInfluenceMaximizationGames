# include "ThresholdSelection.hh"
# include <iostream>

ThresholdSelection::ThresholdSelection(Graph& H) : InfluenceMaximization(H) { }

ThresholdSelection::ThresholdSelection(Graph& H, bool malicious) : InfluenceMaximization(H) {
    this->malicious = malicious;
}

void ThresholdSelection::select_initial_configuration(string mode) {
    NP = G.N - num_target - num_initial;
    if(mode == "random") {

    }
    else if (mode == "empty") {
        for (uint u = 0; u < G.N; ++u)
            if (nodes_type[u] != TARGET)
                strategy_profile.insert(make_pair(u, 1));
    }
    else if (mode == "complete") {
        for (uint u = 0; u < G.N; ++u)
            if (nodes_type[u] != TARGET)
                strategy_profile.insert(make_pair(u, G.in_degree(u)));
    }
}

void ThresholdSelection::select_initial_configuration(string mode, std::mt19937 gen) {
    NP = G.N - num_target - num_initial;
    if(mode == "random") {

    }
    else if (mode == "empty") {
        for (uint u = 0; u < G.N; ++u)
            if (nodes_type[u] != TARGET)
                strategy_profile.insert(make_pair(u, 1));
    }
    else if (mode == "complete") {
        for (uint u = 0; u < G.N; ++u)
            if (nodes_type[u] != TARGET)
                strategy_profile.insert(make_pair(u, G.in_degree(u)));
    }
}

int ThresholdSelection::compute_utility(int u) {
    USI influence_expansion;
    G.expand_influence(initial_set, influence_expansion);
    bool target_influenced = G.is_subset(target_set, influence_expansion);
    if (malicious) {
        if (not target_influenced)
            return strategy_profile[u];
        else
            return -strategy_profile[u];
    }
    else {
        if (target_influenced)
            return strategy_profile[u];
        else
            return -strategy_profile[u];
    }
}

int ThresholdSelection::best_response(int u) {
    int best_ths;
    if (malicious) {
        best_ths = G.in_degree(u);
        G.assign_threshold(u, best_ths);
        USI influence_spread;
        G.expand_influence(initial_set, influence_spread);
        if (not G.is_subset(target_set, influence_spread)) {
            for (int ths = best_ths-1; ths > 0; --ths) {
                G.assign_threshold(u, ths);
                influence_spread.clear();
                G.expand_influence(initial_set, influence_spread);
                if (G.is_subset(target_set, influence_spread))
                    return best_ths;
                best_ths = ths;
            }
        }
    }
    else {
        best_ths = 1;
        G.assign_threshold(u, best_ths);
        USI influence_spread;
        G.expand_influence(initial_set, influence_spread);
        if (G.is_subset(target_set, influence_spread)) {
            for (int ths = 2; ths < G.in_degree(u); ++ths) {
                G.assign_threshold(u, ths);
                influence_spread.clear();
                G.expand_influence(initial_set, influence_spread);
                if (not G.is_subset(target_set, influence_spread))
                    return best_ths;
                best_ths = ths;
            }
        }
    }
    return best_ths;
}

uint ThresholdSelection::game_dynamics() {
    bool some_improved = true;
    uint n_rounds = 0;
    VI player_nodes(NP);
    uint index = 0;
    for (uint v = 0; v < G.N; ++v) {
        if (nodes_type[v] == PLAYER) {
            player_nodes[index] = v;
            ++index;
        }
    }
    while (some_improved) {
        some_improved = false;
        for (auto& v: player_nodes) {
            int br = best_response(v);
            // If the strategy profile is different from
            // the best response then agent can improve
            if (strategy_profile[v] != br) {
                strategy_profile[v] = br;
                G.assign_threshold(v, br);
                some_improved = true;
            }
        }
        ++n_rounds;
    }
    G.expand_influence(initial_set, final_influence);
    return n_rounds;
}

void ThresholdSelection::print_profile() const {
    cout << "Strategy profile: ";
    print(nodes_type);
    cout << "                  ";
    VI strategy(G.N, -1);
    for (auto& s: strategy_profile)
        strategy[s.first] = s.second;
    for (uint v = 0; v < G.N; ++v) {
        if (strategy[v] == -1)
            cout << "- ";
        else
            cout << (double) strategy[v]/G.in_degree(v) << " ";
    }
    cout << endl;
}
