CC = g++
CFLAGS = -O3 -std=c++14 -march=native -fopenmp

TARGET = Graph.o Statistics.o InfluenceMaximization.o InitialSetSelection.o ThresholdSelection.o Process_Data.o

GRAPH = Graph.cpp Graph.hh
INFLUENCE = InfluenceMaximization.cpp InfluenceMaximization.hh
INITIALSET = InitialSetSelection.cpp InitialSetSelection.hh
THRESHOLD = ThresholdSelection.cpp ThresholdSelection.hh
STATISTICS = Statistics.cpp Statistics.hh

all: experiments $(TARGET)

experiments: Games.cpp $(TARGET)
	$(CC) $(CFLAGS) $+ -o $@

Statistics.o: Statistics.cpp Graph.hh Statistics.hh
	g++ $(CFLAGS) -c Statistics.cpp

Graph.o: Graph.cpp Graph.hh
	g++ $(CFLAGS) -c Graph.cpp

InfluenceMaximization.o: InfluenceMaximization.cpp InfluenceMaximization.hh
	g++ $(CFLAGS) -c InfluenceMaximization.cpp

InitialSetSelection.o: InitialSetSelection.cpp InfluenceMaximization.hh InitialSetSelection.hh
	g++ $(CFLAGS) -c InitialSetSelection.cpp

ThresholdSelection.o: ThresholdSelection.cpp InfluenceMaximization.hh ThresholdSelection.hh
	g++ $(CFLAGS) -c ThresholdSelection.cpp

Process_Data.o: Process_Data.cpp Statistics.hh Process_Data.hh
	g++ $(CFLAGS) -c Process_Data.cpp

tar: Games.cpp $(TARGET) $(INFLUENCE) $(INITIAL) $(THRESHOLD) $(STATISTICS) Process_Data.hh Process_Data.cpp Makefile
	tar -czvf program.tar.gz $+ 

clean:
	rm -rf *.o experiments

cleanResults:
	rm -rf ../data/results/first-experiment/complete/th-0.25/*.txt \
	../data/results/first-experiment/complete/th-0.25/thresholds/*.txt \
	../data/results/first-experiment/complete/th-0.50/*.txt \
	../data/results/first-experiment/complete/th-0.50/thresholds/*.txt \
	../data/results/first-experiment/complete/th-0.75/*.txt \
	../data/results/first-experiment/complete/th-0.75/thresholds/*.txt \
	../data/results/first-experiment/complete/th-0.95/*.txt \
	../data/results/first-experiment/complete/th-0.95/thresholds/*.txt \
    ../data/results/first-experiment/random/th-0.25/*.txt \
	../data/results/first-experiment/random/th-0.25/thresholds/*.txt \
	../data/results/first-experiment/random/th-0.50/*.txt \
	../data/results/first-experiment/random/th-0.50/thresholds/*.txt \
	../data/results/first-experiment/random/th-0.75/*.txt \
	../data/results/first-experiment/random/th-0.75/thresholds/*.txt \
	../data/results/first-experiment/random/th-0.95/*.txt \
	../data/results/first-experiment/random/th-0.95/thresholds/*.txt \
	../data/results/second-experiment/cooperative/*.txt \
    ../data/results/second-experiment/cooperative/thresholds/*.txt \
    ../data/results/second-experiment/malicious/*.txt \
	../data/results/second-experiment/malicious/thresholds/*.txt \

cleanPlots:
	rm -rf ../data/plots/first-experiment/complete/th-0.25/*.tex \
	../data/plots/first-experiment/complete/th-0.50/*.tex \
	../data/plots/first-experiment/complete/th-0.75/*.tex \
	../data/plots/first-experiment/complete/th-0.95/*.tex \
	../data/plots/first-experiment/random/th-0.25/*.tex \
	../data/plots/first-experiment/random/th-0.50/*.tex \
	../data/plots/first-experiment/random/th-0.75/*.tex \
	../data/plots/first-experiment/random/th-0.95/*.tex \
	../data/plots/second-experiment/cooperative/*.tex \
	../data/plots/second-experiment/malicious/*.tex