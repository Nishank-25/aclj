
for n in "tests/test*";
do
	echo " File:"
	cat $n
	echo " Output: \n "
	aclj $n
	echo "\n\n"
done
