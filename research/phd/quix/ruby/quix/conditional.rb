

module Quix

  module Conditional

    attr_accessor :ax, :ap

    class << self

      def from_file path
        data = File.open(path,"r").readlines[0..-2].map{ |s| 
          s.split(" ").map!{ |x| x.to_f } 
        }
        make_cond_margi *data.transpose
      end

      def make_cond_margi phz, vic, ax, ap
        Q::Marginal.new(phz, vic).to_conditional(ax,ap)
      end
    
      alias_method :make, :make_cond_margi

      #
      # assume yield() on block returns a marginal distribution object
      #
      def monte_carlo m = 100, r = 1.0, l = 0.0, g = 1.0
        mc_data = Array.new(m) do |i|
          puts "\ngenerating conditional neg data ##{i+1}\n\n"
          yield().conditional(r,l,g).to_condneg
        end
        average = Cond_Negativity.new
        mc_data[0].frac.each_index do |i|
          r, n, f, n1, s1, n2, s2 = 0.0, 0, 0.0, 0.0, 0.0, 0.0, 0.0
          m.times do |j|
            r  += mc_data[j].rad[i]
            n  += mc_data[j].n[i] 
            f  += mc_data[j].frac[i]
            n1 += mc_data[j].neg1[i]
            s1 += mc_data[j].sig1[i] 
            n2 += mc_data[j].neg2[i]
            s2 += mc_data[j].sig2[i]
          end
          s1 /= Math.sqrt m 
          s2 /= Math.sqrt m
          average.add [r, n, f, n1, s1, n2, s2].map!{|x| x * 1.0/m}
        end
        average
      end
    
    end

    def select frac
      sorted = [ self.phase, self.quad, @ax, @ap ].transpose.sort_by do |e|
        e[2]**2 +  e[3]**2
      end
      max = (sorted.length * frac).to_i      
      Conditional::make_cond_margi *sorted[0..max].transpose
    end

    def to_condneg( args = {} )
      limit   = args[:limit]    || 0.005
      verbose = args[:verbose]  || true
      cneg  = Quix::Cond_Negativity.new args[:style]    || :radius 
      cneg.plot.path = self.plot.path + "_neg"
      margi = self
      total = 1.0 * self[:samples]
      n     = total
      frac  = 1.0
      cneg.add [99.0, n, 1.0] + margi.negvar(:kirt, 7.0) + margi.negvar(:sirt)
      while frac > limit do
        margi = margi.select 0.95
        rad   = Math::sqrt( 2 * ( margi.ax[-1]**2 + margi.ap[-1]**2 ) )
        #rad   = ( margi.ax[-1]**2 + margi.ap[-1]**2 )
        n     = margi[:samples]
        frac  = (1.0*n)/total
        data  = [rad, n, frac] + margi.negvar(:kirt, 7.0) + margi.negvar(:sirt)
        puts "radius/size/frac %f %i %f | kernel %f +- %f | series %f +- %f" % data if verbose
        cneg.add data           
      end     
      cneg
    end

  end

  class Cond_Negativity

    class Probability

      RedPlot::insert(in: self, as: :plot, command: 'plot') do
        set.xlabel '"radius"'
        set.ylabel '"fraction"'
        set.xrange '[0:4]'
        set.yrange '[0:1]'
      end

      def initialize radius, fraction
        plot.todraw.add('w l notitle') { [radius, fraction] }
      end

    end

    RedPlot::insert(in: self, as: :plot, command: 'plot') do
      set.ylabel '"negativity"'
      set.style.fill.transparent.solid(0.2).noborder
    end

    attr_reader :rad, :n, :frac, :neg1, :sig1, :neg2, :sig2 

    def initialize style = :radius
      @rad, @n, @frac = [], [], []
      @neg1, @sig1    = [], []
      @neg2, @sig2    = [], []
      case style
        when :radius
          plot.set.xlabel '"radius"'
          plot.set.xrange '[0:5]'
          plot.todraw.add('u 1:($2-$3):($2+$3) w filledcu lc 1 notitle') { [@rad, @neg1, @sig1] }
          plot.todraw.add('u 1:($2-$3):($2+$3) w filledcu lc 3 notitle') { [@rad, @neg2, @sig2] }
          plot.todraw.add('u 1:2 w p lc 1 title "kernel irt"') { [@rad, @neg1] }
          plot.todraw.add('u 1:2 w p lc 3 title "series irt"') { [@rad, @neg2] }
        when :fraction
          plot.set.xlabel '"fraction of total events"'
          plot.set.xrange '[0.005:1]'
          plot.set.xtics '(.005,.01,.03,.05,.1,.3,.5,1.0)'
          plot.set.logscale.x
          plot.todraw.add('u 1:($2-$3):($2+$3) w filledcu lc 1 notitle') { [@frac, @neg1, @sig1] }
          plot.todraw.add('u 1:($2-$3):($2+$3) w filledcu lc 3 notitle') { [@frac, @neg2, @sig2] }
          plot.todraw.add('u 1:2 w p lc 1 title "kernel irt"') { [@frac, @neg1] }
          plot.todraw.add('u 1:2 w p lc 3 title "series irt"') { [@frac, @neg2] }
      end
    end

    def add data
      r, n, f, n1, s1, n2, s2 = *data
      @rad  << r
      @n    << n
      @frac << f
      @neg1 << n1
      @sig1 << s1
      @neg2 << n2
      @sig2 << s2
    end

    def prob
      Probability.new @rad, @frac
    end

  end

  class Marginal < FFI::ManagedStruct

    def to_conditional ax, ap
      self.extend Conditional
      self.plot.todraw.reinit.add(' u 1:2 with dots notitle'){ [self.phase, self.quad, self.ax, self.ap] }
      @ax, @ap = ax, ap
      self
    end

    #
    # !! FALSE !!
    #
    def conditional epr = 1.0, loss = 0.0, g = 1.0, style = :radius
