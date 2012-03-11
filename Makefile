

default: test

bham-test: bham.h

test: bham-test
	./bham-test | diff -sq - bham-test-out.txt

.PHONY: test
