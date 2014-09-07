module Quix

  class DMatrix < FFI::ManagedStruct

    RedPlot::insert(in: self, as: :plot, command: 'splot') do
      set.zrange '[-1:1]'
      set.ticslevel -0.5
      set.style.line(1).lw(5).palette
      set.style.line(2).lw(5).lc.rgb '"red"'
      set.style.line(3).lw(5).lc.rgb '"cyan"'
      set.palette 'defined (-1.0 "red",0.0 "yellow",1.0 "red")'
      #set.palette 'defined(-1.0 "blue",0.0 "cyan",1.0 "blue")'
    end

    include Quix_Object

    [
      [:new,            [:int],                       :pointer],
      [:free,           [:pointer],                   :void],
      [:copy,           [:pointer],                   :pointer],
      [:to_zero,        [:pointer],                   :void],
      [:qubit,          [:int],                       :pointer],
      [:to_Id,          [:pointer],                   :void],
      [:transpose,      [:pointer],                   :void],
      [:conjugate,      [:pointer],                   :void],
      [:diagonal,       [:pointer],                   :pointer],
      [:trace,          [:pointer],                   :double],
      [:renorm,         [:pointer],                   :void],
      [:sym,            [:pointer],                   :void],
      [:avg_phot,       [:pointer],                   :double],
      [:purity,         [:pointer],                   :double],
      [:neg,            [:pointer],                   :double],
      [:prod_alloc,     [:pointer,:pointer],          :pointer],
      [:prod,           [:pointer,:pointer,:pointer], :pointer],
      [:dist_max,       [:pointer,:pointer],          :double],
      [:dist_frob,      [:pointer,:pointer],          :double],
    ].each{ |sig| attach_quix_function sig }

    layout  :n,           :int,
            :re,          :pointer,
            :im,          :pointer

    Quix.todo self, "add something to draw re/im/norm"
    def initialize *args

      super case args[0]
        when FFI::Pointer then args[0] 
        when Fixnum       then Q::dmatrix_new args[0]
      end

      plot.header << "set xrange [-0.5:#{self[:n]}.5]"
      plot.header << "set yrange [-0.5:#{self[:n]}.5]"
      plot.todraw.add('w impulses ls 1 notitle') do
        n, m = [], []
        re.length.times do |i|
          re.length.times do |j|
            n << i
            m << j
          end
        end
        [n,m,re.flatten]
      end

    end

    Quix.todo self, "check read form file"
    def self.from_file path  
      data = File.open(path,"r").readlines
      len  = data.length

      x, y, re, im = Array.new(len), Array.new(len), Array.new(len), Array.new(len) 
      len.times{ |i| x[i], y[i], re[i], im[i] = data[i].split(" ").map{|x| x.to_f} }

      re_val, im_val = [], [] 
      n = x.max.to_i
      m = n+1
      i = 0

      while i < re.length
        re_val << re[i..i+n]
        im_val << im[i..i+n]
        i += m
      end 

      d = DMatrix.new Quix::dmatrix_new(n)
      d[:re].get_array_of_pointer(0, m).each{ |p| p.put_array_of_double(0, re_val.shift) }
      d[:im].get_array_of_pointer(0, m).each{ |p| p.put_array_of_double(0, im_val.shift) }
      d
    end

    def self.from_diag ary
      m = ary.length
      d = DMatrix.new Quix::dmatrix_new m - 1
      d.to_zero
      p = d[:re].get_array_of_pointer(0,m)
      ary.each_with_index do |v,i|
       p[i].put_array_of_double 0, Array.new(i){0.0} + [v] 
      end
      d.renorm
      d
    end

    def re
      m = self[:n]+1
      @re ||= self[:re].get_array_of_pointer(0,m).map!{ |ptr| ptr.get_array_of_double(0,m) }
    end

    def im
      m = self[:n]+1
      @im ||= self[:im].get_array_of_pointer(0,m).map!{ |ptr| ptr.get_array_of_double(0,m) }
    end

    def put_re val
      self[:re].get_array_of_pointer(0, val.length).each_with_index do |ptr,i|
        ptr.put_array_of_double 0, val[i]
      end
      @re = val
    end

    def put_im val
      self[:im].get_array_of_pointer(0, val.length).each_with_index do |ptr,i|
        ptr.put_array_of_double 0, val[i]
      end
      @im = val
    end

    def diag
      data = self.re
      @diag ||= Q::Photon_Distribution.new Array.new(data.length) {|i| data[i][i]}
    end

    def negativity
      neg, sign = 0.0, -1
      diag.each do |x| 
        sign *= -1
        neg += x * sign
      end
      neg / Math::PI
    end

    def minmax      
      r, i = self.re, self.im
      return  r.map{ |col| col.max }.max,
              r.map{ |col| col.min }.min,
              i.map{ |col| col.max }.max,
              i.map{ |col| col.min }.min
    end

    def wigner grid
      Fock.new(grid, self[:n]).wigner self
    end

  end

  DMat = DMatrix

end
