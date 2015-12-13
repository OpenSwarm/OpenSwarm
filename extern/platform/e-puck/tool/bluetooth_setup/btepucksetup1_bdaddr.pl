#! /usr/bin/perl
use strict;

my $inputfile=(shift || 'epuck_bdaddr');
open(INFILE, $inputfile) or die "ERROR: Cannot open input file ", $inputfile;
open(OUTFILE, '>', '/etc/bluetooth/epuck_bdaddr') or die "ERROR: Cannot open output file";

my @epuck;
my %bymac;
my $cc=0;
while (my $line=<INFILE>) {
	while (chomp $line) {}
	
	$line=~/e-puck_(\d+)/ || next;
	my $epucknr=$1;
	if ($epuck[$epucknr]) {print $epucknr, ': duplicate entry', "\n";}
	$epuck[$epucknr]=$line;
	
	$line=~/([0123456789ABCDEF\:]{17,17})/i || next;
	my $epuckmac=$1;
	if (exists $bymac{$epuckmac}) {print $epucknr, ', ', $bymac{$epuckmac}, ': same mac address', "\n";}
	$bymac{$epuckmac}=$epucknr;

	$cc++;
}
print 'Read from ./epuck_bdaddr: ', $cc, ' entries', "\n";

my $cc=0;
foreach my $line (@epuck) {
	next if (! $line);
	print OUTFILE $line, "\n";
	$cc++;
}
print 'Written to /etc/bluetooth/epuck_bdaddr: ', $cc, ' entries', "\n";
