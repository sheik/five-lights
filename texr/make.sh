#!/bin/bash

export PKG_CONFIG_PATH=/usr/X11/lib/pkgconfig
clang++ `pkg-config --cflags --libs cairomm-1.0` -o texr-sample texr.cpp 
