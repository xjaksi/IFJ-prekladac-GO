#!/bin/bash

FAIL='\033[38;5;45m'
PASS='\033[38;5;213m'
NC='\033[0m'
BG='\033[48;5;241m'

FAILED_TESTS=""

FAILONLY=0
TOTAL_PASSED=0
TOTAL_FAILED=0
TOTAL_TESTS=0
LEXICAL_FAIL=0

LEXNO=0

key="$1"
flag="$2"

function result() {
    file=$1
    ret=$2
    exp=$3

    if [ $ret -eq $exp ]
    then
        if [ $FAILONLY -eq 1 ]
        then
            ((TOTAL_PASSED++))
        else
            printf "[${PASS}OK${NC}] TEST: $file \n"
            printf "     Expected: $exp Returned: $ret \n"
            printf "\n"
            ((TOTAL_PASSED++))
        fi

    else
        if [ $ret -eq 1 ] 
        then
            ((LEXICAL_FAIL++))
            ((TOTAL_FAILED++))
            # FAILED_TESTS+="\t $file \n"
        elif [ $exp -eq 1 ] && [ $ret -ne 1 ]
        then 
            ((LEXICAL_FAIL++))
            ((TOTAL_FAILED++))
        else
            ((TOTAL_FAILED++))
        fi

        printf "[${FAIL}FAILED${NC}] TEST: $file \n"
        printf "         Expected: $exp Returned: $ret \n"
        # FAILED_TESTS+="\t $file \n"
        printf "\n"
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
    done

    for f in ./tests/syntax/*.ok
    do
        timeout 2s ./ifj20 <$f  2>/dev/null     
        out=$?
        result "$(basename $f)" "$out" "0"
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
    done

    for f in ./tests/lexical/*.ok
    do
        timeout 2s ./ifj20 <$f  2>/dev/null     
        out=$?
        result "$(basename $f)" "$out" "0"
    done
}

function semantics() {
    # LEXICAL TESTS
    printf "== RUNNING SEMANTICS TESTS =========\n"
    for d in ./tests/semantics/*/
    do
        erType=$( basename $d)

        for f in $d*.err
        do
            filetitle=$(basename $f)

            if [ "$filetitle" = "*.err" ]
            then   
                continue
            fi

            timeout 2s ./ifj20 <$f  2>/dev/null
            out=$?
            result "$filetitle" "$out" "$erType"
        done

        for f in $d*.ok
        do
            filetitle=$(basename $f)
            if [ "$filetitle" = "*.ok" ]
            then   
                continue
            else
                timeout 2s ./ifj20 <$f  2>/dev/null     
                out=$?
                result "$filetitle" "$out" "0"
            fi
        done
    done
}

# PREPARE TERMINAL
make clean 
make
clear

while getopts "flsxa" arg; do
  case $arg in
    f)
        FAILONLY=1 
        ;;
    l)
        lexical
        ;;
    s)
        semantics
        ;;
    x) 
        syntax
        ;;

    a|*)
        lexical
        semantics
        syntax
        ;;
  esac
done

# FINAL PRINTING 
printf "\n"
echo "======== FINAL TEST RESULTS ========"
printf " [${PASS} PASSED ${NC}]:  $TOTAL_PASSED\n"
printf " [${FAIL} FAILED ${NC}]:  $TOTAL_FAILED\n"
printf " [ OUT OF ]:  $TOTAL_TESTS\n"
echo "===================================="
printf " Lexical fails: $LEXICAL_FAIL\n"


#if [ $TOTAL_FAILED -ne 0 ]
#then
#    echo "===================================="
#    printf "   tests failed:\n"
#    for t in $FAILED_TESTS
#    do 
#        printf "$t"
#    done
#fi

echo "===================================="

