#!/bin/bash

FC=0
SC=0
WA=0
TLE=0
TIME_BIN=/usr/bin/time

LIMIT=${3}

if [ -z "$LIMIT" ]; then
    LIMIT=300
fi

# Iterate over the tests
for x in `find tests/${2} -name '*.in' | sort -V`
do
    echo -n "$x ... "

    # Evaluate the program and calculae time used
    {
        IFS= read -rd '' TIME
        IFS= read -rd '' OUTPUT
        IFS= read -rd '' status
    } < <({ out=$($TIME_BIN -f "%e" ./$1 < $x); } 2>&1; printf '\0%s' "$out" "$?")

    LIMIT_ST=`echo $TIME ">" $LIMIT | bc`

    OUTPUT_DIFF=`diff -B -Z <(echo "$OUTPUT") "${x%.in}.out" | head -n 20`
    if [ $LIMIT_ST -eq 1 ]; then
        echo -e "\e[0;33mFAILED - TIME LIMIT EXCEEDED $(echo "$TIME") \e[0m"
        FC=$((FC+1))
        TLE=$((TLE+1))
    elif [ -z "$OUTPUT_DIFF" ]; then
        echo -e "\e[32mOK\e[0m $(echo "$TIME")"
        SC=$((SC+1))
    else
        echo -e "\e[31mFAILED - WRONG ANSWER\n$OUTPUT_DIFF\e[0m"
        FC=$((FC+1))
        WA=$((WA+1))
    fi
done

echo -e "\n\e[32mSUCCESSFUL $SC\e[0m, \e[31mFAILED $FC\e[0m = \e[0;31mWA=$WA\e[0m + \e[0;33mTLE=$TLE\e[0m "
