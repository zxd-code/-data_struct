# -*- Makefile -*-
# 
# $Id: //stdlib/dev/etc/stdlib/config/GNUmakefile#61 $
#
# main makefile to build and test the C++ Standard library
#
##############################################################################
#
# Usage:
#
#   When invoked from $TOPDIR will create $BUILDDIR, makefile.in, configure
#   and create a configuration header file, config.h, build the library,
#   tests and examples, run them and try to post collected results to
#   a revision control system (Perforce):
#
#   $ make BUILDTYPE=<build-type> BUILDDIR=<build-dir> CONFIG=<config-file>
#           PHDIR=<plumhall-testsuite-source-dir>
#
#   From $BUILDDIR/include to [re]configure or just run one or more tests:
#
#   $ make [ config | config.h | <target> ... ]
#
#   From $TOPDIR or $BUILDDIR to configure if necessary and build the lib,
#   tests, plumhall testsuite, and examples:
#
#   $ make [ libstd ] [ rwtest ] [ tst ] [ phtst ] [ exm ]
#
#   From $TOPDIR or $BUILDDIR to build the lib, tests, and examples, run the
#   built executables, and (by default, unless DRYRUN is non-empty)
#   post results in Perforce:
#
#   $ make post [ DRYRUN=1 ]
#
#   From $BUILDDIR/tests or $BUILDDIR/examples to run the executables
#   and create a report file (unless RUN is non-empty):
#
#   $ make run | runall | run_all [ RUN=<executables> | ALL ]
#
#   For every invocation of make the variables CPPOPTS, CXXOPTS, LDOPTS,
#   and RUNOPTS can be set on the command line to pass additional options to
#   the preprocessor, compiler, linker, and the runall.sh script, respectively.
#
#   The variable MAKE can be set to contain any additional make arguments
#   to be passed to recursive invokations of make (e.g., make -j).
#
#   The variable SHELL can be set to replace the default shell, /bin/sh.
#
##############################################################################
#
# Nonstandard variables:
#
#   BUILDDIR  - the root of the build directory tree
#
#   BUILDTAG  - an optional string appended to a report filename
#
#   BUILDTYPE - one of
#                   8s, 8d, 11s, 11d, 12s, 12d, 15s, 15d,
#                   8S, 8D, 11S, 11D, 12S, 12D, 15S, 15D
#               where the capitalized forms are wide forms of the others
#
#   BUILDMODE - a comma separated list of at most one of each of
#                  debug, optimized, shared
#                  threads, pthreads, dcethreads, wide
#               where
#               *  debug turns on debugging and disables optimization
#               *  optimized enables optimization and disables debugging
#               *  shared creates and links with a shared library
#               *  threads uses Solaris threads for thread safety
#               *  pthreads uses POSIX threads for thread safety
#               *  dcethreads uses DCE threads for thread safety
#               *  wide uses wide (typically 64-bit) memory model
#
#   CATFILE   - name of library catalog file to be generated - see gencat(1)
#
#   CCVER     - compiler version (must contain no spaces)
#
#   DEPENDFLAGS - preprocessor flags used to generate dependencies
#
#   LIBBASE   - name of the library w/o the suffix suitable
#               for use by the linker after the -l option
#
#   LIBDIR    - the directory where the library will be built
#
#   DEPENDDIR - the subdirectory where dependency files will reside
#
#   LIBNAME   - full name of the library being built
#
#   LIBVER    - library version number in the <major>.<minor>.<micro> format
#
#   LIBSUFFIX - static/shared library suffix (defaults to a)
#
#   LDSOFLAGS - linker flags used when building a shared lib
#
#   LDLIBS    - set of system libraries to link executables with
#
#   PHDIR     - directory containing the PlumHall testsuite distribution
#                 (e.g., ".../lvs99a")
#
#   PHWARNFLAGS - extra flags to suppress warnings coming from PlumHall tests
#
#   PICFLAGS  - flags to generate Position Independent Code
#               for shared libraries
#
#   PRELINKFLAGS - flags passed to the prelinker
#
#   REPORTFILE - file containing the results of `make run'
#
#   RUNFLAGS  - default options passed to runall.sh when running executables
#   RUNOPTS   - additional options to pass to runall.sh (empty by default)
#
#   SHARED    - set to `true' if building a shared lib, `false' otherwise
#
#   WARNFLAGS - any compiler warning options
#
##############################################################################

