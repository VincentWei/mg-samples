#!/bin/bash

export MG_GAL_ENGINE=dummy
export MG_IAL_ENGINE=dummy

start_epoch=$(date +%s)

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

./drawglyphstringex 3600
if test ! $? -eq 0; then
    echo "drawglyphstringex 3600 not passed"
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

./basicshapingengine 3600
if test ! $? -eq 0; then
    echo "basicshapingengine 3600 not passed"
    exit 1
fi

end_epoch=$(date +%s)
time=$(($end_epoch - $start_epoch))

echo "ALL TEST CASES PASSED ($time S)!"
exit 0
