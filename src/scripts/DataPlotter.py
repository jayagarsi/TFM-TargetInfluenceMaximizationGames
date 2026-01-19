from enum import Enum
import pandas as pd
from pathlib import Path
from typing import (
    List,
    Dict,
)

from Utilities import (
    ths_plot_prefix, 
    ths_plot_suffix,
    table_prefix,
    table_suffix,
    ths_subplot_prefix
)

results_path = "../../data/results/"
plots_path = "../../data/plots/"

header_access = {
    'network': 'Network',
    'N': 'N',
    'pi_I': 'InitialProp',
    'pi_F': 'InfluenceProp',
    'pi_T': 'InfluenceTargetProp',
    'MinDI': 'MinDegreeIni',
    'MaxDI': 'MaxDegreeIni',
    'AvgDI': 'AvgDegreeIni',
    'MinPI': 'MinPageIni',
    'MaxPI': 'MaxPageIni',
    'AvgPI': 'AvgPageIni',
    'MinBI': 'MinBtwIni',
    'MaxBI': 'MaxBtwIni',
    'AvgBI': 'AvgBtwIni',
    'MinDT': 'MinDegreeTar',
    'MaxDT': 'MaxDegreeTar',
    'AvgDT': 'AvgDegreeTar',
    'MinPT': 'MinPageTar',
    'MaxPT': 'MaxPageTar',
    'AvgPT': 'AvgPageTar',
    'MinBT': 'MinBtwTar',
    'MaxBT': 'MaxBtwTar',
    'AvgBT': 'AvgBtwTar',
}

class Networks(Enum):
    DINING_TABLE = "Dining_Table"
    DOLPHINS = 'Dolphins'
    HUMAN_BRAIN = 'Human_Brain'
    ARXIV = 'ArXiv'
    WIKIPEDIA = 'Wikipedia'
    CAIDA = 'Caida'
    ENRON = 'ENRON'
    GNUTELLA = 'Gnutella'
    EPINIONS = 'Epinions'
    HIGGS = 'Higgs'
#    AMAZON = 'Amazon'

epsilon = 10e-6      # 10^-5

THS_RANGE = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1]

class Statistics:
    def __init__(self, network):
        self.network = network.replace('_', ' ')
        self.N = 0
        self.pi_I = 0
        self.pi_T = 0
        self.pi_F = 0
        self.rounds = 0
        self.degree_initial = [0, 0, 0]
        self.degree_influence = [0, 0, 0]
        self.page_rank_initial = [0, 0, 0]
        self.page_rank_influence = [0, 0, 0]
        self.betweenness_initial = [0, 0, 0]
        self.betweenness_influence = [0, 0, 0]

def file_reader(file_path: str) -> Dict[str, Statistics]:
    networks = [n.value for n in Networks]
    networks_stats = {}
    for network in networks:
        networks_stats[network] = Statistics(network)

    data = pd.read_csv(file_path, sep=",")
    for _, row in data.iterrows():
        if row['Network'] not in networks:
            continue
        s = Statistics(row['Network'])
        s.N = row['N']
        s.pi_I = row[header_access['pi_I']]
        s.pi_T = row[header_access['pi_T']]
        s.pi_F = row[header_access['pi_F']]
        s.rounds = row['Rounds']
        s.degree_initial = [
            row[header_access['MinDI']],
            row[header_access['MaxDI']],
            row[header_access['AvgDI']],
        ]
        s.degree_influence = [
            row[header_access['MinDT']],
            row[header_access['MaxDT']],
            row[header_access['AvgDT']],
        ]
        s.page_rank_initial = [
            row[header_access['MinPI']],
            row[header_access['MaxPI']],
            row[header_access['AvgPI']],
        ]
        s.page_rank_influence = [
            row[header_access['MinPT']],
            row[header_access['MaxPT']],
            row[header_access['AvgPT']],
        ]        
        s.betweennes_initial = [
            row[header_access['MinBI']],
            row[header_access['MaxBI']],
            row[header_access['AvgBI']],
        ]
        s.betweenness_influence = [
            row[header_access['MinBT']],
            row[header_access['MaxBT']],
            row[header_access['AvgBT']],
        ]
        networks_stats[row['Network']] = s
    return networks_stats

def string(value: float) -> str:
    # If value has no decimals return an integer
    if value!= 0 and value < epsilon:
        return "$< \epsilon$"
    # Return float with two float positions
    # if value < 1e-4:
    #     value = value*1000
    return f"{value:.4f}"

def count_threshold_occurency(file_path: str) -> None:
    count = {}
    for range_ths in THS_RANGE:
        count[range_ths] = 0
    num_nodes = 0
    with open(file_path, 'r') as file:
        # Skip header line
        next(file)
        for line in file:
            line = line.split(',')
            ths = float(line[1])
            num_nodes += 1
            for range_ths in THS_RANGE:
                if ths <= range_ths:
                    count[range_ths] += 1
                    break
    for range_ths in THS_RANGE:
        count[range_ths] /= num_nodes
    return count

def generate_single_bar_plot(occurrency_count: Dict[float, float], network: str) -> None:
    max_occurency = max(occurrency_count.values())
    max_occurency = 1 if max_occurency > 0.9 else max_occurency + 0.1
    plot = ths_subplot_prefix(network, max_occurency)
    plot += "       "
    for key, value in occurrency_count.items():
        plot += "(" + str(key-0.1) + "," + str(value) + ") "
    plot += "(1," + str(occurrency_count[THS_RANGE[-1]]) + ")"  # repeat last value for proper plot
    plot += "\n    };"
    return plot

