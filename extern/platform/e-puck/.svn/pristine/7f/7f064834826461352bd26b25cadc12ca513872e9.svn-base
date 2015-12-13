#! /usr/bin/perl
use strict;

# Get device from arguments
my $device=shift;
$device=$1 if ($device=~/^\s*(.*)\s*/);
if ($device =~ /^\d+$/) {
	$device='/dev/rfcomm'.$device;
} elsif ($device =~ /^epuck(\d)$/) {
	$device='/dev/rfcomm'.$1;
}
&help() if (! $device);

# Upload that file
BootloaderListener->new($device);
use Time::HiRes;

# Prints a help text
sub help {
	print 'E-Puck bootloader', "\n";
	print 'Usage: epuckbootloader DEVICE FILE', "\n";
	exit(1);
}


package BootloaderListener;
use Device::SerialPort;

sub new {
	my $o=shift;
	my $class=(ref($o) || $o);
	my $newo=bless {}, $class;
	$newo->init(@_);
	return $newo;
}

sub init {
	my $o=shift;
	my $device=shift;

	# Connecting
	print 'Connecting to ', $device, "\n";
	my $com=Device::SerialPort->new($device) || die 'Unable to open serial port '.$device;
	$com->stty_icrnl(0);
	$com->stty_icanon(0);
	$com->is_stty_kill(3);  # kill char because of NAK
	#$com->are_match("\n");  # possible end strings
	$com->lookclear;        # empty buffer
	$com->databits(8);
	$com->baudrate(115200);
	$com->parity('none');
	$com->stopbits(1);
	$o->{'device'}=$device;
	$o->{'com'}=$com;

	# Wait for initialization
	$o->log_info('Waiting');
	$o->{'buffer'}='';
	while (1) {
		$o->readserial();
		if (index($o->{'buffer'}, chr(193))>-1) {last;}
		$o->{'buffer'}='';
	}

	my $cc=0;
	$com->write('qK');
	$o->{'buffer'}='';
	my $packetlen=101;
	while (1) {
		$o->readserial();
		if (length($o->{'buffer'})==$packetlen) {
			#Time::HiRes::usleep(100000);
			$com->write('K');
			$o->{'buffer'}='';
			$cc++;
			if (($cc==90) || ($cc==345) || ($cc==600) || ($cc==859)) {
				$packetlen=100;
			} else {
				$packetlen=101;
			}
			print ' >> ', $cc, "\n";
		}
	}
	return;
}

sub writebyte {
	my $o=shift;
	my $com=$o->{'com'};
	my $byte=shift;
	$com->write(chr($byte));
	print '<', $byte, '>';
	return;
}

sub readserial {
	my $o=shift;
	my $com=$o->{'com'};

	while (my $input=$com->input) {
		while (chomp $input) {}
		next if (! $input);
		$o->log_receive($input, length($o->{'buffer'}));
		$o->{'buffer'}.=$input;
	}

	return;
}

sub makereadable {
	my $o=shift;
	my $t=shift;
	#$t=~s/([\x00-\x1F])/'<'.ord($1).'>'/eg;
	#$t=~s/([\x80-\xFF])/'<'.ord($1).'>'/eg;
	$t=~s/([\x00-\xFF])/&tohex(ord($1)).' '/eg;
	return $t;
}

sub tohex {
	my $var=shift;
	my $var1=$var/16;
	my $var2=$var % 16;
	my @arr=(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 'A', 'B', 'C', 'D', 'E', 'F');
	return $arr[$var1].$arr[$var2];
	
}

sub log_line {
	my $o=shift;
	my @tt=localtime();
	$tt[0]='0'.$tt[0] if ($tt[0]<10);
	$tt[1]='0'.$tt[1] if ($tt[1]<10);
	$tt[2]='0'.$tt[2] if ($tt[2]<10);
	print '[', $tt[2], ':', $tt[1], ':', $tt[0], '] ', @_, "\n";
	return;
}

sub log_colorline {
	my $o=shift;
	my $color=shift;
	my @tt=localtime();
	$tt[0]='0'.$tt[0] if ($tt[0]<10);
	$tt[1]='0'.$tt[1] if ($tt[1]<10);
	$tt[2]='0'.$tt[2] if ($tt[2]<10);
	print chr(033), '[', $color, 'm', '[', $tt[2], ':', $tt[1], ':', $tt[0], '] ', @_, chr(033), '[0m', "\n";
	return;
}

sub log_info {
	my $o=shift;
	my $t=join('', @_);
	$t=$o->makereadable($t);
	$o->log_colorline('0', 'INFO: ', $t);
	return;
}

sub log_error {
	my $o=shift;
	my $t=join('', @_);
	$t=$o->makereadable($t);
	$o->log_colorline('31;1', 'ERROR: ', $t);
	return;
}

sub log_receive {
	my $o=shift;
	#my $t=join('', @_);
	my $t=shift;
	my $len=shift;
	$t=$o->makereadable($t);
	$o->log_colorline('33', 'RECEIVED ', $len, ': ', $t);
	return;
}

sub log_send {
	my $o=shift;
	my $t=join('', @_);
	$t=$o->makereadable($t);
	$o->log_colorline('35', 'SENT: ', $t);
	return;
}

