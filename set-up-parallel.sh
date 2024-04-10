#!/bin/bash

# Dichiarazione delle variabili

command="AMDuProfCLI"
argument0="collect --config tbp"
num_iterazioni=5  # Numero di iterazioni desiderate
num_exponenti=15  # Numero di esponenti desiderati



#per ogni esponte da 0 a 15
for ((i=0; i<=$num_exponenti; i++)); do
    dir="results/set-up-par/$i"

    mkdir -p $dir
    #for each thread
    for ((j=2; j<=25; j++)); do
        for ((k=1; k<=$num_iterazioni; k++)); do
            echo "Esecuzione $k/$num_iterazioni con esponente $(($i)) e thread $j"

            #genero il file di output
            AMDuProfCLI collect --config threading -o $dir src/par $i $j > /dev/null 2>&1
        done
    done
done