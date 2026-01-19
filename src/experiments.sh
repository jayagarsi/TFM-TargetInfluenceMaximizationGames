#!/bin/bash -l
#
#SBATCH -J my_experiments
#SBATCH -o my_experiments."%j".out
#SBATCH -e my_experiments."%j".err
#
#SBATCH --mail-user $jaya.garcia@estudiantat.upc.edu
#SBATCH --mail-type=ALL
#
#SBATCH --mem=8192M
#SBATCH -c 26
#SBATCH -p medium
#SBATCH -w node302

make

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

./experiments
