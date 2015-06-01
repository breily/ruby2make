# To make this Makefile:
#
#CC=gcc
#
#OUTFILE=slash
#
#OFILES=exit_shell.o error.o init.o file.o options.o eval.o help.o aliases.o environ.o redirect.o globbing.o builtins.o parse.o prompt.o shell.o
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
# Use this Ruby:

vars :OUTFILE => "slash", 
     :OFILES => Dir['*.c'].collect { |f| f.rpartition('.').first + '.o' }

rule :slash, :depend => :OFILES do
    compile :output => :OUTFILE
end

rule :unixshell, :depend => :OFILES do
    compile :output => "unixshell"
end

clean '*.o', 'slash'
