require 'rubygems'
require 'open-uri'
require 'json'
require 'net/http'

# Please note that the vanilla wp-json-api plugin does not support user authentication for create_post.
# Check out my fork for authentication support: https://github.com/Achillefs/wp-json-api
class Article
  API_URI = 'http://mywpblog.com/api/'
  API_USER = 'my_wp_username'
  API_PASS = 'my_wp_pass'
  
  attr_accessor :title, :content, :categories, :tags, :type
  
  def initialize(opts = {})
    # set all values if valid
    opts.each do |key,val|
      begin
        self.send(:"#{key}=",val)
      rescue NoMethodError => e
        raise ArgumentError("#{key} is not a valid article attribute")
      end
    end
    self.type = 'post' if self.type == nil
  end
  
  def publish!
    # Need to get a nonce token from WP in order to create a post
    nonce_response = JSON.parse(open(API_URI + "get_nonce/?controller=posts&method=create_post").read)
    if nonce_response['status'] == 'ok'
      nonce = nonce_response['nonce']
      url = URI.parse(API_URI + "posts/create_post")
      args = { 
        'nonce' => nonce, 'author' => API_USER, 'user_password' => API_PASS, 
        'status' => 'publish', 'title' => self.title, 'content' => self.content, 
        'categories' => self.categories.join(','), 'tags' => self.tags.join(','), 'type' => type
      }
      resp, data = Net::HTTP.post_form(url, args)
      response = JSON.parse(data)
      if response['status'] == 'ok'
        puts response.inspect
      else
        raise response['error'].to_s
      end
    else
      raise nonce_response['error'].to_s
    end
    
  end
end

a = Article.new(
  :title => 'test post', 
  :content => 'hello world', 
  :categories => %W[cat1 cat2],
  :tags => %W[tag1 tag2]
)
a.publish!