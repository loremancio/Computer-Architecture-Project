#!/bin/bash

# Dichiarazione delle variabili

command="AMDuProfCLI"
argument0="collect --config tbp"
num_iterazioni=30  # Numero di iterazioni desiderate
num_exponenti=20  # Numero di esponenti desiderati

cartella="results/par"
#se la cartella non esiste la creo
if [ ! -d "$cartella" ]; then
    mkdir -p $cartella
fi



#per ogni thread da 1 a 24
for ((i=1; i<=24; i++)); do
    dir="results/par/thread-$i"

    mkdir -p $dir
    #for each exponent
    for ((j=10; j<=$num_exponenti; j++)); do
        for ((k=1; k<=$num_iterazioni; k++)); do
            echo "Esecuzione $k/$num_iterazioni con esponente $(($j)) e thread $i"

            #genero il file di output
            AMDuProfCLI collect --config threading -o $dir src/par $j $i > /dev/null 2>&1
        done
    done