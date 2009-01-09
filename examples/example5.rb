# To generate this suffix rule:
#
#.SUFFIXES: .o .m
#
#.m.o:
#     $(CC) -c $(FLAGS) $<
#
# Currently, use this Ruby:

suffix ".o", ".m", "$(CC) -c $(FLAGS) $<"
