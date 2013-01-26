# script qui génère des jolies images compressées selon Fourier
require 'RMagick'

include Magick

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


	# Ajout de texte descriptif
	i = ImageList.new("#{foldSave+evenize(nbCoeffs.to_i).to_s}.jpg")
	txt = Draw.new
		i.annotate(txt, 0,0,0,0,"#{evenize(nbCoeffs.to_i).to_s} rayures"){
		txt.gravity = Magick::SouthGravity
		txt.pointsize = 25
		txt.stroke = '#000000'
		txt.fill = '#ffffff'
		txt.font_weight = Magick::BoldWeight
	}
	i.format = 'jpeg'

	i.write("#{foldSave+evenize(nbCoeffs.to_i).to_s}.jpg")


	nbCoeffs*=1.2

end


