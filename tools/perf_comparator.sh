#!/bin/bash

declare -a tests=(#"10 1000 10 999"    # No bounces, no mb
		  #"50 100 10 333"     # Bounces, but only 2 at a time, no mb
		  #"100 250 10 7777"   # 3+ simultaneous bounces happen, no mb
		  #"50 100 10 1942"    # Mb, no bounces
		  #"20 300 100 666"    # Heavy planet influence
		  #"10 200 10 54321"   # Minimun sample
		  "50 200 10 10010"   # Short sample
		  "250 200 50 9876"   # Medium sample
		  #"500 200 200 123"   # Long sample
		  "500 50 200 4917"   # Long sample, low iterations
		  #"1000 100 200 88"   # Huge sample
		  #"200 200 100 1999"  # Heavy test (everything)
		  )
num_tests=${#tests[@]}


if [ $# -ne 5 ]
then
	echo "Wrong arguments. $0 main_src_SEQ nast_src_SEQ main_src_PAR nast_src_PAR test_binary"
	exit -1
fi

rm -r ./perffolder/
mkdir ./perffolder/
cd ./perffolder/
mkdir ./pSeq/
mkdir ./pPar/

cd ./pSeq/
g++ -fopenmp -std=c++14 -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -O3 -DNDEBUG ../../$1 ../../$2

cd ../pPar/
g++ -fopenmp -std=c++14 -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -O3 -DNDEBUG ../../$3 ../../$4

cd ..

for (( i=0; i<${num_tests}; i++ ));
do
	echo "---------------------------------------------------------"

	echo ""
	echo "TEST ${i}: ${tests[i]}"
	echo ""

	for (( j=0; j<10; j++ ))
	do
		DIFStru="$(./pSeq/a.out ${tests[i]})"
		DIFPtru="$(./pPar/a.out ${tests[i]})"

		Stru=$(echo "$DIFStru / $DIFPtru" | bc -l)

		echo "SEQ: ${DIFStru:0:6}ms | PAR: ${DIFPtru:0:6}ms [ ${Stru:0:5} ]"
		echo ""
	done
done
