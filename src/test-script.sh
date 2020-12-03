#!/bin/bash

RED='\033[1;31m'
GREEN='\033[1;32m'
NC='\033[0m'

for f in ./tests/syntax/*.err
do
    echo "---------------------------------"
    timeout 2s ./ifj20 <$f  2>/dev/null
    RET=$?

    if [ $RET -eq 2 ]
    then
        printf "[${GREEN}OK${NC}] TEST: $f \n"
        printf "     Expected: 2 Returned: $RET \n"
    else
        printf "[${RED}FAILED${NC}] TEST: $f \n"
        printf "         Expected: 2 Returned: $RET \n"
    fi
done

for f in ./tests/syntax/*.ok
do
    echo "---------------------------------"
    timeout 2s ./ifj20 <$f  2>/dev/null
    RET=$?

    if [ $RET -eq 0 ]
    then
        printf "[${GREEN}OK${NC}] TEST: $f \n"
        printf "     Expected: 2 Returned: $RET \n"
    else
        printf "[${RED}FAILED${NC}] TEST: $f \n"
        printf "         Expected: 2 Returned: $RET \n"
    fi
done