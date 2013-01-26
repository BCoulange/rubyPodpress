# script d'upload d'un fichier
require 'soundcloud'

client_id,client_secret,username,password=IO.read("credentials").split("\n")

# register a new client, which will exchange the username, password for an access_token
# NOTE: the SoundCloud API Docs advise not to use the user credentials flow in a web app.
# In any case, never store the password of a user.
client = Soundcloud.new({
  :client_id      => client_id,
  :client_secret  => client_secret,
  :username       => username,
  :password       => password
})


# print logged in username
puts client.get('/me').username#

# upload a new track with track.mp3 as audio and image.jpg as artwork
track = client.post('/tracks', :track => {
  :title        => ARGV[1],
  :asset_data   => File.new(ARGV[0])
})

# print new tracks link
puts track.permalink_url