SHELL       = /bin/sh

# Set the value of OSNAME here
OSNAME = $(shell uname)

# provide a value if it isn't already set by (an older version of) make
ifeq ($(CURDIR),)
CURDIR       = $(shell pwd)
endif

# value of BUILDDIR is overridden when passed on command line
BUILDDIR    = $(CURDIR)

ifeq ($(TOPDIR),$(BUILDDIR))
BUILDDIR    = $(TOPDIR)/build
endif

INCDIR      = $(BUILDDIR)/include
LIBDIR      = $(BUILDDIR)/lib
EXMDIR      = $(BUILDDIR)/examples
TSTDIR      = $(BUILDDIR)/tests
PHTSTDIR    = $(BUILDDIR)/plumhall
MAKEDIRS    = $(BUILDDIR)           \
              $(LIBDIR)             \
              $(BUILDDIR)/rwtest    \
              $(BUILDDIR)/bin       \
              $(BUILDDIR)/nls       \
              $(TSTDIR)             \
              $(PHTSTDIR)           \
              $(EXMDIR)             \
              $(INCDIR)

# try to determine configuration (unless specified on the command line)
ifeq ($(CONFIG),)
ifeq ($(shell g++ -v >/dev/null 2>&1 && echo $$?),0)
CONFIG      = gcc.config
else
ifeq ($(shell aCC -V >/dev/null 2>&1 && echo $$?),0)
CONFIG      = hpux_acc.config
else
ifeq ($(shell xlC -qversion >/dev/null 2>&1 && echo $$?),0)
CONFIG      = aix_xlc.config
endif
endif
endif
endif

# file to write log of the build to
LOGFILE     = /dev/null

# convert a relative pathname to an absolute one
ifneq ($(shell echo $(LOGFILE) | sed -n "s/^ *\/.*/\//p"),/)
LOGFILE    := $(BUILDDIR)/$(LOGFILE)
endif

# shared and static library suffix defaults (can be overridden in config file)
SHARED_SUFFIX = .so
STATIC_SUFFIX = .a

ifneq ($(BUILDTYPE),)
ifneq ($(BUILDMODE),)
  ERROR_MSG := $(error Use either BUILDTYPE or BUILDMODE, not both)
endif
endif

# include the makefile.in if it exists 
MAKEFILE_IN = $(BUILDDIR)/makefile.in

-include $(MAKEFILE_IN)

# When BUILDTYPE is set, then decode its value
ifeq ($(BUILDTYPE),8s)
BUILDMODE   = optimized
endif   # ifeq ($(BUILDTYPE),8s)

ifeq ($(BUILDTYPE),8S)
BUILDMODE   = optimized,wide
endif   # ifeq ($(BUILDTYPE),8s)

ifeq ($(BUILDTYPE),8d)
BUILDMODE   = shared,optimized
endif   # ifeq ($(BUILDTYPE),8d)

ifeq ($(BUILDTYPE),8D)
BUILDMODE   = shared,optimized,wide
endif   # ifeq ($(BUILDTYPE),8d)

ifeq ($(BUILDTYPE),11s)
BUILDMODE   = debug
endif   # ifeq ($(BUILDTYPE),11s)

ifeq ($(BUILDTYPE),11S)
BUILDMODE   = debug,wide
endif   # ifeq ($(BUILDTYPE),11s)

# 11d - single-thread, debug, shared
ifeq ($(BUILDTYPE),11d)
BUILDMODE   = debug,shared
endif   # ifeq ($(BUILDTYPE),11d)

