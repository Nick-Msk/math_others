CC=gcc #clang
CFLAGS= -I./ -Wall -Wextra -std=gnu11 -g -O0 -Wno-override-init
# -Wno-initializer-overrides -Wno-unused-function -Wno-format-zero-length
LDFLAGS=
LDLIBS=

all:

checker: checker.c lib/log.o lib/test.o lib/error.o
	$(CC) $(CFLAGS) -DCHECKERTESTING $^ -o test/$@_test

lib/checker.o: checker.c
	$(CC) $(CFLAGS) $^ -c
	@mv $@ lib/

error: error.c lib/log.o lib/common.o lib/test.o
	$(CC) $(CFLAGS) -DERRORTESTING $^ -o test/$@_test

lib/error.o: error.c
	$(CC) $(CFLAGS)  $^ -c
	@mv $@ lib/

log: log.c lib/test.o lib/error.o
	$(CC) $(CFLAGS) -DLOGTESTING $^ -o test/$@_test

lib/log.o:	log.c
	$(CC) $(CFLAGS) $^ -c
	@mv $@ lib/

lib/test.o:	test.c
	$(CC) $(CFLAGS) $^ -c
	@mv $@ lib/

lib/common.o: common.c
	$(CC) $(CFLAGS) $^ -c
	@mv $@ lib/

# to be replaced by metric.c
compl_alg: compl_alg.c
	$(CC) $(CFLAGS) -DCOMPLALGTESTING $^ -o $@_test
	# TODO: replace to dir test/

compl_alg.o: compl_alg.c
	$(CC) $(CFLAGS) $^ -c
	# TODO: replace to lib/ dir

eng_runner.o: eng_runner.c 
	$(CC) $(CFLAGS) $^ -c
	@mv $@ lib/

eng_launcher.o: eng_launcher.c 
	$(CC) $(CFLAGS) $^ -c
	@mv $@ lib/

test: test.c error.o log.o
	$(CC) $(CFLAGS) -DTESTTESTING $^ -o test/$@_test

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

