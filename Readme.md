# Sketchnote Scanner

This is a little script I use to scan my sketchnotes and convert them into SVG.

My sketchnotes are drawn on A5 white paper with a black fineliner and an optional second colored fineliner.
No shaders, gradients, aquarells or similar. In other words: This could work for you as well, if you have
a similar setup. If you do something different, it will likely not turn out the way you want it to.

## Disclaimer

This is "take it or leave it" software. I am happy if it works for you or you can draw inspiration from it.
But I don't intend to develop this beyond my own use case. 

## Installation

    sudo dnf install opencv-devel potrace ImageMagick
    cd decompose/
    cmake .
    make
    
## Usage

    ./scan.sh "Title of the scanned file"

## How it works

0. Make a scan of the image
0. The binary `decompose` detects the colors of my pens and decomposes the scan into monochromic images
   for each color used.
0. [Potrace](http://potrace.sourceforge.net/) converts each of the colors into svg
0. `compose.py` combines the svg files into one drawing