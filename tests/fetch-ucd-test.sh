#!/bin/bash

url='https://www.unicode.org/Public/12.0.0/ucd/'
files='BidiTest.txt BidiCharacterTest.txt auxiliary/WordBreakTest.txt auxiliary/GraphemeBreakTest.txt auxiliary/LineBreakTest.txt auxiliary/SentenceBreakTest.txt'

if test ! -d ucd/auxiliary; then
    mkdir -p ucd/auxiliary
fi

cd ucd/

for file in $files
do
    if [ -f $file ]; then
        echo "$file exists, skipping"
    else
        echo "Fetching $url$file"
        wget -O $file $url$file
    fi
done