ifeq ($(BUILDTYPE),11D)
BUILDMODE   = debug,shared,wide
endif   # ifeq ($(BUILDTYPE),11d)

# 12s - multi-thread, optimized, static
ifeq ($(BUILDTYPE),12s)
BUILDMODE   = pthreads,optimized
endif   # ifeq ($(BUILDTYPE),12s)

ifeq ($(BUILDTYPE),12S)
BUILDMODE   = pthreads,optimized,wide
endif   # ifeq ($(BUILDTYPE),12s)

# 12d - multi-thread, optimized, shared
ifeq ($(BUILDTYPE),12d)
BUILDMODE   = pthreads,shared,optimized
endif   # ifeq ($(BUILDTYPE),12d)

ifeq ($(BUILDTYPE),12D)
BUILDMODE   = pthreads,shared,optimized,wide
endif   # ifeq ($(BUILDTYPE),12d)

# 15s - multi-thread, debug, static
ifeq ($(BUILDTYPE),15s)
BUILDMODE   = debug,pthreads
endif   # ifeq ($(BUILDTYPE),15s)

ifeq ($(BUILDTYPE),15S)
BUILDMODE   = debug,pthreads,wide
endif   # ifeq ($(BUILDTYPE),15s)

# 15d - multi-thread, debug, shared
ifeq ($(BUILDTYPE),15d)
BUILDMODE   = debug,pthreads,shared
endif   # ifeq ($(BUILDTYPE),15d)

ifeq ($(BUILDTYPE),15D)
BUILDMODE   = debug,pthreads,shared,wide
endif   # ifeq ($(BUILDTYPE),15d)

# include the configure file if make is being invoked
# in the source directory; after makefile.in is created
# the configure file will not be included because TOPDIR
# will be defined in makefile.in
ifeq ($(TOPDIR),)
include etc/config/$(CONFIG)
TOPDIR      = $(CURDIR)
endif # TOPDIR

ETCDIR      = $(TOPDIR)/etc/config

##############################################################################
# THE FOLLOWING SECTION IS ONLY INCLUDED IF MAKE IS BEING EXECUTED
# IN THE SOURCE DIRECTORY (AS OPPOSED TO THE BUILD DIRECTORY)
##############################################################################


ifeq ($(CURDIR),$(TOPDIR))

ifeq ($(findstring debug,$(BUILDMODE)),debug)
ifeq ($(findstring optimized,$(BUILDMODE)),optimized)
  ERROR_MSG := $(error Cannot have both debug and optimized options)
endif
endif

# unless $(LD) is set, use the same command to link as to compile
ifeq ($(LD),ld)
LD = $(CXX)
endif

# debug/optimized
ifeq ($(findstring debug,$(BUILDMODE)),debug)
  CXXFLAGS  += $(DEBUG_CXXFLAGS)
  CPPFLAGS  += -D_RWSTDDEBUG $(DEBUG_CPPFLAGS)
else
  # TODO - check the number, check if the compiler supports it
  ifeq ($(findstring optimized,$(BUILDMODE)),optimized)
    CXXFLAGS  += $(OPTMZ_CXXFLAGS)
    CPPFLAGS  += $(OPTMZ_CPPFLAGS)
  endif
endif

## shared/archive
ifeq ($(findstring shared,$(BUILDMODE)),shared)
  CXXFLAGS  += $(SHARED_CXXFLAGS)
  CPPFLAGS  += $(SHARED_CPPFLAGS)
  LDFLAGS   += $(SHARED_LDFLAGS)
  LIBSUFFIX  = $(SHARED_SUFFIX)
else
  CXXFLAGS  += $(STATIC_CXXFLAGS)
  CPPFLAGS  += $(STATIC_CPPFLAGS)
  LDFLAGS   += $(STATIC_LDFLAGS)
  LIBSUFFIX  = $(STATIC_SUFFIX)

  # not applicable to non-shared builds
  PICFLAGS   =
  LDSOFLAGS  =
endif

