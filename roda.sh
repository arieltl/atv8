#!/bin/bash

# Check if exactly two arguments are provided
if [ $# -ne 2 ]; then
    echo "Usage: $0 <executable> <numeric_suffix>"
    exit 1
fi

# Assign parameters to variables for better readability
executable=$1
suffix=$2
input_file="entrada${suffix}.txt"

# Capitalize the first letter of the executable for the filename
capitalized_executable="$(tr '[:lower:]' '[:upper:]' <<< ${executable:0:1})${executable:1}"

# Check if the input file exists
if [ ! -f "$input_file" ]; then
    echo "Error: Input file $input_file does not exist."
    exit 1
fi

# Loop to run the executable 15 times with input redirection
for i in {1..15}; do
    ./"$executable" < "$input_file"
done

# Construct file names for the renaming process
old_filename="resultados${capitalized_executable}.txt"
new_filename="resultados${capitalized_executable}${suffix}.txt"

# Check if the old file exists before attempting to rename
if [ -f "$old_filename" ]; then
    mv "$old_filename" "$new_filename"
else
    echo "Error: File $old_filename does not exist."
    exit 1
fi
