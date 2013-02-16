This is a set of basic data structures written in pure C.
Of course, there is already tons of C libraries, but this
code is written just for myself. If you want to use it -
you are welcome.

Currently, it includes such data structures:
 - Linked list.
 - List (aka vector in C++).
 - Hash map.
 - Heap.
 - Graph.
 - Union-find.

The repository structure goes as follows:
 - /makeScripts - contains all needed makeScripts. The most
handy one is 'makeAll.sh' - it makes all files:) You can
also use separate make scripts (e.g. 'makeHeap.sh') to
'build' separate data structures - but some of them depend
on other data structures.
 - /objectFiles - contains object files:). All the make scripts
put their resulting object files and search for the input
object files here.
 - /testPrograms - make scripts also 'build' test programs
for each data structure and put the executables here. They
come in 2 flavors: with debug information (these have the 
'.og' extension) - for running under gdb, and without it
(these don't have any extension).
 - All other folders contain source files for corresponding
data structure and its test program.
