#!/bin/bash

ffmpeg -n -f lavfi -i testsrc -t 30 -pix_fmt yuv420p testsrc.mp4
