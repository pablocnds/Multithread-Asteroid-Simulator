#!/bin/bash

declare -a tests=("10 1000 10 999"    # No bounces, no mb
		  "50 100 10 333"     # Bounces, but only 2 at a time, no mb
		  "100 250 10 7777"   # 3+ simultaneous bounces happen, no mb
		  "50 100 10 1942"    # Mb, no bounces
		  "20 300 100 666"    # Heavy planet influence
		  "200 200 100 1999"  # Heavy test (everything)
		  )
num_tests=${#tests[@]}
arg=""

if [ $# -eq 3 ]
then
	echo ""
elif [ $# -eq 4 ]
then
	if [ "$4" == "perf" ]
	then
		echo "ATENCIÓN: la funcionalidad perf requiere permisos de seguridad"
		arg="perf stat "
	else
		echo "Formato no implementado"
	fi
else
	echo "Wrong arguments. $0 main_src nast_src test_binary [perf | <num_rnd_tests>]"
	exit -1
fi

rm -r ./testfolder/
mkdir ./testfolder/
cd ./testfolder/
mkdir ./testerBin/

g++ -fopenmp -std=c++14 -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors ../$1 ../$2

for (( i=0; i<${num_tests}; i++ ));
do
	echo "---------------------------------------------------------"

	${arg}./a.out ${tests[i]}
	cd ./testerBin/	
	../../$3 ${tests[i]} >/dev/null
	cd ..

	echo ""
	echo "TEST ${i}: ${tests[i]}"
	../comparator out.txt ./testerBin/out.txt
	echo ""
done
