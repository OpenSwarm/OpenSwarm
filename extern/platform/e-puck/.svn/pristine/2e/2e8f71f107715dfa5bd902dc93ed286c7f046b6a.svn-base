#! /usr/bin/perl
use strict;

my $basenumber=(shift || 0);

open(INFILE, '/etc/bluetooth/epuck_bdaddr') or die "ERROR: Cannot open input file";
open(OUTFILE, '>', '/etc/bluetooth/rfcomm.conf') or die "ERROR: Cannot open output file";

my $cc=0;
while (my $line=<INFILE>) {
	next if ($line!~/\s*(\S+)\s+e-puck_(\d+)/);

	my $name = $2;
	my $MAC = $1;
	my $num=int($name);
	next if ($num<$basenumber);
	next if ($num>=$basenumber+200);
	my $rfcommnum=($num % 200);

	print OUTFILE "rfcomm$rfcommnum {\n";
	print OUTFILE "\t# Automatically bind the device at startup\n";
	print OUTFILE "\tbind yes;\n\n";
	print OUTFILE "\t# Bluetooth address of the device\n";
	print OUTFILE "\tdevice $MAC;\n\n";
	print OUTFILE "\t# RFCOMM channel for the connection\n";
	print OUTFILE "\tchannel 1;\n\n";
	print OUTFILE "\t# Description of the connection\n";
	print OUTFILE "\tcomment \"epuck$name\";\n}\n\n";

	$cc++;
}

print $cc, ' entries written to /etc/bluetooth/rfcomm.conf', "\n";
system('service bluetooth restart') || print 'Bluetooth service restarted', "\n";
