#!/bin/sh
#
# initialises the symbolic links for the repository
#
# This file is executed after the repository was updated.

# delete existing links
rm -f .git/hooks/pre-commit 
rm -f .git/hooks/post-merge
#e-pucks
rm -f os/events/e-puck
rm -f os/io/e-puck
rm -f os/processes/e-puck
rm -f os/platform/e-puck/library 
#kilobots
rm -f os/events/kilobot
rm -f os/io/kilobot
rm -f os/processes/kilobot
rm -f os/platform/kilobot/library 

# create new links
cd os/events/
ln -s ../platform/e-puck e-puck
ln -s ../platform/kilobot kilobot

cd ../io/
ln -s ../platform/e-puck e-puck
ln -s ../platform/kilobot kilobot

cd ../processes/
ln -s ../platform/e-puck e-puck
ln -s ../platform/kilobot kilobot

cd ../platform/e-puck/
ln -s ../../../extern/platform/e-puck/library library
cd ../kilobot/
ln -s ../../../extern/platform/kilobot/kilolib library

# create new git hook links
cd ../../../.git/hooks/
ln -s ../../.git-hooks/pre-commit pre-commit
ln -s ../../.git-hooks/post-merge post-merge
