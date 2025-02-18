#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Error: missing parameters."
    echo "Usage: $0 <writefile> <writerstr>"
    exit 1
fi 

writefile=$1
writestr=$2

dirpath=$(dirname "$writefile")
if [ ! -d "$dirpath" ]
then
    echo "Directory does not exist, creating: $dirpath"
    mkdir -p "$dirpath"
    if [ $? -ne 0 ]
    then
        echo "Error: Could not create directory $dirpath."
        exit 1
    fi
fi

echo "$writestr" > "$writefile"
if [ $? -ne 0 ]
then
    echo "Error: Could not create or write to file $writefile."
    exit 1
fi

echo "Successfully wrote to $writefile"
