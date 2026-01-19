/**
 * @file InfluenceMaximization.cpp
 * @author Jaya Garcia
 * @brief Implementation of the InfluenceMaximization class
 * @version 0.1
 * @date 2026-01-18
 * 
 * @copyright Copyright (c) 2026
 * 
 */

# include "InfluenceMaximization.hh"
# include <algorithm>
# include <iostream>

InfluenceMaximization::InfluenceMaximization(Graph& H) : G(H) {
    nodes_type = VT(H.N, PLAYER);
    generator = std::mt19937(seed);
}

void InfluenceMaximization::select_target_set(double proportion) {
    // Selection of target nodes uniformly at random
    num_target = G.N*proportion;
    // uniformly random number generator using mersenne twister engine
    std::random_device rd;
    
    VI nodes(G.N, 0);
    for (uint u = 0; u < G.N; ++u)
        nodes[u] = u;

    std::shuffle(nodes.begin(), nodes.end(), generator);

    uint i = 0;
    uint t = 0;
    while (t < num_target and i < G.N) {
        if (nodes_type[nodes[i]] != INITIAL) {
            nodes_type[nodes[i]] = TARGET;
            target_set.insert(nodes[i]);
            ++t;
        }
        ++i;
    }
}

void InfluenceMaximization::select_target_set(double proportion, std::mt19937& gen) {
    // Selection of target nodes uniformly at random
    num_target = G.N*proportion;
    // uniformly random number generator using mersenne twister engine
    std::random_device rd;
    
    VI nodes(G.N, 0);
    for (uint u = 0; u < G.N; ++u)
        nodes[u] = u;

    std::shuffle(nodes.begin(), nodes.end(), gen);

    uint i = 0;
    uint t = 0;
    while (t < num_target and i < G.N) {
        if (nodes_type[nodes[i]] != INITIAL) {
            nodes_type[nodes[i]] = TARGET;
            target_set.insert(nodes[i]);
            ++t;
        }
        ++i;
    }
}

void InfluenceMaximization::select_target_set(VI& target) {
    num_target = target.size();
    for (auto& t : target) {
        nodes_type[t] = TARGET;
        target_set.insert(t);
    }
}

void InfluenceMaximization::select_target_set(USI& target) {
    num_target = target.size();
    for (auto& t : target) {
        nodes_type[t] = TARGET;
        target_set.insert(t);
    }
}

void InfluenceMaximization::select_initial_set(double proportion) {
    // Selection of initial nodes uniformly at random
    num_initial = G.N*proportion;

    // uniformly random number generator using mersenne twister engine
    std::random_device rd;
    
    VI nodes(G.N);
    for (uint u = 0; u < G.N; ++u)
        nodes[u] = u;

    std::shuffle(nodes.begin(), nodes.end(), generator);

    uint i = 0;
    uint t = 0;
    while (t < num_initial and i < G.N) {
        if (nodes_type[nodes[i]] != TARGET) {
            nodes_type[nodes[i]] = INITIAL;
            initial_set.insert(nodes[i]);
            ++t;
        }
        ++i;
    }
}

void InfluenceMaximization::select_initial_set(double proportion, std::mt19937& gen) {
    // Selection of initial nodes uniformly at random
    num_initial = G.N*proportion;

    // uniformly random number generator using mersenne twister engine
    std::random_device rd;
    
    VI nodes(G.N);
    for (uint u = 0; u < G.N; ++u)
        nodes[u] = u;

    std::shuffle(nodes.begin(), nodes.end(), gen);

    uint i = 0;
    uint t = 0;
    while (t < num_initial and i < G.N) {
        if (nodes_type[nodes[i]] != TARGET) {
            nodes_type[nodes[i]] = INITIAL;
            initial_set.insert(nodes[i]);
            ++t;
        }
        ++i;
    }
}

void InfluenceMaximization::select_initial_set(VI& initial) {
    num_initial = initial.size();
    for (auto& i : initial) {
        nodes_type[i] = INITIAL;
        initial_set.insert(i);
    }
}

void InfluenceMaximization::select_initial_set(USI& initial) {
    num_initial = initial.size();
    for (auto& i : initial) {
        nodes_type[i] = INITIAL;
        initial_set.insert(i);
    }
}

void gather_statistics() {
    // This method is to be reimplemented in the subclasses
}

void InfluenceMaximization::print(const VI& v) const {
    if (v.empty()) cout << "Empty set!";
    else
        for (auto& i: v)
            cout << i << " ";
    cout << endl;
}

void InfluenceMaximization::print(const VT& t) const {
    if (t.empty()) cout << "Empty set!";
    else
        for (auto& i: t) {
            if (i == TARGET)
                cout << "T ";
            else if (i == INITIAL)
                cout << "I ";
            else
                cout << "P ";
        }
    cout << endl;
}

void InfluenceMaximization::print(const VD& d) const {
    if (d.empty()) cout << "Empty set!";
    else
        for (auto& i: d)
            cout << i << " ";
    cout << endl;
}

void InfluenceMaximization::print(const USI& us) const {
    if (us.empty()) cout << "Empty set!";
    else 
        for (auto& i: us)
            cout << i << " ";
    cout << endl;
}

void InfluenceMaximization::print(const UMII& um) const {
    if (um.empty()) cout << "Empty set!";
    else
        for (auto& m: um)
            cout << "(" << m.first << ", " << m.second << ")";
    cout << endl;
}

void InfluenceMaximization::print_target_set() const {
    for (uint u = 0; u < G.N; ++u)
        if (nodes_type[u] == TARGET)
            cout << u << " ";
    cout << endl;
}