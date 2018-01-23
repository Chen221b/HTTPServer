#!/bin/bash 

echo Damon
declare -A ass_array
ass_array=([damon]=da [stefan]=stef)
echo ${!ass_array[*]}
test()
{
	echo "in function"
	printf "1st prar %15s\n" $1
}
test vicky
read -s -n 3 var
echo $var
var="csv1,csv2,csv3,csv4"
oldIFS=$IFS
IFS=,
for item in $var
do
	echo Item: $item
done
IFS=$oldIFS
