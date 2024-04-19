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
for ((i=1; i<=6; i++)); do
    threads=$((2 ** (i-1)))  # Calculate threads: 1, 2, 4, 8, 16, 32
    dir="results/par/thread-$threads"

    mkdir -p $dir
    #for each exponent
    for ((j=10; j<=$num_exponenti; j++)); do
        newDir="$dir/$j"
        mkdir -p $newDir
        for ((k=1; k<=$num_iterazioni; k++)); do

            #genero il file di output
            AMDuProfCLI collect --config threading -o $newDir src/par $j $threads > /dev/null 2>&1
        done
        echo "Esponente $j/$num_exponenti con $threads threads terminata"
    done

done

prendere misurazioni ben fatte e plottare
verificare eventuali flessioni per diversi thread al variare del carico
