#!/bin/bash
SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

path_to_grep="src/"
path_to_include="include/"
path_to_test="tests/test_files/"

s21_command=(
    "./s21_grep"
)
sys_command=(
    "grep"
)

flags=(
    "v"
    "c"
    "l"
    "n"
    "h"
    "o"
)

tests=(
"s ${path_to_test}test_0_grep.txt FLAGS"
"for ${path_to_grep}grep.c ${path_to_include}grep.h Makefile FLAGS"
"for ${path_to_grep}grep.c FLAGS"
"-e for -e ^int ${path_to_grep}grep.c ${path_to_include}grep.h Makefile FLAGS"
"-e for -e ^int ${path_to_grep}grep.c FLAGS"
"-e regex -e ^print ${path_to_grep}grep.c FLAGS -f ${path_to_test}test_ptrn_grep.txt"
"-e while -e void ${path_to_grep}grep.c Makefile FLAGS -f ${path_to_test}test_ptrn_grep.txt"
"-e intel -e int FLAGS ${path_to_test}test_7_grep.txt"
"-e int -e intel FLAGS ${path_to_test}test_7_grep.txt"
)


manual=(
"-n for ${path_to_test}test_1_grep.txt ${path_to_test}test_2_grep.txt"
"-n for ${path_to_test}test_1_grep.txt"
"-n -e ^\} ${path_to_test}test_1_grep.txt"
"-c -e / ${path_to_test}test_1_grep.txt"
"-ce ^int ${path_to_test}test_1_grep.txt ${path_to_test}test_2_grep.txt"
"-e ^int ${path_to_test}test_1_grep.txt"
"-nivh = ${path_to_test}test_1_grep.txt ${path_to_test}test_2_grep.txt"
"-e"
"-ie INT ${path_to_test}test_5_grep.txt"
"-echar ${path_to_test}test_1_grep.txt ${path_to_test}test_2_grep.txt"
"-ne = -e out ${path_to_test}test_5_grep.txt"
"-iv int ${path_to_test}test_5_grep.txt"
"-in int ${path_to_test}test_5_grep.txt"
"-c -l aboba ${path_to_test}test_1_grep.txt ${path_to_test}test_5_grep.txt"
"-v ${path_to_test}test_1_grep.txt -e ank"
"-noe ) ${path_to_test}test_5_grep.txt"
"-l for ${path_to_test}test_1_grep.txt ${path_to_test}test_2_grep.txt"
"-o -e int ${path_to_test}test_4_grep.txt"
"-e = -e out ${path_to_test}test_5_grep.txt"
"-noe ing -e as -e the -e not -e is ${path_to_test}test_6_grep.txt"
"-e ing -e as -e the -e not -e is ${path_to_test}test_6_grep.txt"
"-c -e . ${path_to_test}test_1_grep.txt -e '.'"
"-l for no_file.txt ${path_to_test}test_2_grep.txt"
"-e int -si no_file.txt ../src/grep.c no_file2.txt ../include/grep.h"
"-si ../src/grep.c -f no_pattern.txt"
"-f ${path_to_test}test_3_grep.txt ${path_to_test}test_5_grep.txt"
)


run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param > "${s21_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log
    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${s21_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - Success $param"
    else
        let "FAIL++"
        echo "$COUNTER - Fail $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

echo "^^^^^^^^^^^^^^^^^^^^^^^"
echo "TESTS WITH NORMAL FLAGS"
echo "^^^^^^^^^^^^^^^^^^^^^^^"
printf "\n"
echo "#######################"
echo "MANUAL TESTS"
echo "#######################"
printf "\n"

for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done

printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "1 PARAMETER"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done
printf "\n"
echo "======================="
echo "2 PARAMETERS"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                run_test "$i"
            done
        fi
    done
done
printf "\n"
echo "======================="
echo "3 PARAMETERS"
echo "======================="
printf "\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    run_test "$i"
                done
            fi
        done
    done
done
printf "\n"
echo "======================="
echo "4 PARAMETERS"
echo "======================="
printf "\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            for var4 in "${flags[@]}"
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="-$var1 -$var2 -$var3 -$var4"
                        run_test "$i"
                    done
                fi
            done
        done
    done
done
printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "DOUBLE PARAMETER"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                run_test "$i"
            done
        fi
    done
done

printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "TRIPLE PARAMETER"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    run_test "$i"
                done
            fi
        done
    done
done
printf "\n"
echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
echo "ALL: $COUNTER"
printf "\n"
##############################
