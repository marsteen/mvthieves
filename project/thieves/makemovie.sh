#!/bin/sh
mencoder mf://movie/*.tga -mf w=800:h=600:fps=50:type=tga -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -oac copy -o movie.avi

