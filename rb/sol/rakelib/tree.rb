
namespace :tree do

  def lookup( dir )
    tree    = {} 
    subdirs = []
    dir.grep(/\A[^\.]/).each do |entry|  #filter away the .git subdirectory
      if File.directory?(dir.path+entry)
        subdirs << entry
      else
        tree[entry] = nil
      end
    end
    subdirs.each { |subdir| tree[subdir] = lookup Dir.new "#{dir.path}#{subdir}/" }
    tree  
  end

  def ptree( tree, offset, target)
    tree.each_pair do |node,subtree|
      if subtree
        target.puts "\n%s+ %s" % [offset,node]
        ptree( subtree, offset+"  ", target )
      else
        target.puts "%s- %s" % [offset,node]
      end
    end
  end

  task :print do
    File.open('TREE','w') { |file| ptree(lookup(Dir.new './'),"",file) }
  end

end


task :tree => 'tree:print'


