#
# wordcount by Jeff Preshing 2011 Jun 03
#  http://preshing.com/20110603/hash-table-performance-tests
#
# Rocco Carbone <rocco@tecsiel.it>
#  porting to Linux
#    Sat Jul 18 11:12:30 CEST 2015 first
#    Sun Jul 31 21:55:30 CEST 2016 revisited
#
# Required packages installed on your system:
#  python
#  Judy
#  Google sparsehash

# Warning: the following files do not compile yet
#   stdexthashmaphashtabletester.cpp
#   stdexthashmaptester.cpp
#   precomp.cpp

# The name of the game
TARGET    = wordcount

# C++ source files
CSRCS     = dlmalloc.c
C++SRCS   = wordcount.cpp                 \
            chainedhashtabletester.cpp    \
            gcchashtabletester.cpp        \
            googledensehashmaptester.cpp  \
            googlesparsehashmaptester.cpp \
            judyhashtabletester.cpp       \
            judyslarraytabletester.cpp    \
            openhashtabletester.cpp       \
            pythondictionarytester.cpp    \
            stdmaphashtabletester.cpp     \
            stdmaptester.cpp              \
            stringhashtabletester.cpp

# gcc
GCCDIR    = gcc-latest
INCLUDE  += -I${GCCDIR}
LIBGCC    = ${GCCDIR}/libgcc-hash.a
USRLIBS  += ${LIBGCC}

# Object and depend files
OBJS      = $(patsubst %.c,%.o,   ${CSRCS})
OBJS     += $(patsubst %.cpp,%.o, ${C++SRCS})
DEPS      = $(patsubst %.c,%.M,   ${CSRCS})
DEPS     += $(patsubst %.cpp,%.M, ${C++SRCS})

# C compiler and flags
DEFINES   = -DUSE_DLMALLOC -DREAD_WHOLE_FILE
INCLUDES  = -I. ${INCLUDE}
CC        = gcc
C++       = g++
AR        = ar
LD        = gcc
COMMON    = -g -O3 ${DEFINES} ${INCLUDES} -fno-diagnostics-color
CFLAGS   += ${COMMON} -Wno-implicit-function-declaration
C++FLAGS += ${COMMON} -std=gnu++0x
C++FLAGS += `pkg-config --cflags python`
SHFLAGS   = -shared
ARFLAGS   = rv
LDFLAGS   =

# System libraries
SYSLIBS   = -lstdc++ -lm
SYSLIBS  += `pkg-config --libs python`
SYSLIBS  += -lJudy

# The main target is responsible to make all
.PHONY: ${SUBDIRS}
all: ${LIBGCC} ${TARGET}

# All the tests programs
${TARGET}: ${OBJS} ${USRLIBS}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

${LIBGCC}:
	@echo "=*= making directory $@ =*="
	@make -s -C ${GCCDIR}

# Cleanup rules
clean:
	@rm -f random-word-list.txt
	@rm -f ${TARGET}
	@rm -f ${OBJS}
	@rm -f *~
	@make -s -C ${GCCDIR} clean

distclean: clean
	@rm -f ${DEPS}
	@make -s -C ${GCCDIR} distclean

# How to make an object file from a C source file
%.o: %.c
	@echo "=*= making object $@ =*="
	@${CC} -c ${CFLAGS} $<

# How to make an object file from a C++ source file
%.o: %.cpp
	@echo "=*= making object $@ =*="
	@${C++} -c ${C++FLAGS} $<

# How to make a depend file from a C source file
%.M: %.c
	@-${CC} -MM -MT $(<:.c=.o) ${INCLUDES} $< -o $@ 1> /dev/null 2>& 1

# How to make a depend file from a C++ source file
%.M: %.cpp
	@-${C++} -MM -MT $(<:.cpp=.o) ${INCLUDES} $< -o $@ 1> /dev/null 2>& 1

-include ${DEPS}

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

help:
	@echo "Usage:"
	@echo "  make run"
	@echo "     Single shot using dictionary in file dictionary.txt"
	@echo
	@echo "  make benchmark"
	@echo "     Running multiple shots using dictionary in file random-word-list.txt"

# Run the program
run: ${TARGET}
	@echo "Running single shot using dictionary in file [dictionary.txt]"
	@echo
	@${TARGET} dictionary.txt | tail -11 | sort -t' ' -k 2n

# Run the benchmark
benchmark: ${TARGET}
	@echo -n "Generating random dictionary ... "
	@python make-word-list.py > random-word-list.txt
	@echo "done!"
#
	@echo ""
	@echo "Running multiple shots using dictionary in file random-word-list.txt"
	@echo
	@${TARGET} random-word-list.txt | tee random-word-list.out
	@echo ""
	@echo "Sorted Average of 5 runs:"
	@echo ""
	@uname -a
	@echo "name                 avg(ms)      stddev(ms)"
	@echo "==================== =======    ============="
	@cat random-word-list.out | tail -11 | sort -t' ' -k 2n
	@rm -f random-word-list.out
