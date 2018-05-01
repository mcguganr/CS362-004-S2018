CFLAGS = -Wall -fpic -coverage -lm

testme: testme.c
	gcc -o testme testme.c -g $(CFLAGS)

runtest: testme
	./testme

clean:
	rm -f *.o *.gcov *.gcda *.gcno *.so testme testme.exe
