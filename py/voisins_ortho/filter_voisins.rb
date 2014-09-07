#!/usr/local/bin/ruby


puts File.open(ARGV[0],"r").readlines                               # lit tt le fichier ligne par ligne
        .select{ |l| l.ascii_only? }                                # garde les lignes ascii
        .map{|l| l.gsub(/,\s+/, ",").split(/\s+/) }                 # cole ensemble les sous-listes
        .select{ |l| ( l[0].length == 4 ) && ( l[1].to_i != 0 ) }   # garde mots taille 4 avec voisins
        .map{ |l| [l[0], l[2], l[3]].join " " }                     # vire les trucs useless
