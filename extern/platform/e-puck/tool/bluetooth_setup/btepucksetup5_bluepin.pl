#! /usr/bin/perl
use strict;

# Check if the bluepin program is around
if (! -f '/usr/bin/bluepin') {
	print 'ERROR: The /usr/bin/bluepin program is not around ... do you need to install BLUEZ first?', "\n";
	exit(1);
}

# Make a backup copy
print 'I\'ll copy the original bluepin program to /usr/bin/bluepin-orig ...', "\n";
if (-f '/usr/bin/bluepin-orig') {
	print 'ERROR: The file /usr/bin/bluepin-orig is already around ... did you run this script already?', "\n";
	print 'Delete or rename /usr/bin/bluepin-orig if you want to run this script.', "\n";
	exit(1);
}

system('cp', '/usr/bin/bluepin', '/usr/bin/bluepin-orig') && die 'ERROR: Failed to backup original bluepin file';

# Copy the new file
print '... and copy my version to /usr/bin/bluepin.', "\n";
system('cp', 'bluepin', '/usr/bin/bluepin') && die 'ERROR: Failed to copy new bluepin file';

# Indicate success
print 'Done!', "\n";
