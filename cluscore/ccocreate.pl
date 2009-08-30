#!/usr/bin/perl

my $count;
my $super;
my $class;
my $classname;
my $classbig;
my $superprops;
my $hidesuperprops;

$count = $#ARGV + 1;
if ($count != 2) {
	print "usage: ccocreate.pl <superclass> <class>\n";
	exit;
}
$super = $ARGV[0];
$class = $ARGV[1];
$classbig = uc $ARGV[1];
if (! -e $super.".h") {
	print "error: undefined a super class, ".$super."\n";
	exit;
}
if (-e $class.".h") {
	print "error: already exists a class, ".$class."\n";
	exit;
}

$class =~ m/^cco_([a-zA-Z0-9_]+)$/;
$classname = $1;

open(FILE, $super.".h");
while(<FILE>){
	if ($_ =~ m/^CCOPERTIES\:([A-Z0-9_]+)$/) {
		$superprops = $superprops."\t".$1."\n";
		$hidesuperprops = $hidesuperprops."CCOPERTIES:".$1."\n";
	}
}
close(FILE);

open(FILE, "cco_SKELETON_h.text");
open(WFILE, ">".$class.".h");
while(<FILE>){
	$_ =~ s/\@CCOSKELETONSUPERPROPERTIES\@/$superprops/g;
	$_ =~ s/\@CCOSKELETONHIDESUPERPROPERTIES\@/$hidesuperprops/g;
	$_ =~ s/\@CCOSKELETONBIGNAME\@/$classbig/g;
	$_ =~ s/\@CCOSKELETONLITTLENAME\@/$class/g;
	$_ =~ s/\@CCOSKELETONLITTLENAMEONLY\@/$classname/g;
	$_ =~ s/\@CCOSKELETONLITTLESUPERNAME\@/$super/g;
	print WFILE $_;
}
close(WFILE);
close(FILE);

open(FILE, "cco_SKELETON_c.text");
open(WFILE, ">".$class.".c");
while(<FILE>){
	$_ =~ s/\@CCOSKELETONSUPERPROPERTIES\@/$superprops/g;
	$_ =~ s/\@CCOSKELETONHIDESUPERPROPERTIES\@/$hidesuperprops/g;
	$_ =~ s/\@CCOSKELETONBIGNAME\@/$classbig/g;
	$_ =~ s/\@CCOSKELETONLITTLENAME\@/$class/g;
	$_ =~ s/\@CCOSKELETONLITTLENAMEONLY\@/$classname/g;
	$_ =~ s/\@CCOSKELETONLITTLESUPERNAME\@/$super/g;
	print WFILE $_;
}
close(WFILE);
close(FILE);





#  $rcd = -e "path/filename";

#  open(FILE, "test.txt") or die;
#  @rec = <FILE>;
#  close(FILE);

#  open(FILE, ">test.txt") or die;
#  print FILE, "aaaaa", "bbbbb", "\n";
#  close(FILE);

