/**
 * @file InfluenceMaximization.hh
 * @author Jaya Garcia
 * @brief Header of the InfluenceMaximization class
 * @version 0.1
 * @date 2026-01-18
 * 
 * @copyright Copyright (c) 2026
 * 
 */

# ifndef INFLUENCE_MAXIMIZATION_HH
# define INFLUENCE_MAXIMIZATION_HH

# include "Graph.hh"
# include <random>

/**
 * @brief Type enumeratio
 * 
 * This will help us easily diferrentiate between
 * Target, Initial and Player nodes.
 * 
 */
enum Type {
    TARGET, INITIAL, PLAYER
};

typedef vector<Type> VT;

/** @class InfluenceMaximization
 * @brief Placeholder for the Target Set Influence Games.
 * 
 * This class serves as a parent for the Seed Selection Game and Threshold Selection Game.
 * Since many definitions are the same for both games, this class is necessary to avoid
 * repeating code. 
 * 
 * Disclaimer: THIS CLASS SHOULD NOT BE INSTANCIATED.
 * 
 */
class InfluenceMaximization {

public:

    /** @brief Instance of the network */
    Graph& G;

    /** @brief array with the type of the nodes: Initial, Target or Player */
    VT nodes_type;

    /** @brief Number of target nodes */
    uint num_target;

    /** @brief Number of initial nodes */
    uint num_initial;

    /** @brief Target set */
    USI target_set;

    /** @brief Initial set */
    USI initial_set;

    /** @brief Influenced nodes */
    USI final_influence;

    /** @brief Seed value */
    unsigned seed = 2000;

    /** @brief Random Number Generator */
    std::mt19937 generator;

    /**
     * @brief Construct a new Influence Maximization object
     * 
     * @param H input graph 
     */
    InfluenceMaximization(Graph& H);

    /**
     * @brief Randomly initializes a target set
     * 
     * @param proportion proportion of nodes in the target set
     */
    void select_target_set(double proportion);

    /**
     * @brief Randomly initializes a target set
     * 
     * @param proportion proportion of nodes in the target set
     * @param gen random number generator instance
     */
    void select_target_set(double proportion, std::mt19937& gen);

    /**
     * @brief Initializes the target set from a list of nodes
     * 
     * @param target set of nodes as a list
     */
    void select_target_set(VI& target);

    /**
     * @brief Initializes the target set from a set of nodes
     * 
     * @param target set of nodes in an unordered map
     */
    void select_target_set(USI& target);

    /**
     * @brief Randomly initializes the initial set
     * 
     * @param proportion proportion of nodes in the initial set
     */
    void select_initial_set(double proportion);

    /**
     * @brief Randomly initializes the initial set
     * 
     * @param proportion proportion of nodes in the initial set
     * @param gen random number generator instance
     */
    void select_initial_set(double proportion, std::mt19937& gen);

    /**
     * @brief Initializes the target set from a set of nodes
     * 
     * @param initial set of nodes in an unordered map
     */
    void select_initial_set(VI& initial);

    /**
     * @brief Initializes the target set from a set of nodes
     * 
     * @param initial set of nodes in an unordered map
     */
    void select_initial_set(USI& initial);

    /**
     * @brief Method to compute the metrics from the sets
     * 
     * This method is abstract, hence it should be reimplemented
     * in the child classes
     * 
     */
    void gather_statistics();

    /**
     * @brief Prints a given integer vector
     * 
     * @param v array
     */
    void print(const VI& v) const;

    /**
     * @brief Prints a given type vector
     * 
     * @param t array
     */
    void print(const VT& t) const;

    /**
     * @brief Prints a given double vector
     * 
     * @param d array
     */
    void print(const VD& d) const;

    /**
     * @brief Prints a given unordered set of ints
     * 
     * @param us unordered set
     */
    void print(const USI& us) const;

    /**
     * @brief Prints a given unorderd map of int to int
     * 
     * @param um unordered map
     */
    void print(const UMII& um) const;

    /**
     * @brief Prints the target set
     * 
     */
    void print_target_set() const;
};

# endif