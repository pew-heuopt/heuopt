#!/usr/bin/perl -w 
#
#
die "$0: <input-file> <filter>" if( scalar(@ARGV) != 2 );


open $fin, $ARGV[0] or die "unable to open input file $ARGV[0] $!\n";

@header_columns= split( /\s+/, <$fin> );

$i=0;
$first= shift @header_columns;
print "$first ";
@indices= ($i++);
foreach $c (@header_columns)
{
    if( $c =~ /$ARGV[1]/ )
    {
        print $c." ";
        push @indices, $i ;
    }
    $i++;
}
print "\n";

foreach $line (<$fin>)
{
    @row_cols= split( /\s+/, $line );

    foreach $index ( @indices )
    {
        print $row_cols[$index] . " ";
    }
    print "\n";
}
