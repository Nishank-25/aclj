TESTS="tests/*"

for n in $TESTS;
do
	printf "*************************************************\n"
	printf "{$n}\n"
	cat  "$n"
	printf "\n"
	./aclj $n "$@"
	printf "*************************************************\n\n"
done
