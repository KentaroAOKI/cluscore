#!/usr/bin/perl
#
#  Copyright (c) 2008-2009 Kentaro Aoki
#  Copyright (c) 2009 ClusCore
#
#  http://www.cluscore.com/
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# Generates the new class of ClusCore.
#
# Author: Kentaro Aoki

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
