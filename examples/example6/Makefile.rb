# Makefile:
#
# CC=g++
# FLAGS=-g -DLINUX
# LIBS=-lpthread
#
# philosophers: philosophers.o
#     $(CC) $(FLAGS) philosophers.o $(LIBS) -o philosophers
#
# philosophers.o: philosophers.h philosophers.cpp
#     $(CC) $(FLAGS) philosophers.cpp -c -o philosophers.o
#
# philosophers_stats: philosophers_stats.cpp
#     $(CC) $(FLAGS) philosophers_stats.cpp $(LIBS) -o philosophers_stats
#
# clean:
#     rm -f *.o philosophers philosophers_stats
#
# Ruby:

vars :CC => "g++", :FLAGS => "-g -DLINUX", :LIBS => "-lpthread"

rule "philosophers", :d => "philosophers.o" do
    compile :LIBS
end

rule "philosophers.o", :d => ["philosophers.h", "philosophers.cpp"] do
    compile :to_obj
end

rule "philosophers_stats", :d => "philosophers_stats.cpp" do
    compile :LIBS
end

clean "*.o", "philosophers", "philosophers_stats"
