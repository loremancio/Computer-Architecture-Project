#!/bin/bash

# Dichiarazione delle variabili

command="AMDuProfCLI"
argument0="collect --config tbp"
num_iterazioni=30  # Numero di iterazioni desiderate
num_exponenti=20  # Numero di esponenti desiderati

cartella="results/seq"
#se la cartella non esiste la creo
if [ ! -d "$cartella" ]; then
    mkdir -p $cartella
fi



for ((i=10; i<=$num_exponenti; i++)); do
    dir="results/seq/$i"

    mkdir -p $dir
    for ((j=1; j<=$num_iterazioni; j++)); do
        echo "Esecuzione $j/$num_iterazioni con esponente $(($i))"

        #genero il file di output
        AMDuProfCLI collect --config threading -o $dir src/seq $i > /dev/null 2>&1




    done
done