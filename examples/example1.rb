# To generate this Makefile:
#
#CC = gcc
#FLAGS = -g
#
#hello: hello.o
#	$(CC) $(FLAGS) hello.o -o hello
#	@echo "Done compiling, run ./hello"
#    
#hello.o: hello.c hello.h
#	$(CC) $(FLAGS) hello.c -c
#
#clean:
#	-rm hello *.o;
#
# Use this Ruby:

vars :FLAGS => "-g"

rule "hello", :depends => "hello.o" do
    compile :output => "hello"
    echo "Done compiling, run ./hello"
end

rule "hello.o", :d => ["hello.c", "hello.h"] do
    compile :to_obj
end

clean 'hello', '*.o'
