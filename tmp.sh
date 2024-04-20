#!/bin/bash

# Dichiarazione delle variabili

command="AMDuProfCLI"
argument0="collect --config tbp"
num_iterazioni=30  # Numero di iterazioni desiderate
num_exponenti=22  # Numero di esponenti desiderati

cartella="results/par"
#se la cartella non esiste la creo
if [ ! -d "$cartella" ]; then
    mkdir -p $cartella
fi


#per ogni thread da 1 a 24
for ((i=1; i<=24; i++)); do
    #threads=$((2 ** (i-1)))  # Calculate threads: 1, 2, 4, 8, 16, 32
    threads=$i
    dir="results/par/$threads"

    mkdir -p $dir
    #for each exponent
    for ((j=10; j<=$num_exponenti; j++)); do
        newDir="$dir/$j.txt"
        
        
        for ((k=1; k<=$num_iterazioni; k++)); do
            #echo "./binaries/par $threads $j $newDir"
            #genero il file di output
            #echo "./binaries/par $threads $j > $newDir"
            ./binaries/par $j $threads >> $newDir
        done
        echo "Esponente $j/$num_exponenti con $threads threads terminata"
    done

done

