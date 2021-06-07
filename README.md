# Jodie's Image Viewer
terminal image viewer for linux written in c without a libc

![jiv image.png $width](https://raw.githubusercontent.com/MathGeniusJodie/jodie-s-image-viewer/main/jiv.png)

## features
- static binary
- fills the width of your terminal window by default
- supports all image formats that imagemagick supports
- transparency

## how it works
JIV uses imagemagick to actually read and resize the image (for compatibility with multiple image formats without adding bloat to JIV itself). The actual user-facing program is a shell script that pipes the rgba output of imagemagick to a binary that handles actually printing to the terminal.

## dependencies
- imagemagick
- stty
- gcc or clang if you want to compile from source
- linux
- x86 64 cpu

## how to install
```
git clone https://github.com/MathGeniusJodie/jodie-s-image-viewer.git
```
copy the files "jiv" and "jiv_binary" to "/usr/bin/" or alternatively compile your own binary with by running "build.sh", you can also edit the build script to use clang instead of gcc
