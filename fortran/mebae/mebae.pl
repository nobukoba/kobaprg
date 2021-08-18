use strict;
use Image::BMP;
my $img = new Image::BMP;
$img->open_file('mebae.bmp');
my ($iW, $iH) = ($img->{Width}, $img->{Height});
print $iW, $iH, "\n";
for( my $iY = 0; $iY < $iH; ++$iY){
 for( my $iX = 0; $iX < $iW; ++$iX){
     my ($r,$g,$b) = $img->xy_rgb($iX,$iY);
     if ($r > 0) {
	 print "1"; 
     }else{
	 print "0"; 
     }
 }
 print "\n";
}
