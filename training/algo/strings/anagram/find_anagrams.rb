#!/usr/local/bin/ruby

#dictionary of English word in /usr/share/dict/american-english

class String
    def signature
        self.each_char.sort.join("").to_sym
    end
end


dictionary = Enumerator.new{ |out|
    File.open(ARGV[0],"r") do |file| 
        out.yield file.readline.chomp.downcase until file.eof?
    end
}.reject{|l| l[-2] == "'" }


anagrams = {}


dictionary.each{ |x|
    hash = x.signature
    anagrams[hash] ||= []
    anagrams[hash] << x
}


anagrams.each do |signature, words| puts "%s: %w{%s}" % [signature, words.join(" ")] end
