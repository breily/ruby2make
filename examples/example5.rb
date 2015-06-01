# To generate this suffix rule:
#
#LEMON = /usr/bin/lemon
#
#.SUFFIXES: .o .m
#
#.m.o:
#     $(CC) -c $(FLAGS) $<
#
#grammar:
#     $(LEMON) $(FLAGS) gram.y
#
# Use this Ruby:

vars :LEMON => "/usr/bin/lemon"

suffix ".o", ".m", "$(CC) -c $(FLAGS) $<"

rule "grammar" do 
    compile :compiler => :LEMON, :input => "gram.y"
end
