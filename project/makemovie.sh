#!/bin/sh
mencoder mf://movie/*.tga -mf w=800:h=600:fps=25:type=jpg -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -oac copy -o movie.avi

