TESTS="tests/*"

for n in $TESTS;
do
	printf "*************************************************\n"
	printf "{$n}\n"
	cat  "$n"
	printf "\n"
	printf "Output: \n"
	./aclj $n "$@"
	printf "*************************************************\n\n"
done
