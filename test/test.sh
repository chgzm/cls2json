#!/bin/bash

function error() {
    echo -e "\e[31;1m${1}\e[m"
}

function success() {
    echo -e "\e[32;1m${1}\e[m"
}


declare -A class_answer_map

class_answer_map["./java/Hello.class"]="hello_answer.json"
class_answer_map["./java/Test.class"]="test_answer.json"

RET=0
for key in ${!class_answer_map[@]}
do
    answer="${class_answer_map[${key}]}"
    ../cls2json ${key} > testfile.json
    diff testfile.json ${answer} > diff.txt
    if [[ -s diff.txt ]]; then
        error "Creating json from ${key} failed."
        RET=1
    else    
        success "Creating json from ${key} succeeded."
    fi

    rm testfile.json diff.txt
done

exit ${RET}
