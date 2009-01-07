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

rule "hello" do
    depend "hello.o"
    compile :input => "hello.o", :output => "hello"
end

rule "hello.o" do
    depend "hello.cc", "hello.h"
    compile "hello.cc"
end

rule "clean" do
    command "rm hello *.o"
end

