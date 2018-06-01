#!/usr/bin/perl
use strict;
#use warnings;

my $file1;
my $file2;
my $message = "";

if($ARGV[0] eq undef || $ARGV[1] eq undef)
{
    $file1 = "None";
    $file2 = "None";
}
else
{
    $file1 = $ARGV[0];
    $file2 = $ARGV[1];
}

if($file1 eq "None")
{
    die "You need to provide two files to compare!";
}

#read both files into arrays
# open the files for reading
open(my $old, "<", $file1) || die "Cannot open $file1 for reading!";
open(my $new, "<", $file2) || die "Cannon open $file2 for reading!";

#get old file's last modified date
my $epoch_timestamp1 = (stat($file1))[9];
my $timestamp1 = localtime($epoch_timestamp1);

#get new file's last modified date
my $epoch_timestamp2 = (stat($file2))[9];
my $timestamp2 = localtime($epoch_timestamp2);

#read old file's lines and store them in an array
my @oldInput = <$old>;
my $oldLines = @oldInput;
close $old;

#read new file's lines and store them in an array
my @newInput = <$new>;
my $newLines = @newInput;
close $new;

print "Comparing $file1 to $file2\n----------------------------------\n";
print "$file1 last edited: $timestamp1\n";
print "with $oldLines lines!\n\n";

print "$file2 last edited: $timestamp2\n";
print "with $newLines lines!\n\n";

$message = $message . "Comparing $file1 and $file2\n";
$message = $message . "\t$file1:\t$file2:\n";
$message = $message . "Date:\t$timestamp1\t$timestamp2\n";
$message = $message . "Lines:\t$oldLines\t$newLines\n\n";

my @added;
my @removed;
my @changed;
my @common;

#convert newInput to map
my %newInputMap = map {$_ => 1} @newInput;

#find the common
for(@oldInput)
{
    if(exists($newInputMap{$_}))
    {
        #print $_;
        push(@common, $_);
    }
}

#make a map of the commons to easily check stuff
my %commonMap = map {$_ => 1} @common;

#check for what was "removed"
for(@oldInput)
{
    if(!exists($commonMap{$_}))
    {
        #print $_;
        push(@removed, $_);
    }
}

#check for what was "added"
for(@newInput)
{
    if(!exists($commonMap{$_}))
    {
        #print $_;
        push(@added, $_);
    }
}

#now we have dig through the added and removed arrays
#to figure out what was changed from each file.
my @temp1 = @added;
my @temp2 = @removed;
my $score_exact = 0.0;
my $score_succession = 0.0;

foreach my $a (@removed)
{
    foreach my $b (@added)
    {
        chomp($a);
        chomp($b);
        my @oldWords = split(/ /, $a);
        my @newWords = split(/ /, $b);
        my $oldLength = @oldWords;
        my $newLength = @newWords;
        
        if($oldWords[0] eq $newWords[0])
        {
            #print "$a <-> $b\n";
            while(@oldWords)
            {
                my $j = shift @oldWords;
                my $i = shift @newWords;
                chomp($j);
                chomp($i);
                if($j eq $i)
                {
                    $score_exact = $score_exact + 1.0;
                    $score_succession = $score_succession + 1.0;
                }
                else
                {
                    $score_exact = $score_exact - 1.0;
                    $score_succession = 0.0 #$score_succession - 1.0;
                }
            }
            #print ($score_exact + $score_succession); #/ $oldLength;
            #rint "\n";
            #print ($score_exact + $score_succession); #/ $newLength;
            #print "\n\n";
            
            if(($score_exact + $score_succession) > 0)
            {
                push(@changed, $a);
                push(@changed, $b);
            }
            $score_exact = 0;
            $score_succession = 0;
        }
    }
}
my %changedMap = map {$_ => 1} @changed;
my @tempArray;
while(@added)
{
    #print "Here\n";

    my $current = shift @added;
    
    if(exists($changedMap{$current}))
    {
        #do nothing
    }
    else
    {
        push(@tempArray, $current);
    }
}

@added = @tempArray;
@tempArray = ();

while(@removed)
{
    my $current = shift @removed;
    
    if(exists($changedMap{$current}))
    {
        
    }
    else
    {
        push(@tempArray, $current);
    }
}



@removed = @tempArray;
@tempArray = ();

#print "Common:\n";
#print @common;
#print "\n\n";
print "Changed $file1 ---> $file2:\n----------------------------------\n";
$message = $message . "\nItems changed from $file1 to $file2:\n";
while (@changed)
{
    my $old = shift @changed;
    my $new = shift @changed;
    $message = $message . "$old ---> $new\n";
    print "$old ---> $new\n";
}

print "\n";
print "Removed from $file1:\n----------------------------------\n";
$message = $message . "\nItems removed from $file1:\n";
foreach (@removed)
{
    print "$_\n";
    $message = $message . "$_\n";
}

print "\n";
print "Added to $file2:\n----------------------------------\n";
$message = $message . "\nItems added from $file1 to $file2:\n";
foreach (@added)
{
    print "$_\n";
    $message = $message . "$_\n";
}

print "\nDone! Log saved under compare_output.txt!\n----------------------------------\n";

#write data to the output file, close output file and exit program
open(my $output, ">", "compare_output.txt");
print $output $message;
close $output;

exit 0;