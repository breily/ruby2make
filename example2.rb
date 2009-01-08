# To generate this Makefile:
#
#CC = gcc
#OFILES = uthread.o test_threads.o
#OUTFILE = threadpkg
#FLAGS = -D__MAKECONTEXT_V2_SOURCE -Wall
#
#.SUFFIXES: .o .c
#
#all:	$(OFILES)
#	$(CC) $(FLAGS) $(OFILES) -o $(OUTFILE)
#
#threadpkg: $(OFILES)
#   $(CC) $(FLAGS) $(OFILES) -o $(OUTFILE)
#
#library: 	
#	$(CC) $(FLAGS) -c uthread.c -o uthread.o
#
#clean:
#	rm -f *.o *~
#	rm -f $(OUTFILE)
#
#uthread.o: uthread.c uthread.h
#main.o: main.c uthread.c uthread.h
#
# Work in progress...

vars :OFILES => "uthread.o test_threads.o", :OUTFILE => "threadpkg", 
     :FLAGS => "-D__MAKECONTEXT_V2_SOURCE -Wall"

# Add suffix rule

rule "all", :depends => :OFILES do
    compile :output => :OUTFILE
end

rule "threadpkg", :depends => :OFILES do
    compile :output => :OUTFILE
end

rule "library" do
    compile :to_obj, :output => "uthread.o", :input => "uthread.c"
end

clean '*.o', :OUTFILE

rule "uthread.o", :depends => ["uthread.c", "uthread.h"]
rule "main.o", :depends => ["main.c", "uthread.c", "uthread.h"]
