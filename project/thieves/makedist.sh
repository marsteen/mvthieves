#!/bin/sh
DISTPATH=dist-linux
THIEFPATH=$DISTPATH/thiefs
#
mkdir -p $DISTPATH
cd $DISTPATH
rm -rf thiefs
mkdir thiefs
cd ..
cp -r files $THIEFPATH
cp doc/readme.txt $THIEFPATH
cp doc/readme-linux.txt $THIEFPATH
cp thiefs $THIEFPATH
cp thiefs32 $THIEFPATH
cp run-window.sh $THIEFPATH
#
cd $DISTPATH
tar cvf thiefs.tar thiefs
gzip thiefs.tar
cd ..