def generate_thresholds_bar_plots(thresholds_path: str, out_path: str, experiment: str, ths: str, conf: str):
    dir_path = Path(thresholds_path)
    network_count = {}
    for path in dir_path.glob("*.txt"):
        occurency = count_threshold_occurency(path)
        network_count[path.stem] = occurency

    small_plot_order = [
        Networks.DINING_TABLE.value,
        Networks.DOLPHINS.value,
        Networks.HUMAN_BRAIN.value,
        Networks.ARXIV.value,
        Networks.WIKIPEDIA.value
    ]
    big_plot_order = [
        Networks.CAIDA.value,
        Networks.ENRON.value,
        Networks.GNUTELLA.value,
        Networks.EPINIONS.value,
        Networks.HIGGS.value
        # Networks.AMAZON.value
    ]
    plot = ths_plot_prefix()
    for network in small_plot_order:
        sub_plot = generate_single_bar_plot(network_count[network], network)
        plot += "\n" + sub_plot + "\n"
    plot += ths_plot_suffix(experiment, ths, "small", conf)
    with open(out_path + "thresholds-range-small.tex", "w") as file:
        file.write(plot)

    plot = ths_plot_prefix()
    for network in big_plot_order:
        sub_plot = generate_single_bar_plot(network_count[network], network)
        plot += "\n" + sub_plot + "\n"
    plot += ths_plot_suffix(experiment, ths, "big", conf)
    with open(out_path + "thresholds-range-big.tex", "w") as file:
        file.write(plot)

def fill_metric_table(network_stats: List[Statistics], out_path: str, experiment: str, model: str, ths: str, conf: str) -> str:
    # Generate long table
    table_string = table_prefix(experiment, model, ths, "long", conf)
    for stat in network_stats.values():
        table_string += ("  \\textbf{" + stat.network + "} & " + 
                         string(stat.pi_I) + " & " + string(stat.pi_F) + " & " + string(stat.pi_T) + " & " + string(stat.rounds) + " & " +
                         string(stat.degree_initial[0]) + " & " + string(stat.degree_initial[2]) + " & " + string(stat.degree_initial[1]) + " & " +
                         string(stat.degree_influence[0]) + " & " + string(stat.degree_influence[2]) + " & " + string(stat.degree_influence[1]) + " & " +
                         string(stat.betweenness_initial[0]) + " & " + string(stat.betweenness_initial[2]) + " & " + string(stat.betweenness_initial[1]) + " & " +
                         string(stat.betweenness_influence[0]) + " & " + string(stat.betweenness_influence[2]) + " & " + string(stat.betweenness_influence[1]) + " & " +
                         string(stat.page_rank_initial[0]) + " & " + string(stat.page_rank_initial[2]) + " & " + string(stat.page_rank_initial[1]) + " & " +
                         string(stat.page_rank_influence[0]) + " & " + string(stat.page_rank_influence[2]) + " & " + string(stat.page_rank_influence[1]) +
                         " \\\\" + "\n")
    table_string += table_suffix("long")
    with open(out_path + "-long.tex", 'w') as file:
        file.write(table_string)

    # Generate short table
    table_string = table_prefix(experiment, model, ths, "short", conf)
    for stat in network_stats.values():
        table_string += ("  \\textbf{" + stat.network + "} & " + 
                         string(stat.pi_I) + " & " + string(stat.pi_F) + " & " + string(stat.pi_T) + " & " + string(stat.rounds) + " & " +
                         string(stat.degree_initial[2]) + " & " + string(stat.degree_influence[2]) + " & " +
                         string(stat.betweenness_initial[2]) + " & " + string(stat.betweenness_influence[2]) + " & " +
                         string(stat.page_rank_initial[2]) + " & " + string(stat.page_rank_influence[2]) +
                         " \\\\" + "\n")
    table_string += table_suffix("short")
    with open(out_path + "-short.tex", 'w') as file:
        file.write(table_string)

def generate_all_tables(type_first='complete', type_second='cooperative') -> None:
    # First experiment
    thresholds = ["0.25", "0.50", "0.75", "0.95"]
    models = ['model-1', 'model-2']
    model_to_string = {
        'model-1': 'first',
        'model-2': 'second'
    }
    exp = "first"
    for th in thresholds:
        for model in models:
            input_path = results_path + "first-experiment/" + type_first + "/th-" + th + "/"
            model_path = input_path + model + ".txt"
            stats = file_reader(model_path)
            out_path = plots_path + "first-experiment/" + type_first + "/th-" + th + "/" + model
            fill_metric_table(stats, out_path, exp, model_to_string[model], th, type_first)

    # Second experiment
    exp = "second"
    for model in models:
        input_path = results_path + "second-experiment/" + type_second + "/"
        model_path = input_path + model + ".txt"
        stats = file_reader(model_path)
        out_path = plots_path + "second-experiment/" + type_second + "/" + model
        fill_metric_table(stats, out_path, exp, model_to_string[model], "", type_second)

def generate_all_plots(type_first='complete', type_second='cooperative') -> None:
    # First experiment
    thresholds = ["0.25", "0.50", "0.75", "0.95"]
    exp = "first"
    for th in thresholds:
        input_path = results_path + "first-experiment/" + type_first + "/th-" + th + "/thresholds/"
        out_path = plots_path + "first-experiment/" + type_first + "/th-" + th + "/"
        generate_thresholds_bar_plots(input_path, out_path, exp, th, type_first)

    # Second experiment
    exp = "second"
    input_path = results_path + "second-experiment/" + type_second + "/thresholds"
    output_path = plots_path + "second-experiment/" + type_second + "/"
    generate_thresholds_bar_plots(input_path, output_path, exp, "", type_second)

if __name__ == "__main__":
    types_first = ['complete', 'random']
    types_second = ['cooperative', 'malicious']

    for i in range(2):
        generate_all_tables(types_first[i], types_second[i])
        generate_all_plots(types_first[i], types_second[i])
