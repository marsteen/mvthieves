#!/bin/tcsh
#/usr/nekoware/bin/bash
#sleep 30h

#for {set x 0} {$x<10} {incr x}
#puts "x is $x"
set x=0
#while [ $x -le 100 ]; do
while ( $x < 100 )
 echo $x
 @ x=$x + 1
end







