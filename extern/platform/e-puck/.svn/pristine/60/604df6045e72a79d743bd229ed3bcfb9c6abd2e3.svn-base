#! /usr/bin/perl
use strict;

my @devices;
while (my $devicename=shift) {
	$devicename='/dev/rfcomm'.$devicename if ($devicename=~/^\d+$/);
	push @devices, {'name'=>$devicename};
}

my $nok=1;
while ($nok) {
	$nok=0;
	foreach my $device (@devices) {
		next if ($device->{'ok'});
		undef $@;
		undef $!;
		print 'Opening ', $device->{'name'}, ' ...', "\n";
		if (open(my $fh, '+<', $device->{'name'})) {
			$device->{'ok'}=1;
			$device->{'fh'}=$fh;
			print $device->{'name'}, ' - OK ', "\n";
		} else {
			$nok=1;
			print $device->{'name'}, ' - ', $@, ' - ', $!, "\n";
		}
	}
	last if ($nok==0);
	
	my $sl=rand()*10;
	print 'Sleeping for ', $sl, ' seconds', "\n";
	sleep $sl;
}
print 'All devices are open now.', "\n";

for (my $i=0; $i<100; $i++) {
	foreach my $device (@devices) {
		my $fh=$device->{'fh'};
		print $fh "A\n";
		my $line=<$fh>;
		while (chomp $line) {}
		print $line, "\n";
	}
}

print 'Closing ...', "\n";
foreach my $device (@devices) {
	close $device->{'fh'};
}
print 'All devices closed.', "\n";

__END__
./rfcommtest.pl 77 &
./rfcommtest.pl 59 &
./rfcommtest.pl 57 &
./rfcommtest.pl 140 &
./rfcommtest.pl 64 &
