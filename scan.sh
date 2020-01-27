#!/bin/bash -eu

# @author Christian Zenker<dev@xopn.de>
#
# This script scans one of my hand drawn sketches that are basically black
# and white with one accent color.

#
# SETTINGS
#

# A5 is 148x210
# using A5 with a little bit cut off to avoid black borders
SCAN_WIDTH="206"
SCAN_HEIGHT="145"
SCAN_RESOLUTION="600"
SCAN_FORMAT="-l 1 -t 1 -x ${SCAN_WIDTH} -y ${SCAN_HEIGHT}"

if [ -n "${1+x}" ]; then
    PNG_FILENAME="${1}.png"
else
    PNG_FILENAME="scan-$(date '+%Y-%m-%d-%H-%M-%S').png"
fi

SVG_FILENAME=$(basename "${PNG_FILENAME}" ".png").svg

echo "$SVG_FILENAME"

SCAN_TMPFILE=$(mktemp --suffix="scan") || exit
# usage of "trap" as seen on the tempfile manpage
trap "rm -f -- '$SCAN_TMPFILE'" EXIT
 
echo "Scanning the image @${SCAN_RESOLUTION}dpi..."
scanimage --mode=Color --depth=8 --resolution=${SCAN_RESOLUTION} ${SCAN_FORMAT} -p >$SCAN_TMPFILE

convert "$SCAN_TMPFILE" "$PNG_FILENAME"

# now we are no longer needing the raw data from the scan
rm -f -- "$SCAN_TMPFILE"
trap - EXIT

#echo "---------------------------------------------------------------"
#echo "Showing $PNG_FILENAME"
#echo ""
#echo "Please close the preview if everything is to your satisfaction."
#echo "---------------------------------------------------------------"
#eog "$PNG_FILENAME" &


SCAN_TMPFILE=$(mktemp -d --suffix="scan") || exit
# usage of "trap" as seen on the tempfile manpage
trap "rm -rf -- '$SCAN_TMPFILE'" EXIT

./decompose/decompose "$PNG_FILENAME" "$SCAN_TMPFILE"

if [ -f "${SCAN_TMPFILE}/black.ppm" ]; then
  echo -n "Trace Black... "
  potrace --color "#000000" --svg --blacklevel 0.99 --width "${SCAN_WIDTH}" --height "${SCAN_HEIGHT}" --resolution "${SCAN_RESOLUTION}" "${SCAN_TMPFILE}/black.ppm" -o "${SCAN_TMPFILE}/black.svg"
  echo "Done"
fi
if [ -f "${SCAN_TMPFILE}/cobaltGreen.ppm" ]; then
  echo -n "Trace Cobalt Green... "
  potrace --color "#218676" --svg --blacklevel 0.99 --width "${SCAN_WIDTH}" --height "${SCAN_HEIGHT}" --resolution "${SCAN_RESOLUTION}" "${SCAN_TMPFILE}/cobaltGreen.ppm" -o "${SCAN_TMPFILE}/cobaltGreen.svg"
  echo "Done"
fi
if [ -f "${SCAN_TMPFILE}/indanthreneBlue.ppm" ]; then
  echo -n "Trace indanthrene Blue... "
  potrace --color "#8295d9" --svg --blacklevel 0.99 --width "${SCAN_WIDTH}" --height "${SCAN_HEIGHT}" --resolution "${SCAN_RESOLUTION}" "${SCAN_TMPFILE}/indanthreneBlue.ppm" -o "${SCAN_TMPFILE}/indanthreneBlue.svg"
  echo "Done"
fi
if [ -f "${SCAN_TMPFILE}/mayGreen.ppm" ]; then
  echo -n "Trace May Green... "
  potrace --color "#b6cc66" --svg --blacklevel 0.99 --width "${SCAN_WIDTH}" --height "${SCAN_HEIGHT}" --resolution "${SCAN_RESOLUTION}" "${SCAN_TMPFILE}/mayGreen.ppm" -o "${SCAN_TMPFILE}/mayGreen.svg"
  echo "Done"
fi
if [ -f "${SCAN_TMPFILE}/phthaloBlue.ppm" ]; then
  echo -n "Trace Phthalo Blue... "
  potrace --color "#578bd9" --svg --blacklevel 0.99 --width "${SCAN_WIDTH}" --height "${SCAN_HEIGHT}" --resolution "${SCAN_RESOLUTION}" "${SCAN_TMPFILE}/phthaloBlue.ppm" -o "${SCAN_TMPFILE}/phthaloBlue.svg"
  echo "Done"
fi
if [ -f "${SCAN_TMPFILE}/red.ppm" ]; then
  echo -n "Trace Red... "
  potrace --color "#bf4d60" --svg --blacklevel 0.99 --width "${SCAN_WIDTH}" --height "${SCAN_HEIGHT}" --resolution "${SCAN_RESOLUTION}" "${SCAN_TMPFILE}/red.ppm" -o "${SCAN_TMPFILE}/red.svg"
  echo "Done"
fi
if [ -f "${SCAN_TMPFILE}/sanguine.ppm" ]; then
  echo -n "Trace Sanguine... "
  potrace --color "#b36a5f" --svg --blacklevel 0.99 --width "${SCAN_WIDTH}" --height "${SCAN_HEIGHT}" --resolution "${SCAN_RESOLUTION}" "${SCAN_TMPFILE}/sanguine.ppm" -o "${SCAN_TMPFILE}/sanguine.svg"
  echo "Done"
fi

./compose.py "${SCAN_TMPFILE}" "${SVG_FILENAME}"

rm -rf -- "$SCAN_TMPFILE"
trap - EXIT

echo "Done"