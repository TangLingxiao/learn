#!/bin/bash

demoFun(){
    echo "this is my first shell function"
}

funwithReturn(){
    echo add function
    echo input the first num:
    read a
    echo input the second num:
    read b
    printf "the two nums are: %d, %d\n" $a $b
    printf "%s" "the result is :"
    var=$[a+b]
    return $var #$((a+b))
}
echo "--function start--"
funwithReturn
printf "%d\n" $?
echo "--function end--"