## POSIX, Ssolaris, DCE threads
ifeq ($(findstring pthreads,$(BUILDMODE)),pthreads)
  CPPFLAGS  += $(MULTI_CPPFLAGS_POSIX)
  LDFLAGS   += $(MULTI_LDFLAGS_POSIX)
else
  ifeq ($(findstring dcethreads,$(BUILDMODE)),dcethreads)
    ifneq ($(OSNAME),OSF1)
      ERROR_MSG := $(error DCE threads not suported on this platform)
    endif

    CPPFLAGS  += $(MULTI_CPPFLAGS_DCE)
    LDFLAGS   += $(MULTI_LDFLAGS_DCE)
  else
    ifeq ($(findstring threads,$(BUILDMODE)),threads)
      ifneq ($(OSNAME),SunOS)
        ERROR_MSG := $(error Solaris threads not suported on this platform)
      endif

      CPPFLAGS  +=  $(MULTI_CPPFLAGS_SOLARIS)
      LDFLAGS   +=  $(MULTI_LDFLAGS_SOLARIS)
    else
      CPPFLAGS  += $(SINGL_CPPFLAGS)
      LDFLAGS   += $(SINGL_LDFLAGS)
    endif
  endif
endif

# wide (typically 64-bit) mode
ifeq ($(findstring wide,$(BUILDMODE)),wide)
  CXXFLAGS    += $(WIDE_CXXFLAGS)
  LDFLAGS     += $(WIDE_LDFLAGS)
  LDSOFLAGS   += $(WIDE_LDSOFLAGS)
  ARFLAGS     += $(WIDE_ARFLAGS)
endif

# platform is determined as {OS-name}-{OS-version}-{hardware}
PLATFORM   = $(shell uname -srm | sed "s/[ \/]/-/g")

ifeq ($(OSNAME),SunOS)
  # Sun recommends to use uname -p rather than the POSIX uname -m
  PLATFORM  := $(shell uname -srp | sed "s/[ \/]/-/g")
else
  ifeq ($(OSNAME),AIX)
    PLATFORM  := $(shell uname -srv | awk '{ print $$1 "-" $$3 "." $$2 }')
  else
    ifeq ($(findstring CYGWIN,$(OSNAME)),CYGWIN)
        PLATFORM := $(shell uname -sm | sed "s/[ \/]/-/g")
    endif
  endif
endif

# harmonize all the different Intel IA32 chips
PLATFORM  := $(subst i486,i86,$(PLATFORM))
PLATFORM  := $(subst i586,i86,$(PLATFORM))
PLATFORM  := $(subst i686,i86,$(PLATFORM))

REPORTFILE = $(CXX)-$(CCVER)-$(PLATFORM)-$(BUILDTYPE)

# name of the library w/o the prefix and suffix
# suitable for use by the linker after the -l option
LIBBASE     = std$(BUILDTYPE)

# full library filename (including suffix)
LIBNAME     = lib$(LIBBASE)$(LIBSUFFIX)

# add to enable config.h (alternate config mechanism)
CPPFLAGS += -D_RWSTD_USE_CONFIG

endif  # ifeq ($(CURDIR),$(TOPDIR))

##############################################################################
# TARGETS
##############################################################################

ifeq ($(CURDIR),$(TOPDIR))

ifeq ($(DEPENDDIR),)
DEPENDDIR=.depend
endif

# obtain library version number from the macro _RWSTD_VER
# #defined in the rw/_config.h library header
LIBVER     = $(shell awk '/^.define _RWSTD_VER / { major = substr ($$3, 3, 2); minor = substr ($$3, 5, 2); micro = substr ($$3, 7, 2); print (major + 0) "." (minor + 0) "." (micro + 0) }' $(TOPDIR)/include/rw/_config.h)

# invoked from $(TOPDIR)...

all: builddir config libstd 

