/**
 * @file ThresholdSelection.hh
 * @author Jaya García
 * @brief Header of the ThresholdSelection class
 * @version 0.1
 * @date 2026-01-18
 * 
 * @copyright Copyright (c) 2026
 * 
 */

# ifndef THRESHOLD_SELECTION_HH
# define THRESHOLD_SELECTION_HH

# include "InfluenceMaximization.hh"

/**
 * @brief Class for the Threshold Selection Game
 * 
 * This class inherits from the InfluenceMaximization class
 * 
 */
class ThresholdSelection : public InfluenceMaximization {

public:
    /** @brief Strategy chosen for each agent */
    UMII strategy_profile;

    /** @brief Number of Players */
    uint NP;

    /** @brief Variable for the type of the agents, False for cooperative True otherwise */
    bool malicious = false;

    /**
     * @brief Construct a new Threshold Selection object
     * 
     * @param H network
     * 
     */
    ThresholdSelection(Graph& H);

    /**
     * @brief Construct a new Threshold Selection object
     * 
     * @param H network
     * @param malicious type of the agents 
     */
    ThresholdSelection(Graph& H, bool malicious);

    /**
     * @brief Initializes the strategy profile with a given set
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
    void select_initial_configuration(string mode, std::mt19937 gen);
    
    /**
     * @brief Computes the utility of an agent for a given action
     * in the game
     * 
     * @param u node
     * @return int utility
     */
    int compute_utility(int u);

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