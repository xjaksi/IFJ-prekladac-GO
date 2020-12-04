#!/bin/bash

FAIL='\033[38;5;45m'
PASS='\033[38;5;213m'
NC='\033[0m'
BG='\033[48;5;241m'

FAILED_TESTS=""

TOTAL_PASSED=0
TOTAL_FAILED=0
TOTAL_TESTS=0

function result() {
    file=$1
    ret=$2
    exp=$3

    if [ $ret -eq $exp ]
    then
        printf "[${PASS}OK${NC}] TEST: $file \n"
        printf "     Expected: $exp Returned: $ret \n"
        ((TOTAL_PASSED++))

    else
        printf "[${FAIL}FAILED${NC}] TEST: $file \n"
        printf "         Expected: $exp Returned: $ret \n"
        ((TOTAL_FAILED++))
        FAILED_TESTS+="\t $file \n"
    fi
    ((TOTAL_TESTS++))
}

function syntax() {
    # SYNTAX TESTS
    printf "== RUNNING SYNTAX TESTS ============\n"
    for f in ./tests/syntax/*.err
    do
        timeout 2s ./ifj20 <$f  2>/dev/null
        out=$?
        result "$(basename $f)" "$out" "2"
    printf "\n"
    done

    for f in ./tests/syntax/*.ok
    do
        timeout 2s ./ifj20 <$f  2>/dev/null     
        out=$?
        result "$(basename $f)" "$out" "0"
        printf "\n"
    done
}

function lexical() {
    # LEXICAL TESTS
    printf "== RUNNING LEXICAL TESTS ===========\n"
    for f in ./tests/lexical/*.err
    do
        timeout 2s ./ifj20 <$f  2>/dev/null
        out=$?
        result "$(basename $f)" "$out" "1"
        printf "\n"
    done

    for f in ./tests/lexical/*.ok
    do
        timeout 2s ./ifj20 <$f  2>/dev/null     
        out=$?
        result "$(basename $f)" "$out" "0"
        printf "\n"
    done
}

function semantics() {
    # LEXICAL TESTS
    printf "== RUNNING SEMANTICS TESTS =========\n"
    for d in ./tests/semantics/*/
    do
        for f in $d*.err
        do
            filetitle=$(basename $f)

            if [ "$filetitle" = "*.err" ]
            then   
                continue
            fi

            timeout 2s ./ifj20 <$f  2>/dev/null
            out=$?
            result "$filetitle" "$out" "2"
        printf "\n"
        done

        for f in $d*.ok
        do
            filetitle=$(basename $f)
            printf "$filetitle \n"
            if [ "$filetitle" = "*.ok" ]
            then   
                break
            else
                timeout 2s ./ifj20 <$f  2>/dev/null     
                out=$?
                result "$filetitle" "$out" "0"
                printf "\n"
            fi
        done
    done
}

# PREPARE TERMINAL
make clean 
make
clear

key="$1"

case $key in
    -l)
        lexical
        ;;

    -s)
        semantics
        ;;

    -x)
        syntax
        ;;

    -a|*)    
        lexical
        semantics
        syntax
        ;;
esac

# FINAL PRINTING 
printf "\n"
echo "======== FINAL TEST RESULTS ========"
printf " [${PASS}PASSED${NC}]:  $TOTAL_PASSED\n"
printf " [${FAIL}FAILED${NC}]:  $TOTAL_FAILED\n"
printf " [OUT OF]:  $TOTAL_TESTS\n"

if [ $TOTAL_FAILED -ne 0 ]
then
    echo "===================================="
    printf "   tests failed:\n"
    for t in $FAILED_TESTS
    do 
        printf "$t"
    done
fi

echo "===================================="

