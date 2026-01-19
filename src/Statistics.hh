/**
 * @file Statistics.hh
 * @author Jaya Garcia
 * @brief Header of the Statistics class
 * @version 0.1
 * @date 2026-01-18
 * 
 * @copyright Copyright (c) 2026
 * 
 */

# ifndef STATISTICS_HH
# define STATISTICS_HH

# include <limits>
# include "Graph.hh"

enum Metric {DEGREE, PAGERANK, BETWENNESS};

const vector<Metric> METRICS = {
    DEGREE, PAGERANK, BETWENNESS
};

const uint INF = 1e9;
const uint NUM_METRICS = 3;

/** @struct MetricSummary
 * @brief Holder for the values of a metric
 * 
 */
struct MetricSummary {

    /** @brief Minimum value of a single execution */
    double local_min = std::numeric_limits<double>::infinity();

    /** @brief Maximum value of a single execution */
    double local_max = -std::numeric_limits<double>::infinity();

    /** @brief Aggregated value of a single execution */
    double sum = 0.0;

    /** @brief Number of added values */
    size_t count = 0;

    /** @brief Minimum value of different executions */
    double global_min = 0;

    /** @brief Maximum value of different executions */
    double global_max = 0;

    /** @brief Average value of different execution */
    double global_avg = 0.0;
    
    /**
     * @brief Adds a new value to the summary
     * 
     * This method updates the maximum, minimum and average
     * 
     * @param value 
     */
    void add(double value) {
        local_min = min(local_min, value);
        local_max = max(local_max, value);
        sum += value;
        ++count;
    }

    /**
     * @brief Computes the averag of a single execution
     * 
     * @return double average
     */
    double avg() const {
        return count == 0 ? 0.0 : sum/count;
    }

    /**
     * @brief Restarts the temporary variables
     * 
     */
    void restart_variables() {
        count = 0;
        sum = 0.0;
        local_min = std::numeric_limits<double>::infinity();
        local_max = -std::numeric_limits<double>::infinity();
    }

    /**
     * @brief Accumulates the global variables
     * 
     */
    void accumulate_global() {
        global_min += local_min;
        global_max += local_max;
        global_avg += avg();
        restart_variables();
    }

    /**
     * @brief Computes the average of multiple executions
     * 
     * @param num_reps number of repetitions
     */
    void global_average(uint num_reps) {
        global_min /= num_reps;
        global_max /= num_reps;
        global_avg /= num_reps;
    }
};

/** @class Statistics
 * @brief Class that manages the Statistics
 * 
 * This class deals with the management of single executions
 * and global averages of the metrics.
 * 
 */
class Statistics {

public:
    /** @brief Proportion of initial nodes */
    double pi_I = 0.0;

    /** @brief Proportion of influenced nodes */
    double pi_F = 0.0;

    /** @brief Proportion of target nodes influenced */
    double pi_T = 0.0;

    /** @brief Number of rounds played */
    double num_rounds = 0.0;

    /** @brief Metrics of the initial set */
    vector<MetricSummary> initial_metrics = vector<MetricSummary>(NUM_METRICS);

    /** @brief Metrics of the influenced set */
    vector<MetricSummary> influence_metrics = vector<MetricSummary>(NUM_METRICS);

    /** @brief Thresholds of the agents */
    VD thresholds;

    /**
     * @brief Construct a new Statistics object
     * 
     */
    Statistics();

    /**
     * @brief Construct a new Statistics object
     * 
     * @param N number of nodes
     */
    Statistics(uint N);

    /**
     * @brief Updates the computed metrics
     * 
     * Computes the metrics and adds the results to the
     * partial results
     * 
     * @param G network
     * @param initial initial set
     * @param target target set
     * @param influence_expansion influenced set
     * @param rounds number of rounds taken
     */
    void update_metrics(const Graph& G, USI& initial, USI& target, USI& influence_expansion, uint rounds);
    
    /**
     * @brief Averages the results from different results
     * 
     * @param num_reps number of rounds
     */
    void average_statistics(uint num_reps);

    /**
     * @brief Prints the computesd metrics
     * 
     */
    void print() const;

private:

    /**
     * @brief Gets the value of a metric for a given node
     * 
     * @param G network
     * @param node node
     * @param m metric
     * @param in_degree True if query for in_degree False otherwise
     * @return double value of the metric
     */
    double get_metric(const Graph& G, uint node, Metric m, bool in_degree);

};

# endif