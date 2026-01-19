import networkx as nx
import matplotlib.pyplot as plt
from pathlib import Path
from Utilities import (
    read_graph,
    is_directed,
    Networks,
    file_to_name
)

DATA_PATH = "../../data/networks/"
RESULTS_PATH = "../../data/results/"


if __name__ == "__main__":
    network = Networks.DINING_TABLE.value
    path = Path(DATA_PATH + network)
    G = read_graph(is_directed[network], path, file_to_name[network])
    plt.figure(figsize=(4,4))
    nx.draw(G, with_labels=True)
    plt.savefig(RESULTS_PATH + file_to_name[network] + ".png")
    plt.close()