puts "\nepr #{epr}, loss #{loss}, g #{g}\n"
      n       = self[:samples]
      genvac  = Quix::Generator.vacuum
      l, a, s = Math::sqrt(2.0), Math::exp(+2.0*epr), Math::exp(-2.0*epr)

      pin      = Quix::Generator.one.marginal(n).quad

      asqz1    = genvac.marginal(n).quad.map{ |x| l*a*x }
      asqz2    = genvac.marginal(n).quad.map{ |x| l*a*x }
      sqz1     = genvac.marginal(n).quad.map{ |x| l*s*x }
      sqz2     = genvac.marginal(n).quad.map{ |x| l*s*x }

      #asqz1    = Marginal.gaussian(n, Math::exp(+epr))
      #sqz2     = Marginal.gaussian(n, Math::exp(-epr))

      xin     = self.quad
      phz     = self.phase

      xu      = Array.new(n){ |i| (xin[i] - 0.5*( asqz1[i] +  sqz2[i] ) )/l }
      pv      = Array.new(n){ |i| (pin[i] + 0.5*(  sqz1[i] + asqz2[i] ) )/l }


      #xout    = Array.new(n){ |i| 
      #  cos, sin = Math::cos(phz[i]), Math::sin(phz[i])
      #  g*xin[i] -0.5*(g+1.0)*(cos*sqz2[i]-sin*sqz1[i]) -0.5*(g-1.0)*(cos*asqz1[i]-sin*asqz2[i])
      #}

      xout    = Array.new(n){ |i| g*xin[i] -0.5*(g+1.0)*sqz2[i] -0.5*(g-1.0)*asqz1[i] }

      #if loss == 0.0
      #  Conditional.make( self.phase, xout, xu, xu )
      #else
      #  Marginal.new(self.phase,xout).loss(loss).to_conditional(xu,xu)
      #end
      Conditional.make( phz, xout, xu, pv )
    end

    def mc_conditional m = 100, r = 1.0, g = 1.0, style = :radius
      mc_data = Array.new(m) do |i|
        puts "\ngenerating conditional neg data ##{i+1}\n\n"
        self.conditional(r,g).to_condneg(verbose: false) 
      end
      average = Cond_Negativity.new style
      mc_data[0].frac.each_index do |i|
        r, n, f, n1, s1, n2, s2 = 0.0, 0, 0.0, 0.0, 0.0, 0.0, 0.0
        m.times do |j|
          r  += mc_data[j].rad[i]
          n  += mc_data[j].n[i] 
          f  += mc_data[j].frac[i]
          n1 += mc_data[j].neg1[i]
          s1 += mc_data[j].sig1[i] 
          n2 += mc_data[j].neg2[i]
          s2 += mc_data[j].sig2[i]
        end
        s1 /= Math.sqrt m 
        s2 /= Math.sqrt m
        average.add [r, n, f, n1, s1, n2, s2].map!{|x| x * 1.0/m}
      end
      average
    end

  end

end


