#
#OUTFILE=inode
#
#inode: inode.c inode_414.h
#	gcc inode.c -o $(OUTFILE)
#
#clean:
#	rm $(OUTFILE)

vars :OUTFILE => "inode"

rule "inode" do
    depend "inode.c", "inode_414.h"
    compile "inode.c", :output => :OUTFILE
end

clean "rm", :OUTFILE
