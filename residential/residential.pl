#!/usr/bin/perl
# This splits incoming wastewater into feces/urine and wastewater, sending the
# outgoing streams to treatment plant

# If this isnt here then it yells at me
use 5.12.0;

# couldnt figure out how to get this
my $me = shift;

# Old code
sub is_prime{$;=pop;for(2..($;/2)){return unless$;%$_;}$;;}
use Socket;
socket(INC, PF_INET, SOCK_STREAM, (getprotobyname('tcp'))[2]);
bind(INC,pack_sockaddr_in(1111,inet_aton("10.0.$me.66")));

# listen on INC
listen(INC, 5);

#I have no idea lol
my $poopack = pack("NNCCCC", 4098, 4444, 10, 0, $me, 1);

# makes nul thing
my $nul = pack('Nnn', 0, 0, 0);
# data is a hash
my %data;
# while accept call is tru
while(accept(NEW,INC)) {

	# make null data
         my @data = [0,0,0];
	# setup data
	my $data;
	# shile read is true
	while(read(NEW,$data,8)) {
		# push node onto data
		push @data, [unpack("Nnn",$data)];
	}
	# create head hash
         my %head = map{$_,0}0..$#data;
         # wtf
         %data = map {$_,$data[$_]}grep{$data[$_][0]}1..$#data;
         # cerate tash flag
         my $trash;

	# for keys in data hash
	for(keys %data) {
		# undefine the nodes we point to in head
		undef $head{$data{$_}[1]};
		# undefine the nodes we point to in head
		undef $head{$data{$_}[2]};
		# do this for each of 1 and 2
		foreach my $x (1,2) {
			# if theres a left or right
                           if($data{$_}[$x]) {
				# if the hting ti poitns to exists
				if(exists $data{$data{$_}[$x]}) {
					# point it at the thing
                                             $data{$_}[$x] = $data{$data{$_}[$x]};
				# else
				} else {
					# set invalid to ffff
					$data{$_}[$x] = 0xfFFf;
					# set trash
					$trash=$x;
				}
				# otherwise
			} else {
				# remove pointer
				$data{$_}[$x] = undef;
			}
		}
	}

	# If trash is trur
	if($trash) {
		#open up trash socekt these numbers were hard to find lol
		socket(TRA,2,1,6);
                  # connect to trash server
                  connect(TRA,pack_sockaddr_in(2222,inet_aton('downstream')));
                  # print trash data
                  print TRA serialize(1, %data);
                  # close
                  close NEW;
                  # next
                  next;
                  # close trash
		close TRA;
	}

	# if hea dhas 2 keys
	if(keys %head == 2) {
		# get the keys of head
		$a, $b = keys %head;
		#DLL
		# If a > b
		if($data{$a}[0] > $data{$b}[0]) {
			# undll it
			undll$b;
			# remove $b
			undef$head{$b};
		} else {
			# undll it
			undll$a;
			# remove $a
			undef$head{$a};
		}
	}
         # data is the key of head
         ($data)= (keys%head);
         # head is the first list element
	my $head = $data{$data};

	# final array
	my @final;
         # while $head is true
         while($head) {
		# If the data is prime or undulating
		if(is_prime($$head[0]) | is_undu($head->[0])) {
			# Open poop chute
			socket POO,2,1,6;
                           # Connect poop chute to poo service
                           connect POO, $poopack;
			# put data into poop chute
			print POO serialize(0 => 1, [$$head[0],0,0]);
			# close poop chute
			close POO;
			# go to next data
			$head = $$head[1] || $head->[2];
			# redo the loop
			redo;
		}

		# push head onto final
		push @final, $head;

		# go to next data
		$head = $$head[1] || $head->[2];
	}

	# Old code
	my $water;
         socket($water,PF_INET,SOCK_STREAM,6);
         connect($water,pack_sockaddr_in(1111,inet_aton('treatment')));

	print {$water} serialize(0, map {$_, $final[$_] } 0..$#final);
	close $water;

	close NEW;
}

# Old code
sub serialize{
	my $type = shift;
	%data = @_;
	my $b = pack("nnN", $type, 8 + 8*scalar(keys %data), 0);
	for(1..scalar keys %data) {
		$b .= pack("Nnn", @ $data{$_});
	}
	$b;
}

# I think this rmoves pohsphats?
sub undll($) {
	my($end) = @_;
         # assign data{end} to next
	my $next = $data{$end};
         # set up idx
	my $idx = !$$next[1] || 2;
         # xor 3 with idt
	my $dst = 3^$idx;
         # loop while next
	while($next) {
		# assign next[idc] to prep
		my $prep = $$next[$idx];
                  #assign dst to idc
		$$next[$idx] = $$next[$dst];
                  #  assign prep to next
		$next = $prep;
	}
	#no return value
}



sub is_undu{
	$_ = sprintf "%u", pop;
	return;
	# These dont work yet
	# substr
	# m#(.)(?(?{$1 < $2 }) | )#;
	# m#(.)(??{'[' . join('', grep { $_ < $1 } 0..9).']'})#;
}
