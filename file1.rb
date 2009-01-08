#
#CC=g++
#FLAGS=-g #-O2
#
#hello: hello.o
#	$(CC) $(FLAGS) hello.o -o hello
#	@echo "Done compiling, run ./hello"
#    
#hello.o: hello.cc hello.h
#	$(CC) $(FLAGS) hello.cc -c
#
#clean:
#	rm hello *.o;

vars :CC => "g++", :FLAGS => "-g"

rule "hello", :depend => "hello.o" do
    compile :output => "hello"
    echo "done compiling, run ./hello"
end

rule "hello.o" do
    depend "hello.c", "hello.h"
    compile :to_obj, :output => "hello.o"
end

clean "hello *.o"
