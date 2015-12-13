#!/usr/bin/perl
use strict;

my $basenumber=(shift || 0);

open(INFILE, '/etc/bluetooth/epuck_bdaddr') or die "cannot open input file";

# Delete existing minirc.epuck* files
system('rm /etc/minirc.epuck*');
print 'Existing minirc.epuck* files deleted in /etc', "\n";

# Create new files
my $cc=0;
while (my $line=<INFILE>) {
	next if ($line!~/\s*(\S+)\s+e-puck_(\d+)/);

	my $name = $2;
	my $MAC = $1;
	my $num=int($name);
	next if ($num<$basenumber);
	next if ($num>=$basenumber+200);
	my $rfcommnum=($num % 200);

	open(MINIRC, '>', "/etc/minirc.epuck$num") or die "cannot create file minirc.epuck$num";
	print MINIRC "pr port\t/dev/rfcomm$rfcommnum\n";
	print MINIRC "pr lock\t/var/lock\n";
	print MINIRC "pu baudrate\t115200\n";
	print MINIRC "pu minit\n";
	print MINIRC "pu mreset\n";
	print MINIRC "pu mdialpre\n";
	print MINIRC "pu mdialsuf\n";
	print MINIRC "pu mdialpre2\n";
	print MINIRC "pu mdialsuf2\n";
	print MINIRC "pu mdialpre3\n";
	print MINIRC "pu mdialsuf3\n";
	print MINIRC "pu mconnect\n";
	print MINIRC "pu mnocon1\n";
	print MINIRC "pu mnocon2\n";
	print MINIRC "pu mnocon3\n";
	print MINIRC "pu mnocon4\n";
	print MINIRC "pu mhangup\n";
	print MINIRC "pu mdialcan\n";
	print MINIRC "pu mretries\t1\n";
	print MINIRC "pu rtscts\tNo\n";
	print MINIRC "pu xonxoff\tNo\n";
	close MINIRC;

	$cc++;
}

print $cc, ' minirc.epuck* files created in /etc', "\n";
print '', "\n";
print 'You can now launch minicom with', "\n";
print '	minicom epuckNUM', "\n";
print 'where NUM is the number of the epuck (without leading zeros).', "\n";
