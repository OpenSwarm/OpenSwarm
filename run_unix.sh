#!/bin/sh
#
# initialises the symbolic links for the repository
#
# This file is executed after the repository was updated.

# create new links
ln -s os/platform/e-puck os/events/e-puck
ln -s os/platform/e-puck os/io/e-puck
ln -s os/platform/e-puck os/processes/e-puck
ln -s extern/platform/e-puck/library os/platform/e-puck/library

# create new git hook links
ln -s .git-hooks/pre-commit .git/hooks/pre-commit
ln -s .git-hooks/post-merge .git/hooks/post-merge
