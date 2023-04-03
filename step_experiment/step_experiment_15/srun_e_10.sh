#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1      ##This should be scheduled automatically
#SBATCH --cpus-per-task=48       ##This should be scheduled automatically
#SBATCH --nvram-options=none
#SBATCH --time=1-00:00:00
#SBATCH --array=0-3

# set number of qubits
#SBATCH --job-name=AqcPqc

# OMP settings
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
export OMP_PROC_BIND=TRUE
export OMP_PLACES=cores

# Report run information
echo "SLURM_JOB_NUM_NODES=$SLURM_JOB_NUM_NODES"
echo "SLURM_TASKS_PER_NODE=$SLURM_TASKS_PER_NODE"
echo "SLURM_CPUS_PER_TASK=$SLURM_CPUS_PER_TASK"
echo "OMP_NUM_THREADS=$OMP_NUM_THREADS"

TIME_S=700
STEPS=15

srun ../bin/AqcPqc -b -d stepExp -i $((TIME_S)) -s $((STEPS)) -q 10 --seedselect $((5*SLURM_ARRAY_TASK_ID)) &
srun ../bin/AqcPqc -b -d stepExp -i $((TIME_S)) -s $((STEPS)) -q 10 --seedselect $((5*SLURM_ARRAY_TASK_ID+1)) & 
srun ../bin/AqcPqc -b -d stepExp -i $((TIME_S)) -s $((STEPS)) -q 10 --seedselect $((5*SLURM_ARRAY_TASK_ID+2)) & 
srun ../bin/AqcPqc -b -d stepExp -i $((TIME_S)) -s $((STEPS)) -q 10 --seedselect $((5*SLURM_ARRAY_TASK_ID+3)) &
srun ../bin/AqcPqc -b -d stepExp -i $((TIME_S)) -s $((STEPS)) -q 10 --seedselect $((5*SLURM_ARRAY_TASK_ID+4)) & 

wait 

# Report job time
echo ""
echo "ROUGH WALLTIME: (mm:ss)"
squeue -h -j $SLURM_JOBID -o "%M" 
