#
#CC=gcc
#
#OUTFILE=slash
#
#OFILES=exit_shell.o error.o init.o file.o options.o eval.o help.o aliases.o environ.o redirect.o globbing.o builtins.o parse.o prompt.o shell.o
#
#.SUFFIXES: .o .c
#
#slash:	$(OFILES)
#	$(CC) $(OFILES) -o $(OUTFILE)
#
#unixshell:	$(OFILES)
#	$(CC) $(OFILES) -o unixshell
#
#clean:
#	rm -f *.o
#
#exit_shell.o: exit_shell.c exit_shell.h
#error.o: error.c error.h
#init.o: init.c init.h
#file.o: file.c file.h
#options.o: options.c options.h
#eval.o: eval.c eval.h
#help.o: help.c help.h
#aliases.o: aliases.c aliases.h
#environ.o: environ.c environ.h
#redirect.o: redirect.c redirect.h
#globbing.o: globbing.c globbing.h
#builtins.o: builtins.c builtins.h
#parse.o: parse.c parse.h
#prompt.o: prompt.c prompt.h
#shell.o: shell.c prompt.c prompt.h parse.c parse.h builtins.c builtins.h globbing.c globbing.o redirect.c redirect.h environ.c environ.h aliases.c aliases.h help.c help.h eval.c eval.h options.c options.h file.c file.h init.c init.h error.c error.h exit_shell.c exit_shell.h
#
# Working on the Ruby version...

vars :OUTFILE => "slash", 
     :OFILES => Dir['*.c'].collect { |f| f.rpartition('.').first + '.o' }

# Damn suffix rule again

rule :slash, :depend => :OFILES do
    compile :output => :OUTFILE
end

rule :unixshell, :depend => :OFILES do
    compile :output => "unixshell"
end

clean '*.o'

# .....
