#!/bin/bash

export MG_GAL_ENGINE=dummy
export MG_IAL_ENGINE=dummy

./ustrgetbreaks
if test ! $? -eq 0; then
    echo "ustrgetbreaks not passed"
    exit 1
fi

./biditest
if test ! $? -eq 0; then
    echo "biditest not passed"
    exit 1
fi

./bidicharactertest
if test ! $? -eq 0; then
    echo "bidicharactertest not passed"
    exit 1
fi

./createlogfontex 3600
if test ! $? -eq 0; then
    echo "createlogfontex 3600 not passed"
    exit 1
fi

./drawglyphstring 3600
if test ! $? -eq 0; then
    echo "drawglyphstring 3600 not passed"
    exit 1
fi

./createtextrunsinfo 1
if test ! $? -eq 0; then
    echo "createtextrunsinfo 1 not passed"
    exit 1
fi

./createtextrunsinfo 2
if test ! $? -eq 0; then
    echo "createtextrunsinfo 2 not passed"
    exit 1
fi

./createlayoutinfo
if test ! $? -eq 0; then
    echo "createlayoutinfo not passed"
    exit 1
fi

./createlayoutinfo 1
if test ! $? -eq 0; then
    echo "createlayoutinfo 1 not passed"
    exit 1
fi

./createlayoutinfo 2
if test ! $? -eq 0; then
    echo "createlayoutinfo 2 not passed"
    exit 1
fi

./createlayoutinfo 3
if test ! $? -eq 0; then
    echo "createlayoutinfo 3 not passed"
    exit 1
fi

echo "ALL TEST CASES PASSED!"
exit 0
