#!/bin/sh
#
# initialises the symbolic links for the repository
#
# This file is executed after the repository was updated.

# delete existing links
rm -f os/events/e-puck
rm -f os/io/e-puck
rm -f os/processes/e-puck
rm -f os/platform/e-puck/library 
rm -f .git/hooks/pre-commit 
rm -f .git/hooks/post-merge

# create new links
cd os/events/
ln -s ../platform/e-puck e-puck

cd ../io/
ln -s ../platform/e-puck e-puck

cd ../processes/
ln -s ../platform/e-puck e-puck

cd ../platform/e-puck/
ln -s ../../../extern/platform/e-puck/library library

# create new git hook links
cd ../../../.git/hooks/
ln -s ../../.git-hooks/pre-commit pre-commit
ln -s ../../.git-hooks/post-merge post-merge
