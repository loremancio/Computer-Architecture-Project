#!/bin/bash

allowed_strings=("bitonicSortGPU")


input_string="$1"

#check if the input string is -h or --help
if [ "$input_string" == "-h" ] || [ "$input_string" == "--help" ] || [ $# -eq 0 ] || [[ !("${allowed_strings[@]}" =~ "$input_string") ]]; then
  echo "Usage: $0 bitonicSortGPU [numberOfExponents] [numOfIterations] [numOfThreads]"
  echo "Default values:"
  echo "  numberOfExponents: 30"
  echo "  numOfIterations: 30"
  echo "  numOfThreads: 128"
  echo
  echo "NOTE: the number of threads is the number of threads per block, and can only be a power of 2"
  echo
  echo "For multiple runs, use && to separate the commands, e.g."
    echo "  $0 [par|parO2|parM|parMO2] && $0 [par|parO2|parM|parMO2] && ..."
  exit 0
fi


#if there isn't the second argument, the default value is 24
if [ -z "$2" ]; then
  numberOfExponents=30
else
  numberOfExponents=$2
fi

#if there isn't the third argument, the default value is 30
if [ -z "$3" ]; then
  numOfIterations=30
else
  numOfIterations=$3
fi

#if there isn't the fourth argument, the default value is 128
if [ -z "$4" ]; then
  numOfThreads=1024
else
  numOfThreads=$4
fi

echo "Running $input_string with $numberOfExponents exponents, $numOfIterations iterations, and $numOfThreads threads"


# Create the directory if it does not exist
if [ ! -d "resultsGPU/$input_string" ]; then
  mkdir -p "resultsGPU/$input_string"
fi

for ((i=32; i<=$numOfThreads; i<<=1)); do
  threads=$i
  dir="resultsGPU/$input_string/$threads"
  mkdir -p $dir
  for ((j=20; j<=$numberOfExponents; j++)); do
    filePath="$dir/$j.txt"
    for ((k=1; k<=$numOfIterations; k++)); do
      echo "Running $input_string with $j exponents, $k iterations, and $threads threads"
      ./binaries/$input_string $j $threads >> $filePath
    done
    echo "$input_string: Exponent $j/$numberOfExponents with $threads threads completed"
  done
done