# create $(BUILDDIR) and $(MAKEFILE_IN)
$(MAKEFILE_IN):
	@(echo "creating BUILDDIR=$(BUILDDIR)" ;                           \
          mkdir -p $(MAKEDIRS) || {                                        \
              echo "unable to create build directory" ;                    \
              exit 1 ;                                                     \
          } ;                                                              \
      echo "generating $(MAKEFILE_IN)..." ;                                \
      echo "TOPDIR     = $(TOPDIR)"                   >> $(MAKEFILE_IN) ;  \
      echo "BUILDDIR   = $(BUILDDIR)"                 >> $(MAKEFILE_IN) ;  \
      echo "BUILDTYPE  = $(BUILDTYPE)"                >> $(MAKEFILE_IN) ;  \
      echo "BUILDMODE  = $(BUILDMODE)"                >> $(MAKEFILE_IN) ;  \
      echo "CXX        = $(CXX)"                      >> $(MAKEFILE_IN) ;  \
      echo "CXXFLAGS   = $(CXXFLAGS)"                 >> $(MAKEFILE_IN) ;  \
      echo "PRELINKFLAGS = $(PRELINKFLAGS)"           >> $(MAKEFILE_IN) ;  \
      echo "PICFLAGS   = $(PICFLAGS)"                 >> $(MAKEFILE_IN) ;  \
      echo "CPPFLAGS   = $(CPPFLAGS)"                 >> $(MAKEFILE_IN) ;  \
      echo "WARNFLAGS  = $(WARNFLAGS)"                >> $(MAKEFILE_IN) ;  \
      echo "DEPENDFLAGS = $(DEPENDFLAGS)"             >> $(MAKEFILE_IN) ;  \
      echo "LD         = $(LD)"                       >> $(MAKEFILE_IN) ;  \
      echo "LDFLAGS    = $(LDFLAGS)"                  >> $(MAKEFILE_IN) ;  \
      echo "LDLIBS     = $(LDLIBS)"                   >> $(MAKEFILE_IN) ;  \
      echo "LDSOFLAGS  = $(LDSOFLAGS)"                >> $(MAKEFILE_IN) ;  \
      echo "MAPFILE    = $(MAPFILE)"                  >> $(MAKEFILE_IN) ;  \
      echo "RUNFLAGS   = -t 180"                      >> $(MAKEFILE_IN) ;  \
      echo "LIBDIR     = $(LIBDIR)"                   >> $(MAKEFILE_IN) ;  \
      echo "DEPENDDIR  = $(DEPENDDIR)"                >> $(MAKEFILE_IN) ;  \
      echo "PHDIR      = $(PHDIR)"                    >> $(MAKEFILE_IN) ;  \
      echo "PHWARNFLAGS = $(PHWARNFLAGS)"             >> $(MAKEFILE_IN) ;  \
      echo "LIBSUFFIX  = $(LIBSUFFIX)"                >> $(MAKEFILE_IN) ;  \
      echo "LIBBASE    = $(LIBBASE)"                  >> $(MAKEFILE_IN) ;  \
      echo "LIBVER     = $(LIBVER)"                   >> $(MAKEFILE_IN) ;  \
      echo "LIBNAME    = lib$$""(LIBBASE)$$""(LIBSUFFIX)"                  \
                                                      >> $(MAKEFILE_IN) ;  \
      echo "AR         = $(AR)"                       >> $(MAKEFILE_IN) ;  \
      echo "ARFLAGS    = $(ARFLAGS)"                  >> $(MAKEFILE_IN) ;  \
      echo "CCVER      = $(CCVER)"                    >> $(MAKEFILE_IN) ;  \
      echo "SHARED     = $(SHARED)"                   >> $(MAKEFILE_IN) ;  \
      echo "CATFILE    = $(CATFILE)"                  >> $(MAKEFILE_IN) ;  \
      echo "OMIT_EXM_SRCS = $(OMIT_EXM_SRCS)"         >> $(MAKEFILE_IN) ;  \
      echo "OMIT_TST_SRCS = $(OMIT_TST_SRCS)"         >> $(MAKEFILE_IN) ;  \
      echo "BUILDTAG   = $(BUILDTAG)"                 >> $(MAKEFILE_IN) ;  \
      echo "PLATFORM   = $(PLATFORM)"                 >> $(MAKEFILE_IN) ;  \
      echo "REPORTFILE = $(REPORTFILE)$$""(BUILDTAG)" >> $(MAKEFILE_IN) ;  \
      echo "DEFAULT_SHROBJ = $(DEFAULT_SHROBJ)"       >> $(MAKEFILE_IN) ;  \
	  echo "CXX_REPOSITORY = $(CXX_REPOSITORY)"	  >> $(MAKEFILE_IN))

