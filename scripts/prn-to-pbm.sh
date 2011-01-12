#!/bin/bash

. $EC_FUNCTIONS

svg2pdf="$EC_TEST_CODE/svg2pdf/svg2pdf"

shift $(($OPTIND - 1))

if [ $# != 1 ]; then
  echo "Usage: $0 prn-file"
  exit 1
fi
file=$1

if test $VERBOSE; then
  set -x
else
  # suppress segmentation fault output
  exec 2> /dev/null
fi

try "rtl2svg $file... " "$EC_PYTHON/rtl2svg.py -c $file $file.svg"
if [ $? -ne 0 ]; then
  echo "rtl2svn.py failed"
fi
try "svg2pdf $file.svg..." "$svg2pdf $file.svg $file.pdf"
if [ $? -ne 0 ]; then
  echo "svg2pdf failed"
fi
try "pdftoppm $file.pdf" "pdftoppm -q -r 36 -mono $file.pdf tmpfile"
# Segmentation fault - it has happened
if [ $? -eq 139 ]; then
  exit 1
fi
try "mv result..." "mv tmpfile-1.pbm $file.pbm"

