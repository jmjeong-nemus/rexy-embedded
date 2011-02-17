# ARGV[0] : Directory Name, SMDK545_target
# ARGV[1] : Build Type, Releae / Debug
# ARGV[2] : ARM Mode, ARM / THUMB

sub getNewAddr; # arg0 : the location of map file.

if( @ARGV[0] eq 0 ) { die("Null Argument 0"); }
if( @ARGV[1] eq 0 ) { die("Null Argument 1"); }
if( @ARGV[2] eq 0 ) { die("Null Argument 2"); }



$globalPath 	= join("\\", '.\\..', @ARGV[0]);
$localMapPath		= join("", '\\build\\ROM\\Sam_S3C2410_Full_enUS_ADS_'.@ARGV[2].'_160x220_'.@ARGV[1]);
$spkSymPath		= join("\\", 'build\\ROM\\AXF\\Sam_S3C2410', @ARGV[1], @ARGV[2], '160x220\\');
$palmSymPath	= join("\\", 'ROM\\AXF\\ARM_4T', @ARGV[1], @ARGV[2]);

$smallmapPath	= $globalPath.$localMapPath.'.smallmap';
$mapPath		= $globalPath.$localMapPath.'.map';

$smallDalSym	= $spkSymPath.'SmallDAL-1X.axf';
$bigDalSym		= $spkSymPath.'DAL.axf';
$serialSym		= $spkSymPath.'2410SerialDriver.axf';
$systemSym		= $palmSymPath.'System.axf';
$usbSym			= $spkSymPath.'2410USBDriver.axf';



$bigDAL		= join( ":", $mapPath,	'DAL.prc',				'Reset',		$bigDalSym);
$serial		= join( ":", $mapPath,	'2410SerialDriver.bprc','PilotMain',	$serialSym);
$system		= join( ":", $mapPath,	'System.bprc',			'SystemMain',	$systemSym);
$usb		= join( ":", $mapPath,	'2410USBDriver.bprc',	'PilotMain',	$usbSym);


open (FHSYM, ">", '.\spk_sym.cmm') or die ("can't make sym.cmm\n");

#print FHSYM "y.res\n\n";

print FHSYM "d.load.elf $globalPath\\$smallDalSym /nocode /noclear\n";

foreach $files ( $bigDAL, $serial, $usb )
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
