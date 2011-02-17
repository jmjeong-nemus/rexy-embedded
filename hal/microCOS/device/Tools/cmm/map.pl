# ARGV[0] : Directory Name, SMDK545_target
# ARGV[1] : Build Type, Releae / Debug
# ARGV[2] : ARM Mode, ARM / THUMB

if( @ARGV[0] eq 0 ) { die("Null Argument 0"); }
if( @ARGV[1] eq 0 ) { die("Null Argument 1"); }
if( @ARGV[2] eq 0 ) { die("Null Argument 2"); }

$Path = join("\\", '.\\..', @ARGV[0], 'build\\Obj\\ARM_4T', @ARGV[1], @ARGV[2], 'Libraries\\Net\\Interfaces\\FakeEth\\');
$Name = join("", 'Sam_S3C2410_Full_enUS_ADS_', @ARGV[2], '_160x220_', @ARGV[1], '.rpt');
$FullName = $Path.$Name;

system "echo $FullName";

open FH, $FullName or die;
$base = 0x20030000 + 0x550;
$name = 'Boot';

while (<FH>)
{
	$_ =~ m/([0-9]+) bytes used by \'(.*)\'/;
	$end = $base + $1;
	$name = $2;
	print sprintf("%33s : 0x%08X\n", $name, $base);
	$base = $end;
}

close FH;

