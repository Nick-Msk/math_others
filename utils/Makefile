CC=gcc #clang
CFLAGS= -I./ -Wall -Wextra -std=gnu11 -g -O0 -Wno-initializer-overrides
# -Wno-initializer-overrides -Wno-unused-function -Wno-format-zero-length
LDFLAGS=
LDLIBS=

all:

check: check.c log.o test.o error.o
	$(CC) $(CFLAGS) -DCHECKERTESTING $^ -o $@_test

check.o: check.c log.c
	$(CC) $(CFLAGS) $^ -c

error: error.c log.o common.o test.o
	$(CC) $(CFLAGS) -DERRORTESTING $^ -o $@_test

error.o: error.c log.o common.o
	$(CC) $(CFLAGS)  $^ -c

log: log.c test.o error.o
	$(CC) $(CFLAGS) -DLOGTESTING $^ -o test/$@_test

log.o:	log.c
	$(CC) $(CFLAGS) $^ -c
	@mv $@ lib/

common.o: common.c
	$(CC) $(CFLAGS) $^ -c

compl_alg: compl_alg.c
	$(CC) $(CFLAGS) -DCOMPLALGTESTING $^ -o $@_test
	# TODO: replace to dir test/

compl_alg.o: compl_alg.c
	$(CC) $(CFLAGS) $^ -c
	# TODO: replace to lib/ dir

eng_runner.o: eng_runner.c
	$(CC) $(CFLAGS) $^ -c
	@mv $@ lib/

test: test.c error.o log.o
	$(CC) $(CFLAGS) -DTESTTESTING $^ -o $@_test

.PHONY: clean cleanlog cleanall no_targets__ list
clean:
	-rm *.o
	-rm *_test
	-rm -r *.dSYM
	-rm *.gch
	-rm *~
	find . -perm -u=x -type f -ls -maxdepth 1 -exec rm {} \;

cleanlog:
	-rm *.log

cleanall: clean cleanlog

no_targets__:
list:
	sh -c "$(MAKE) -p no_targets__ | awk -F':' '/^[a-zA-Z0-9][^\$$#\/\\t=]*:([^=]|$$)/ {split(\$$1,A,/ /);for(i in A)print A[i]}' | grep -v '__\$$' | sort"

