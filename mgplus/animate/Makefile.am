TOP_DIR=..
AM_CPPFLAGS = -I../include

SUBDIRS=common_animates

# The directory where the include files will be installed
libanimateincludedir = $(includedir)

noinst_LTLIBRARIES = libanimate.la

SRC_FILES =  animate_sence.c timeline.c p-code.c

HDR_FILES =  animate.h p-code.h instruction.h my_debug.h 

INSTALL_HDRS_FILES =  animate.h p-code.h 

libanimateinclude_HEADERS=$(INSTALL_HDRS_FILES) 

libanimate_la_SOURCES = $(SRC_FILES) $(HDR_FILES)
libanimate_la_LIBADD = common_animates/libcommon_animates.la
