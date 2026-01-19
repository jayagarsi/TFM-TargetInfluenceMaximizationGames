from enum import Enum
import networkx as nx
from pathlib import Path
from typing import(
    Union
)

def game(model: str):
    if model == "first":
        return "$\Gamma_I$"
    elif model == "second":
        return "$\Gamma_f$"
    else:
        return model

class Networks(Enum):
    AMAZON = 'com-amazon.ungraph.txt'
    ARXIV = 'CA-GrQc.txt'
    CAIDA = 'as-caida20071105.txt'
    DINING_TABLE = 'Dining-table_partners.txt'
    DOLPHINS = 'dolphins.txt'
    ENRON = 'Email-Enron.txt'
    EPINIONS = 'soc-Epinions1.txt'
    GNUTELLA = 'p2p-Gnutella31.txt'
    HIGGS = 'higgs-retweet_network.txt'
    HUMAN_BRAIN = 'budapest_connectome_3.0_209_0_median.txt'
    TEXAS = 'roadNet-TX.txt'
    WIKIPEDIA = 'Wiki-Vote.txt'

file_to_name = {
    Networks.AMAZON.value: 'Amazon',
    Networks.ARXIV.value: 'ArXiv',
    Networks.CAIDA.value: 'Caida',
    Networks.DINING_TABLE.value: 'Dining Table',
    Networks.DOLPHINS.value: 'Dolphins',
    Networks.ENRON.value: 'Enron',
    Networks.EPINIONS.value: 'Epinions',
    Networks.GNUTELLA.value: 'Gnutella',
    Networks.HIGGS.value: 'Higgs',
    Networks.HUMAN_BRAIN.value: 'Human Brain',
    Networks.WIKIPEDIA.value: 'Wikipedia'
}

is_directed = {
    Networks.AMAZON.value: False,
    Networks.ARXIV.value: False,
    Networks.CAIDA.value: True,
    Networks.DINING_TABLE.value: True,
    Networks.DOLPHINS.value: False,
    Networks.ENRON.value: False,
    Networks.EPINIONS.value: True,
    Networks.GNUTELLA.value: True,
    Networks.HIGGS.value: True,
    Networks.HUMAN_BRAIN.value: False,
    Networks.WIKIPEDIA.value: True
}

num_reps = 5
repetition_string = "The results are an average of " + str(num_reps) + " repetitions of the experiment."

def read_graph(is_directed: bool, file_path: Path, network_name: str) -> Union[nx.Graph, nx.DiGraph]:
    if is_directed:
        G = nx.DiGraph()
    else:
        G = nx.Graph()
    with open(file_path.as_posix(), 'r') as file:
        for line in file:
            line = line.split()
            if line:
                u = int(line[0])
                v = int(line[1])
                G.add_edge(u, v)
    return G


nets_from_size = {
    "small": "Dining Table, Dolphins, Human Brain, ArXiv, and Wikipedia",
    "big": "Caida, ENRON, Gnutella, Epinions and Higgs" # and Amazon"
}

def ths_plot_prefix() -> str:
    thresholds_plot_prefix = """\\begin{figure}[H]
    \centering
    \\begin{tikzpicture}
    
    \\begin{groupplot}[
        group style={
            group size=2 by 4,
            vertical sep=3cm,
            horizontal sep=2.5cm,
        },
        width=8cm,
        height=4.9cm,
        ybar interval,
        xlabel={Thresholds range},
        xtick=data,% reset from ybar interval
        xticklabel=
            {$(\pgfmathprintnumber\\tick,%
	        \pgfmathprintnumber\\nexttick]$},
        % xticklabel interval boundaries,
        x tick label style={font=\scriptsize,rotate=45,anchor=east,},
        ylabel={Proportion of nodes},
        enlargelimits=false
    ]
    """
    return thresholds_plot_prefix

def ths_plot_suffix(experiment: str, ths: str, size: str, conf: str) -> str:
    suffix = """    \end{groupplot}\n   \end{tikzpicture}\n"""
    if experiment == 'first':
        label = "fig:ths-first-" + ths + "-" + conf + "-" + size + "-nets"
        caption = """Threshold value ranges for the """ + experiment + """ experiment outcome starting with thresholds $""" + ths + """$ and initial """ + conf + """ configuration. 
    Each bar is the number of nodes that have a threshold value lying in the corresponding range. 
    These plots are for the """ + size + """ datasets: """ +  nets_from_size[size] + ". " + repetition_string
    else:
        label = "fig:ths-second-" + conf + "-" + size + "-nets"
        caption = """Threshold value ranges for the """ + experiment + """ experiment outcome with """ + conf + """ agents. 
    Each bar is the number of nodes that have a threshold value lying in the corresponding range. 
    These plots are for the """  + size + """ datasets: """ +  nets_from_size[size] + ". " + repetition_string
    suffix += "\caption{" + caption + "}\n"
    suffix += "\label{" + label + "}\n"
    suffix += "\end{figure}\n"
    return suffix

def ths_subplot_prefix(network: str, max_y: float) -> str:
    prefix = """
    \\nextgroupplot[
        title={\\textbf{""" + network.replace("_", " ") + """} },
        ymin=0, ymax="""+ f"{max_y:.2f}" + """,
    ]
    \\addplot coordinates { \n"""
    return prefix


