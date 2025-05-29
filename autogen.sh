#!/bin/sh

libtoolize
cat m4/*.m4 > aclocal.m4
aclocal
automake --foreign --add-missing --copy
autoconf
