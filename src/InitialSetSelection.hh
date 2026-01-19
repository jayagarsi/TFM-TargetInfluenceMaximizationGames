/**
 * @file InitialSetSelection.hh
 * @author Jaya García
 * @brief Header of the InitialSetSelection class
 * @version 0.1
 * @date 2026-01-18
 * 
 * @copyright Copyright (c) 2026
 * 
 */

# ifndef INITIAL_SET_HH
# define INITIAL_SET_HH

# include "InfluenceMaximization.hh"

/**
 * @brief Class for the Seed Set Selection Game
 * 
 * This class inherits from the InfluenceMaximization class
 * 
 */
class InitialSetSelection : public InfluenceMaximization {

public:
    /** @brief Strategy chosen for each agent */
    UMII strategy_profile;

    /** @brief Price paid by the agents in the cost function */
    const double alpha = 0.5;

    /** @brief Number of Players */
    uint NP;

    /**
     * @brief Construct a new Initial Set Selection object
     * 
     * @param H network
     */
    InitialSetSelection(Graph& H);

    /**
     * @brief Initializes the strategy profile with a given set
     * 
     * @param set nodes that participate
     */
    void select_initial_configuration(USI& set);

    /**
     * @brief Initializes the strategy profile
     * 
     * @param mode string with the mode of initialization
     *             "empty" | "complete" | "random"
     */
    void select_initial_configuration(string mode);

    /**
     * @brief Initializes the strategy profile with a given
     * random number generator
     * 
     * @param mode string with the mode of initialization
     *             "empty" | "complete" | "random"
     * @param gen random number generator
     */
    void select_initial_configuration(string mode, std::mt19937& gen);
    
    /**
     * @brief Computes the cost of an agent for a given action
     * in the game
     * 
     * @param u node
     * @param action chosen action
     * @return double cost
     */
    double compute_cost(int u, int action);

    /**
     * @brief Computes the Best Response of an agent in the game
     * 
     * @param u node 
     * @return int Best Response strategy
     */
    int best_response(int u);

    /**
     * @brief Simulates the Best Response Dynamics of the game
     * 
     * @return uint number of rounds played
     */
    uint game_dynamics();
    
    /**
     * @brief Prints the current strategy profile
     * 
     * If the number of nodes is too large, it is not
     * advised to use this method
     * 
     */
    void print_profile() const;
};

# endif