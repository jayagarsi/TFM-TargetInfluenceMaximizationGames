# Target Influence Maximization

This repository holds the source code for the experiments of the work A game theory approach to set thresholds for influence spread. The details on the models and the experiments can be found in the doc folder. Regarding the organization we have the typical software tree:
- data: folder with the generated plots, networks, centralities and results obtained with the experiments. Due to constraints of the Github repository memory, we cannot upload the full set of files.
- src: source folder. Here you can find the implemented classes and methods, as well as their description. The Graph and Process_Data class have been implemented by different persons, as mentioned in the documentation. The other classes have been fully implemented by us.
- plots: generated plots in Latex for the experimental results.
- results: daat gathered from the experiments. The data is seperated between the first experiment and the second.

Regarding the source code, to execute the experiments on can compile the code with ```make```, and then execute the generated ```./experiments``` executable file. This will execute the experiments for each network and store them in the results folder. Notice that some of this networks are very large, therefore it will take a long time to execute them. 
