# The name of the games
SRCS      = hashtab.c xmalloc.c xexit.c
MAINSRCS  = basic.c
LIB       = libgcc-hash.a
SHLIB     = $(patsubst %.a,%.so, ${LIB})
TARGETS   = $(patsubst %.c,%, ${MAINSRCS})

# Object and depend files
OBJS      = $(patsubst %.c,%.o, ${SRCS})
MAINOBJS  = $(patsubst %.c,%.o, ${MAINSRCS})
DEPS      = $(patsubst %.c,%.M, ${SRCS} ${MAINSRCS})

# C compiler and flags
DEFINES   = -DHAVE_STDLIB_H -DHAVE_STRING_H -DHAVE_MALLOC_H -DHAVE_LIMITS_H -D HAVE_INTTYPES_H -DHAVE_STDINT_H
INCLUDES  = -I.
CC        = gcc
C++       = g++
AR        = ar
LD        = gcc
CFLAGS    = -g -Wall -fPIC ${INCLUDES} ${DEFINES} -fno-diagnostics-color
C++FLAGS  = ${CFLAGS}
SHFLAGS   = -shared
ARFLAGS   = rv
LDFLAGS   =

# Static private libraries
USRLIBS   = ${LIB}

# System libraries
SYSLIBS   =

# The main target is responsible to make all
all: ${LIB} ${TARGETS}

# Static Library
${LIB}: ${OBJS}
	@echo "=*= making static library $@ =*="
	@${AR} ${ARFLAGS} $@ $^ 1> /dev/null 2>& 1

# Shared Library
${SHLIB}: ${OBJS}
	@echo "=*= making dynamic library $@ =*="
	@${CC} ${SHFLAGS} $^ -o $@

# All the binary programs
%: %.o ${USRLIBS}
	@echo "=*= making program $@ =*="
	@${LD} ${LDFLAGS} $^ ${SYSLIBS} -o $@

# Cleanup rules
clean:
	@rm -f ${TARGETS}
	@rm -f ${LIB} ${SHLIB}
	@rm -f ${OBJS} ${MAINOBJS}
	@rm -f *~

distclean: clean
	@rm -f ${DEPS}

# How to make an object file from a C source
%.o: %.c
	@echo "=*= making object $@ =*="
	@${CC} -c ${CFLAGS} $<

# How to make a depend file from a C source
%.M: %.c
	@-${CC} -MM -MT $(<:.c=.o) ${CFLAGS} $< -o $@ 1> /dev/null 2>& 1

-include ${DEPS}
