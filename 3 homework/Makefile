CC = gcc
headers := string_builder.h text_redactor.h
objs := main.o string_builder.o text_redactor.o
valgrindFlagsBuild = -std=c11 -Wall -ggdb3
valgrindFlags = --leak-check=full --track-origins=yes
depugFlags = -O0
releaseFlags = -O3

all: depug

valgrindBuild: $(objs)
	$(CC) -o valgrindBuild $(valgrindFlagsBuild) $(objs)

valgrind:
	make valgrindBuild
	valgrind  $(valgrindFlags) ./valgrindBuild

release: $(objs)
	$(CC) -o homework3 $(releaseFlags) $(objs)

depug: $(objs)
	$(CC) -o homework3 $(depugFlags) $(objs)

homework3: $(objs)
	$(CC) -o homework3 $(objs)

main.o: main.c $(headers)
	$(CC) -c main.c

text_redactor.o: text_redactor.c $(headers)
	$(CC) -c text_redactor.c

string_builder.o: string_builder.c string_builder.h
	$(CC) -c string_builder.c

.PHONY : clean

clean:
	@rm -f $(objs) homework3 valgrindBuild