# link makefiles and scripts
builddir: $(MAKEFILE_IN)
	@(ln -sf $(TOPDIR)/GNUmakefile     $(BUILDDIR);                     \
      ln -sf $(ETCDIR)/GNUmakefile.cfg     $(BUILDDIR)/include/GNUmakefile; \
      ln -sf $(ETCDIR)/GNUmakefile.lib     $(LIBDIR)/GNUmakefile;           \
      ln -sf $(ETCDIR)/GNUmakefile.rwt     $(BUILDDIR)/rwtest/GNUmakefile;  \
      ln -sf $(ETCDIR)/GNUmakefile.exm     $(EXMDIR)/GNUmakefile;           \
      ln -sf $(ETCDIR)/GNUmakefile.tst     $(TSTDIR)/GNUmakefile;           \
      ln -sf $(ETCDIR)/GNUmakefile.ph      $(PHTSTDIR)/GNUmakefile;         \
      ln -sf $(ETCDIR)/GNUmakefile.bin     $(BUILDDIR)/bin/GNUmakefile;     \
      ln -sf $(ETCDIR)/makefile.common     $(BUILDDIR);                     \
      ln -sf $(ETCDIR)/makefile.rules      $(BUILDDIR);                     \
      ln -sf $(ETCDIR)/configure.sh        $(BUILDDIR)/include/configure;   \
      ln -sf $(ETCDIR)/runall.sh           $(BUILDDIR)/run;                 \
      ln -sf $(ETCDIR)/runall.sh           $(BUILDDIR)/bin/run;             \
      ln -sf $(ETCDIR)/run_locale_utils.sh $(BUILDDIR)/bin/run_locale_utils.sh;\
      ln -sf $(ETCDIR)/runall.sh           $(TSTDIR)/run;                   \
      ln -sf $(ETCDIR)/runall.sh           $(PHTSTDIR)/run;                 \
      ln -sf $(ETCDIR)/runall.sh           $(EXMDIR)/run)

post:
	@$(MAKE) builddir
	@$(MAKE) -C$(BUILDDIR) $@


# cd from TOPDIR to BUILDDIR and post from there
.DEFAULT:
	@$(MAKE) -C$(BUILDDIR) $@


else   # ifneq ($(CURDIR),$(TOPDIR))

# invoked from $(BUILDDIR)...

all: config libstd locutils rwtestlib tst exm $(PHDTSTDIR)

# configure (create config.h)
config:
	@$(MAKE) -C include

# build utilities, ignore errors
util:
	@$(MAKE) -C $(BUILDDIR)/bin

# build library, errors are fatal
libstd:
	@$(MAKE) -C $(LIBDIR)

# locale utilities
bin:
	@$(MAKE) -C $(BUILDDIR)/bin

# build rw test library
rwtestlib:
	@$(MAKE) -C $(BUILDDIR)/rwtest

# build tests, ignore failures
tst: libstd rwtestlib
	-@$(MAKE) -C $(TSTDIR)

# build plumhall tests, ignore failures
phtst: libstd
	-@$(MAKE) -C $(PHTSTDIR)


# make examples, ignore failures
exm: libstd
	-@$(MAKE) -C $(EXMDIR)


# run all tests and examples, generate result file and post it in a revison
# control system (unless the variable DRYRUN is non-empty) ; regressions
# are computed with respect to last posted report (not just any report that's
# lying around)

# if DRYRUN isn't set, deletes all the executables first so that if a test
# doesn't compile, old executables aren't run by accident
# although the target lists no dependencies is does make lib and tries
# to make (failures are ignored via -k) the tst and exm targets

