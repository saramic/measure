require "net/http"
require "json"

run lambda { |env|
  res = Net::HTTP.get(URI("https://www.youtube.com/channel/UCVWaSXuexIlR6raOcm3m2Hw"))
  line = res.split("\n").find{|l| l =~ %r{window..ytInitialData..}}
  subscribers = %r{(\d+)\ssubscribers}
    .match(JSON.parse(/window..ytInitialData....(.*);$/.match(line)[1])
    .dig("header", "c4TabbedHeaderRenderer", "subscriberCountText", "simpleText"))[1]
  [200, {'Content-Type'=>'text/plain'}, StringIO.new({subscribers: subscribers}.to_json)]
}
