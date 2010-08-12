add_library(trinitylua src/lapi.c      src/lcode.c   src/ldo.c    src/lgc.c     src/llex.c      src/loadlib.c   src/loslib.c   src/lstring.c  src/ltablib.c  src/luac.c     src/lzio.c
src/lauxlib.c   src/ldblib.c  src/ldump.c  src/linit.c   src/lmathlib.c  src/lobject.c   src/lparser.c  src/lstrlib.c  src/ltm.c      src/lundump.c  src/print.c
src/lbaselib.c  src/ldebug.c  src/lfunc.c  src/liolib.c  src/lmem.c      src/lopcodes.c  src/lstate.c   src/ltable.c   src/lua.c      src/lvm.c      )
SET_TARGET_PROPERTIES(trinitylua PROPERTIES COMPILE_FLAGS -fPIC )
