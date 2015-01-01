

### PARAMETERS ###


namespace :compile do

  flags = "-O3"
  #flags << " -ffast-math -msse2 -mfpmath=387"
  #flags << " -fstrict-aliasing -std=c99"
  #flags << " -DUSE_RESTRICT"

  dir_ext = './ext/'
  dir_inc = dir_ext
  dir_mcr = dir_ext + 'macro/'
  dir_typ = dir_ext + 'types/'

  macros  = [
              'reduce_operator.h',
              'linear_operator.h',
              'loop_template.h'
            ].map{ |file| dir_mcr + file }

  lib_src = FileList[  dir_typ + '*.c' ]
  lib_obj = lib_src.map{ |src| src.sub('types','objects').sub('.c','.o') }
  lib_trg = lib_src.map{ |src| src.sub('types/','bin/lib').sub('.c','.so') }
  
  [lib_src, lib_obj, lib_trg].transpose.each do |chain|
    src, obj, trg = *chain 
    file trg => obj           do sh "gcc -shared -o %s %s -lm" % [trg,obj]  end
    file obj => [src]+macros  do sh "gcc -c -o #{obj} #{src} #{flags} -I#{dir_inc}" end
  end

  task :all     =>  lib_trg

end


task :compile => 'compile:all'