# make listtarget is stripped of make's diganostic output (the "Entering/
# Leaving directory" message); tr is used to chop up the long list of files
# into a bunch of short lines to accomodate SunOS sed that can't handle very
# long input lines (Output line too long error)
post:
	@(trap "rm -rf $(REPORTFILE) post 2" ;                                \
          log=$(LOGFILE);                                                     \
          [ "`echo "$$log" | sed -n 's/^ *\/.*/\//p'`" = / ]                  \
          || log=$(BUILDDIR)/$$log ;                                          \
          ts0=`date +%T`;                                                     \
          echo > $$log ; date >> $$log ;                                      \
          if [ -z "$(DRYRUN)" ] ; then                                        \
              POST="-p $(TOPDIR)/etc/results/$(REPORTFILE)";                  \
              export POST;                                                    \
              $(MAKE) -s -C $(INCDIR) | tee -a $$log ;                        \
              $(MAKE) cleantarget ;                                           \
          fi ;                                                                \
          ts1=`date +%T`;                                                     \
          echo >> $$log ; date >> $$log ; echo >> $$log ;                     \
          $(MAKE) LOGFILE=$$log lib ;                                         \
          ts2=`date +%T`;                                                     \
          date >> $$log ;                                                     \
          $(MAKE) -k LOGFILE=$$log bin tst exm ;                              \
          echo >> $$log ; date >> $$log ; echo >> $$log ;                     \
          [ ! -d post ] && mkdir post;                                        \
          last=$(TOPDIR)/etc/results/$(REPORTFILE) ;                          \
          tsts=`$(MAKE) -C $(TSTDIR) listtarget | sed -n "n;p" | tr ' ' '\n'  \
                | sed "s:^:../tests/:g"`;                                     \
          ph_tsts=`$(MAKE) -C $(PHTSTDIR) listtarget listsubtests             \
                | sed -n "n;p" | tr ' ' '\n' | sed "s:^:../plumhall/:g"`;     \
          exms=`$(MAKE) -C $(EXMDIR) listtarget | sed -n "n;p" | tr ' ' '\n'  \
                | sed "s:^:../examples/:g"` ;                                 \
          desc="Sizes: `du -ak lib/$(LIBNAME) | awk '{print $$1}'             \
                ` KB $(LIBNAME), `du -sk $(BUILDDIR) | awk '{print $$1}'      \
                ` KB $(BUILDDIR)" ;                                           \
          ltsts=`$(MAKE) -C$(BUILDDIR)/bin listruntarget |                    \
                sed -n "n;p" | tr ' ' '\n' | sed "s:^:../bin/:g"`;            \
          (cd post;                                                           \
           LD_LIBRARY_PATH=$(LIBDIR):$$LD_LIBRARY_PATH                        \
           PATH=$(BUILDDIR)/bin:$$PATH                                        \
           ../run $(RUNFLAGS) -d "$$desc" -X "-C $(CXX)-$(CCVER)" -r $$last   \
                  -T "$$ts0,$$ts1,$$ts2" --log $$log $$ltsts $$tsts           \
                  -x $(TOPDIR)/examples $$exms --ph $$ph_tsts $$POST          \
                  | tee $(REPORTFILE) ) ;                                     \
          mv post/$(REPORTFILE) $(REPORTFILE) ;                               \
          rm -r post)


# try each submakefile
.DEFAULT:
	-@$(MAKE) -C $(LIBDIR) $@
	-@$(MAKE) -C $(BUILDDIR)/rwtest $@
	-@$(MAKE) -C $(BUILDDIR)/bin $@
	-@$(MAKE) -C $(TSTDIR) $@
	-@$(MAKE) -C $(PHTSTDIR) $@
	-@$(MAKE) -C $(EXMDIR) $@

endif   # ($(CURDIR),$(TOPDIR))


.PHONY: all builddir libstd rwtest bin tst phtst exm post
