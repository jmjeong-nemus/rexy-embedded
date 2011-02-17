sub getNewAddr; # arg0 : the location of map file.

if( @ARGV eq 0 ) { die("Null Argument"); }

$globalPath 	= join("\\", '.\\..', @ARGV[0]);
$smallmapPath	= $globalPath.'\\build\\ROM\\Sam_S3C2410_Full_enUS_ADS_THUMB_160x220_Debug.smallmap';
$mapPath		= $globalPath.'\\build\\ROM\\Sam_S3C2410_Full_enUS_ADS_THUMB_160x220_Debug.map';
#$mapPath		= $globalPath.'\\build\\ROM\\SMIT_Galaxy_Debug.map';

$bigDAL		= join( ":", $mapPath,	'DAL.prc',				'Reset',		'build\\ROM\\AXF\\Sam_S3C2410\\Debug\\THUMB\\160x220\\DAL.axf');
$serial		= join( ":", $mapPath,	'2410SerialDriver.bprc','PilotMain',	'build\\ROM\\AXF\\Sam_S3C2410\\Debug\\THUMB\\160x220\\2410SerialDriver.axf');
$RamSlot	= join( ":", $mapPath,	'RAMSlotDrvr.prc',		'PilotMain',	'build\\ROM\\AXF\\Sam_S3C2410\\Debug\\THUMB\\160x220\\RAMSlotDrvr.axf');

open (FHSYM, ">", '.\sym.cmm') or die ("can't make sym.cmm\n");

print FHSYM "y.res\n\n";

foreach $files ( $bigDAL, $serial )
{
	@prc = split( /:/, $files );
	$mapfile = $prc[0];
	$prcname = $prc[1];
	$mapsym  = $prc[2];
	$axffile = $prc[3];
	if ( $axffile eq undef or $mapfile eq undef or $prcname eq undef or $mapsym eq undef )
	{
		die ("wrong parameter.\naxf = $axffile\nmapfile = $mapfile\nprcname = $prcname\nmapsym = $mapsym\n");
	}
	
	#############################################
	# get addr of entry							#
	#############################################
	$entryaddr = getNewAddr($mapfile);
	$tstr = sprintf ("$prcname\'s entry: 0x%08X\n",$entryaddr);
	print $tstr;
	print FHSYM sprintf("d.load.elf $globalPath\\$axffile 0x%08X /nocode /noclear\n", $entryaddr);
}

close ( FHSYM );

#======================================================================================
sub getNewAddr
{
	my $mpfile = $_[0];
	my $addr = 0;
	open ( FHMAP, "<", $mpfile ) or die ("can't open $mpfile\n");
	
	while( <FHMAP> ){ last if ( /^#.*$prcname/ ); }
	if ( eof FHMAP ){ die ("can't file $prcname in $mpfile.\n") };
	
	while ( <FHMAP> ) 
	{
		if ( /^\s*$mapsym\s*([0-9A-Fa-fxX]+)/ )
		{
			$addr = hex($1);
			last;
		}
		last if ( /^#/ ); 
	}
	close ( FHMAP );
	if ( $addr == 0 ) { die ("can't read $prcname\'s $mapsym addr\n" )};
	if ( $addr < 0x20000000 or $addr > 0x30000000 ) 
	{
		die sprintf("wrong addr. addr: 0x%08X\n",$addr);
	}
	return $addr;
}
