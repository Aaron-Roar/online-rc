#!/bin/sh
for directory in $(ls . | sed s/compile.sh//)
do

    echo -e "Compiling tests in $directory directory\n\n"
    for file in $(ls $directory/ | grep ".cpp")
    do
        binary=$(echo $file | sed s/.cpp//)
        echo -e "  [*]g++ -g $directory/$file -o $directory/$binary\n"
        g++ -g $directory/$file -o $directory/$binary
        echo -e "\n"
    done
    echo -e "Finished compiling $directory\n--------------------------------------------------------------------------------------------\n\n"

done
