#!/bin/bash

export MG_GAL_ENGINE=dummy
export MG_IAL_ENGINE=dummy

./ustrgetbreaks
if test ! $? -eq 0; then
    exit 1
fi

./biditest
if test ! $? -eq 0; then
    exit 1
fi

./bidicharactertest
if test ! $? -eq 0; then
    exit 1
fi

./createtextrunsinfo
if test ! $? -eq 0; then
    exit 1
fi

./createlayoutinfo
if test ! $? -eq 0; then
    exit 1
fi

./createlayoutinfo 1
if test ! $? -eq 0; then
    exit 1
fi

./createlayoutinfo 2
if test ! $? -eq 0; then
    exit 1
fi

./createlayoutinfo 3
if test ! $? -eq 0; then
    exit 1
fi

echo "ALL TEXT CASES PASSED!"
exit 0