def table_prefix(experiment: str, model: str, ths: str, type: str, conf: str) -> str:
    if experiment == "first":
        caption = """Table with the metrics of the """ + experiment + """ experiment with initial threshold """ + ths + """ and initial """ + conf + """ configuration for the initial and influence sets. 
        This table is the outcome of the """ + game(model) + """ model. """ + repetition_string
        label = "tab:metrics-" + experiment + "-" + ths + "-" + conf + "-" + model + "-model-" + type
    else:
        caption = """Table with the metrics of the """ + experiment + """ experiment with """ + conf + """ agents for the initial and influence sets. 
        This table is the outcome of the """ + game(model) + """ model. """ + repetition_string
        label = "tab:metrics-" + experiment + "-" + conf + "-" + model + "-model-" + type
    if type == "long":
        prefix = """\\begin{sidewaystable}[p]
\centering
\caption{Full """ + caption + """}
\label{""" + label + """}
\\resizebox{\\textheight}{!}{%
\\begin{tabular}{|c|cccc|ccc|ccc|ccc|ccc|ccc|ccc|}
\hline
\multirow{2}{*}{\\textbf{Network}} &
  \multicolumn{1}{c|}{\multirow{2}{*}{\\textbf{$\pi_I$}}} &
  \multicolumn{1}{c|}{\multirow{2}{*}{\\textbf{$\pi_F$}}} &
  \multicolumn{1}{c|}{\multirow{2}{*}{\\textbf{$\pi_T$}}} &
  \\textbf{Rounds} &
  \multicolumn{3}{c|}{\\textbf{\\begin{tabular}[c]{@{}c@{}}Out-Degree \\\\ Initial\end{tabular}}} &
  \multicolumn{3}{c|}{\\textbf{\\begin{tabular}[c]{@{}c@{}}In-Degree\\\\ Influence\end{tabular}}} &
  \multicolumn{3}{c|}{\\textbf{\\begin{tabular}[c]{@{}c@{}}Betweenness\\\\ Initial\end{tabular}}} &
  \multicolumn{3}{c|}{\\textbf{\\begin{tabular}[c]{@{}c@{}}Betweenness\\\\ Influence\end{tabular}}} &
  \multicolumn{3}{c|}{\\textbf{\\begin{tabular}[c]{@{}c@{}}Pagerank\\\\ Initial\end{tabular}}} &
  \multicolumn{3}{c|}{\\textbf{\\begin{tabular}[c]{@{}c@{}}Pagerank\\\\ Influence\end{tabular}}} \\\\ \cline{6-23} 
 &
  \multicolumn{1}{c|}{} &
  \multicolumn{1}{c|}{} &
  \multicolumn{1}{c|}{} &
   &
  \multicolumn{1}{c|}{\\textbf{Min}} &
  \multicolumn{1}{c|}{\\textbf{Avg}} &
  \\textbf{Max} &
  \multicolumn{1}{c|}{\\textbf{Min}} &
  \multicolumn{1}{c|}{\\textbf{Avg}} &
  \\textbf{Max} &
  \multicolumn{1}{c|}{\\textbf{Min}} &
  \multicolumn{1}{c|}{\\textbf{Avg}} &
  \\textbf{Max} &
  \multicolumn{1}{c|}{\\textbf{Min}} &
  \multicolumn{1}{c|}{\\textbf{Avg}} &
  \\textbf{Max} &
  \multicolumn{1}{c|}{\\textbf{Min}} &
  \multicolumn{1}{c|}{\\textbf{Avg}} &
  \\textbf{Max} &
  \multicolumn{1}{c|}{\\textbf{Min}} &
  \multicolumn{1}{c|}{\\textbf{Avg}} &
  \\textbf{Max} \\\\ \hline
  """
    if type == "short":
        prefix = """\\begin{table}[H]
\centering
\caption{""" + caption + """}
\label{""" + label + """}
\\resizebox{\\textwidth}{!}{%
\\begin{tabular}{|c|cccc|cc|cc|cc|}
\hline
\\textbf{Network} &
  \\textbf{$\pi_I$} &
  \\textbf{$\pi_F$} &
  \\textbf{$\pi_T$} &
  \\textbf{Rounds} &
  \\textbf{\\begin{tabular}[c]{@{}c@{}}Average \\\\ Out-Degree  \\\\ Initial\end{tabular}} &
  \\textbf{\\begin{tabular}[c]{@{}c@{}}Average \\\\ In-Degree \\\\ Influence\end{tabular}} &
  \\textbf{\\begin{tabular}[c]{@{}c@{}}Average \\\\ Betweenness \\\\ Initial\end{tabular}} &
  \\textbf{\\begin{tabular}[c]{@{}c@{}}Average \\\\ Betweenness \\\\ Influence\end{tabular}} &
  \\textbf{\\begin{tabular}[c]{@{}c@{}}Average \\\\ Pagerank \\\\ Initial\end{tabular}} &
  \\textbf{\\begin{tabular}[c]{@{}c@{}}Average \\\\ Pagerank \\\\ Influence\end{tabular}} \\\\ \hline
"""
    return prefix

def table_suffix(type: str) -> str:
    if type == "long":
        end = "sidewaystable"
    if type == "short":
        end = "table"
    suffix = "\n\hline\n\end{tabular}%\n}\n\end{"+ end +"}\n"
    return suffix
