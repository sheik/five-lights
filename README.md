five-lights
===========

There are four lights!

Requirements
============
cairomm:
http://cairographics.org/cairomm/

Compiling Font lib OS X
=======================
Install cairomm from homebrew:

        bash$ brew install cairomm

For some reason, $PKG_CONFIG_PATH gets messed up. Here's how to compile:

        bash$ export PKG_CONFIG_PATH=/usr/X11/lib/pkgconfig
        bash$ clang++ `pkg-config --cflags --libs cairomm-1.0` test-cairomm.cpp 

