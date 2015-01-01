

module SoL

  require 'ffi'

  module AutoWrapper
    class << self

      def get_prototype(args)
        [ get_name(args), get_input(args[:arty]) , :void ]
      end

      def get_name(args)
        "sol_#{args[:type]}_#{args[:operation]}#{args[:arty]}_#{args[:name]}".to_sym
      end

      def get_input(arty)
        [ :pointer, :pointer, :pointer, :int].tap{ |sig| sig.shift if arty == 1}
      end

      def pattern
        name = 'def_(linear|reduce)_(arty(\d)|hete)'
        args = '\(\s*([^,]*)\s*,\s*([^,]*)\s*,([^,]*|[^,]*\s*,\s*[^,]*)\s*\)'
        /#{name}#{args}/
      end

      def parse_header(path)
        File.open(path,'r') do |file|
          file.readlines.select{ |line| line.include? 'def_' }.map!{ |line|
                line =~ pattern
                {
                  :operation  =>  $~[1], 
                  :type       =>  $~[5],
                  :arty       =>  $~[3].nil? ? '_hete' : $~[3].to_i,
                  :name       =>  $~[4],
                }
              }.map!{ |args| get_prototype(args) }
          #file.readlines                                             # 1.9 Hash syntax
          #    .select!{ |line| line.include? 'def_' }                # not compatible
          #    .map!{ |line|                                          # with Rubinius
          #      line =~ pattern
          #      {
          #        operation:  $~[1], 
          #        type:       $~[5],
          #        arty:       $~[3].nil? ? '_hete' : $~[3].to_i,
          #        name:       $~[4],
          #      }
          #    }
          #    .map!{ |args| get_prototype(args) }
        end
      end

      def make_module( name, path)
        SoL.const_set( name.capitalize.to_sym, Module.new do
puts "making wrapper module for #{name} -> #{name.capitalize.to_sym}"
          extend FFI::Library
          ffi_lib './ext/bin/lib%s.so' % name
          eigen = (class << self; self; end)
          AutoWrapper::parse_header(path).each{ |prototype| 
            attach_function *prototype
            tag = prototype[0].to_s.sub("sol_#{name}_","").to_sym            
            eigen.send( :define_method, tag) { |*args| send( prototype[0], *args) } #bug
          }.tap{ |meths| eigen.send(:define_method, :c_meths) { meths } }
        end )
      end

      def make_all(path)
        Dir.new(path).each do |entry|
          entry =~ /(.*)\.c/
          make_module( $~[1], path + '/' + entry) if $~
        end
      end

    end
  end

end


SoL::AutoWrapper::make_all './ext/types'


