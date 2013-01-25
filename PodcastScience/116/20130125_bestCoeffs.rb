# script qui génère des jolies images compressées selon Fourier
require 'RMagick'

def evenize(number)
	if number%2==0 then
		return number
	else
		return number+1
	end
end

foldSave=ARGV[1]
inFile=ARGV[0]

foldSave+="/" unless foldSave[-1]=="/"

i = Magick::ImageList.new(inFile)
nbCoeffMax=i.columns*i.rows

Dir.mkdir(foldSave) unless File.exists?(foldSave) 


nbCoeffs=2
while nbCoeffs < nbCoeffMax
	`./keepBestnDFTCoeffs #{inFile} #{nbCoeffs} #{foldSave+evenize(nbCoeffs.to_i).to_s}.jpg`
	nbCoeffs*=1.2


end
