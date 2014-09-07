#!/usr/local/bin/ruby


links = File.open(ARGV[0], 'r').readlines.map{ |link|       # relit la liste de voisins filtree
    l = link.split /\s+/                                    # resplit sur espace
    [
        l[0].to_sym,
        l[1].split(",").map(&:to_sym),
        l[2].split(",").map(&:to_f)
    ]
}

freq = {}                                                   # prepare un tableau
links.each{ |l|                                             # separe avec tt les mots
    l[1].each.with_index{ |word,index|                      # et leur frequence
        freq[word] ||= l[2][index]
    }
}


pairs = {}                                                  # stock les paires

links.each{ |l|
    l[1].each.with_index{ |word,index|
        pair = [l[0], word].sort                            # tries les pairs pour virer les doubles
        if freq[l[0]] && freq[word]                         # certains mots n ont pas leur freq 
            pairs[pair.join.to_sym] ||= [pair, [freq[l[0]], freq[word]]]   # insert la paire si inexistante
        end
    }
}


# trie les paires par valeur de distance et imprime sur stdout
pairs
    .values
    .map{|pair, freqs|
#        [pair, (freqs[0] - freqs[1]).abs]       # distance relative
        [pair, (freqs[0]*freqs[1]).**(0.5)]     # moyenne geometrique
    }
    .sort_by{|pair, metric| metric}
    .each{ |pair, dist| puts "%s %f" % [pair.join(","), dist] }
