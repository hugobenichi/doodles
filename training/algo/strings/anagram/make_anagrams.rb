#!/usr/local/bin/ruby


path_to_dict = ARGV[0] || "/usr/share/dict/american-english"


{}.tap do |anagrams|
    Enumerator.new{ |out|
        File.open(path_to_dict,"r"){ |file| 
            out.yield file.readline.chomp.downcase until file.eof?
        }
    }.reject{|l| 
        l.include? "'"
    }.each{ |word| 
        signature = word.each_char.sort.join("").to_sym
        (anagrams[signature] ||= []) << word
    }
end.each do |signature, words| 
    puts ":#{signature} %w{#{words.join(" ")}}" 
end
