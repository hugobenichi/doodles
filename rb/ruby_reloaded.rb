def fl(file_name)
  file_name += '.rb' unless file_name =~ /\.rb/
  @@recent = file_name 
  load "#{file_name}"
end
 
def rl
  fl(@@recent)
end

def rt
  rl
  eval(choose_last_command)
end

# prevent 'rt' itself from recursing. 
def choose_last_command
  real_last = Readline::HISTORY.to_a[-2]
  real_last == 'rt' ? @@saved_last :  (@@saved_last = real_last)
end

def ls
  %x{ls}.split("\n")
end

alias _ choose_last_command

