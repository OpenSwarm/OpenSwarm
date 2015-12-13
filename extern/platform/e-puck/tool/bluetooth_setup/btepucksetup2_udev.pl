#! /usr/bin/perl
use strict;

my $res=addtoudev('/etc/udev/permissions.d/50-udev.permissions');
if (! $res) {
	print 'Failed to add the udev configuration for RFCOMM!', "\n";
}

sub addtoudev {
	my $file=shift;

	open(F, $file) || return 0;
	while (my $line=<F>) {
		if ($line =~ /rfcomm\*\:/) {
			print 'The udev configuration for RFCOMM is already present in ', $file, "\n";
			return 1;
		}
	}
	close F;

	open(F, '>>', $file) || return 0;
	print F "\n";
	print F '# bluetooth devices', "\n";
	print F 'rfcomm*:root:root:0666', "\n";
	close F;
	
	print 'The udev configuration for RFCOMM was added to ', $file, "\n";
	return 1;
}
