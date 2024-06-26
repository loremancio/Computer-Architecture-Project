#!/bin/bash

allowed_strings=("par" "parO2" "parM" "parMO2")


input_string="$1"

#check if the input string is -h or --help
if [ "$input_string" == "-h" ] || [ "$input_string" == "--help" ] || [ $# -eq 0 ]; then
  echo "Usage: $0 [par|parO2|parM|parMO2] [numberOfExponents] [numOfIterations] [numOfThreads]"
  echo "Default values:"
  echo "  numberOfExponents: 24"
  echo "  numOfIterations: 30"
  echo "  numOfThreads: 24"
  echo
  echo "For multiple runs, use && to separate the commands, e.g."
    echo "  $0 [par|parO2|parM|parMO2] && $0 [par|parO2|parM|parMO2] && ..."
  exit 0
fi

# Check if the input string is in the allowed list
if [[ !("${allowed_strings[@]}" =~ "$input_string") ]]; then
  echo "Usage: $0 [par|parO2|parM|parMO2]"
  exit 1
fi

#if there isn't the second argument, the default value is 24
if [ -z "$2" ]; then
  numberOfExponents=24
else
  numberOfExponents=$2
fi

#if there isn't the third argument, the default value is 30
if [ -z "$3" ]; then
  numOfIterations=30
else
  numOfIterations=$3
fi

#if there isn't the fourth argument, the default value is 24
if [ -z "$4" ]; then
  numOfThreads=24
else
  numOfThreads=$4
fi

echo "Running $input_string with $numberOfExponents exponents, $numOfIterations iterations, and $numOfThreads threads"


# Create the directory if it does not exist
if [ ! -d "results/$input_string" ]; then
  mkdir -p "results/$input_string"
fi

for ((i=1; i<=$numOfThreads; i++)); do
  threads=$i
  dir="results/$input_string/$threads"
  mkdir -p $dir
  for ((j=8; j<=$numberOfExponents; j++)); do
    filePath="$dir/$j.txt"
    for ((k=1; k<=$numOfIterations; k++)); do
      ./binaries/$input_string $j $threads >> $filePath
    done
    echo "$input_string: Exponent $j/$numberOfExponents with $threads threads completed"
  done
done

