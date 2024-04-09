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



#per 