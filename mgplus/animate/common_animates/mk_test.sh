#!/bin/sh
export CFLAGS=$TARGET_CFLAGS
export LDFLAGS=$TARGET_LDFLAGS
OPTIONS="${CFLAGS} -I.. ${LDFLAGS} -L.libs -L../.libs -lanimate -lcommon_animates -lminigui_procs -lpthread -lpng -ljpeg"

gcc  -o pin_test pin_test.c ${OPTIONS}
gcc  -o push_pull_img_test push_pull_img_test.c ${OPTIONS}
gcc  -o push_pull_wnd_test push_pull_wnd_test.c ${OPTIONS}

