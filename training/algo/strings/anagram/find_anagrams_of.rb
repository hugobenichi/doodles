#!/usr/local/bin/ruby

word  = ARGV[0]
anagrams_file = ARGV[1] || "anagrams"


sign_to_match = word.each_char.sort.join("") #.to_sym


File.open(anagrams_file, 'r') do |file|
    until file.eof?
        dontcare, signature, words = *file.readline.match(/:(.+) %w\{(.+)\}/)
        if signature == sign_to_match
            puts words.split " "
            break
        end
    end
end
