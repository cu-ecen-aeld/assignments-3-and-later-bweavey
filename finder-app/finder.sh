#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Error: missing parameters."
    echo "Usage: $0 <filesdir> <searchstr>"
    exit 1
fi 

filesdir=$1
searchstr=$2

if [ ! -d "$filesdir" ]
then
    echo "Error: $filesdir is not a valid directory."
    exit 1
fi

file_count=0
matching_lines=0

for file in $(find "$filesdir" -type f)
do
    ((file_count++))
    matching_in_file=$(grep -c "$searchstr" "$file")
    ((matching_lines+=matching_in_file))
done

echo "The number of files are $file_count and the number of matching lines are $matching_lines."
