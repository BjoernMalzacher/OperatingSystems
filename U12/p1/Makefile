CC=     gcc
testlib=../testlib
CFLAGS= -g -W -Wall -Werror -std=c99 -D_DEFAULT_SOURCE -I$(testlib) -I.
LDFLAGS=-pthread
TARGET= fssize
SRC=    fssize.c testlib.c
VPATH=  $(testlib) tests
TESTS=  $(basename $(notdir $(wildcard tests/*.c)))

OBJ=      $(SRC:.c=.o)
OBJ-sol=  $(subst $(TARGET),$(TARGET)-solution,$(OBJ))
TESTS-sol=$(addsuffix -sol,$(TESTS))
testnames=$(1) $(addsuffix -sol,$(1))

TESTDATA= testdir/hardlink

all: $(TARGET) $(TESTS) $(TESTDATA)

solution: $(TARGET)-sol $(TESTS-sol) $(TESTDATA)

run-tests: $(TESTS) $(TESTDATA)
	@bash $(testlib)/testrunner $(addprefix ./,$(TESTS))

run-tests-solution: $(TESTS-sol) $(TESTDATA)
	@bash $(testlib)/testrunner $(addprefix ./,$(TESTS-sol))

$(call testnames,test_list_dir): LDFLAGS+=-Wl,--wrap=closedir -Wl,--wrap=opendir
$(call testnames,test_list_dir_error): LDFLAGS+=-Wl,--wrap=exit -Wl,--wrap=opendir
$(call testnames,test_process_file_sync): LDFLAGS+=-Wl,--wrap=pthread_mutex_unlock -Wl,--wrap=pthread_mutex_lock
$(call testnames,test_get_filesystem_size_init): LDFLAGS+=-Wl,--wrap=pthread_create -Wl,--wrap=hcreate -Wl,--wrap=sem_init -Wl,--wrap=pthread_mutex_init
$(call testnames,test_get_filesystem_size_free): LDFLAGS+=-Wl,--wrap=pthread_mutex_destroy -Wl,--wrap=pthread_join -Wl,--wrap=hdestroy -Wl,--wrap=sem_destroy

testdir/hardlink:
	ln -f testdir/a.txt $@

$(TARGET): main.o $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(TESTS): %: %.o $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(TARGET)-sol: main.o $(OBJ-sol)
	$(CC) -o $@ $^ $(LDFLAGS)

$(TESTS-sol): %-sol: %.o $(OBJ-sol)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(TESTS) $(TESTS-sol) $(TARGET)-sol *.o $(TESTDATA)

.PHONY: all solution run-tests run-tests-solution clean
