import networkx as nx
import pandas as pd
from pathlib import Path
from typing import Union
from Utilities import(
    Networks,
    file_to_name,
    is_directed,
    read_graph
)

data_path = '../../data/networks/'
results_path = '../../data/centralities/'

def compute_max_degree(G: Union[nx.Graph, nx.DiGraph], network_name: str):
    if nx.is_directed(G):
        degrees = [val for (_, val) in G.in_degree()]
        min_degree = min(degrees)
        max_degree = max(degrees)
    else:
        degrees = [val for (_, val) in nx.degree(G)]
        min_degree = min(degrees)
        max_degree = max(degrees)
    print(network_name, max_degree, min_degree)

def compute_centralities(G: Union[nx.Graph, nx.DiGraph], network_name: str):
    pagerank = nx.pagerank(G)
    pagerank = [pagerank[u] for u in sorted(pagerank)]
    betweenness = nx.betweenness_centrality(G)
    betweenness = [betweenness[u] for u in sorted(betweenness)]
    degree = nx.degree_centrality(G)        # in-degree/out-degree?
    degree = [degree[u] for u in sorted(degree)]
    output_path = results_path + network_name + "-centralities.txt"
    with open(output_path, 'w') as file:
        file.write('Node,Pagerank,Betweenness,Degree\n')
        for u in range(G.number_of_nodes()):
            line = str(u) + "," + str(pagerank[u]) + "," + str(betweenness[u]) + "," + str(degree[u]) + "\n"
            file.write(line)

if __name__ == '__main__':
    dir_path = Path(data_path)
    print("----- STARTING COMPUTATION -----")
    for network_path in dir_path.iterdir():
        network_name = network_path.name
        # print("Network", file_to_name[network_name])
        G = read_graph(is_directed[network_name], network_path, network_name)
        # compute_centralities(G, file_to_name[network_name])
        compute_max_degree(G, file_to_name[network_name])
