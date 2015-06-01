# To generate this Makefile:
#
#CC=gcc
#OUTFILE=inode
#
#inode: inode.c inode_414.h
#	$(CC) inode.c -o $(OUTFILE)
#
#clean:
#	rm $(OUTFILE)
#
# Use this Ruby:

vars :OUTFILE => "inode"

rule "inode", :depends => ["inode.c", "inode_414.h"] do
    compile :output => :OUTFILE
end

clean :OUTFILE
