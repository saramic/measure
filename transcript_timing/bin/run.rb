#!/usr/bin/env ruby

filename = ARGV[0]

stats = {}
last_minutes = last_seconds = last_person = last_word_count = nil

File.open(filename).each do |line|
  matches = /(^\d+):(\d+)\s([^:]+):(.*)/.match(line)
  if matches
    minutes = matches[1].to_i
    seconds = matches[2].to_i
    person = matches[3]
    word_count = matches[4].split.count
    if last_minutes && last_seconds && last_person
      stats[last_person] ||= { seconds: 0, words: 0 }
      now = minutes * 60 + seconds
      last_now = last_minutes * 60 + last_seconds
      stats[last_person][:seconds] += now - last_now
      stats[last_person][:words] += last_word_count
    end
    last_minutes = minutes
    last_seconds = seconds
    last_person = person
    last_word_count = word_count
  end
  # will miss the last spoken line?
end

total_talk_seconds = stats
  .map{|person,_value| stats[person][:seconds] }
  .sum
stats.keys.each do |person|
  stats[person][:words_per_second] = (
    stats[person][:words].to_f / stats[person][:seconds]
  ).round(3)
  stats[person][:percentage_talking] = format(
    "%<time>.2f %%",
    time:(
      (
        stats[person][:seconds].to_f / total_talk_seconds
      ) * 100
    ).round(2)
  )
end

pp stats
