module Analysis

  module Projector

    class << self

      def get_mode_function(type, *args)
        case type
          when 1 then mode_function           *args
          when 2 then mode_function2          *args
          when 3 then mode_function_sym       *args
          when 4 then mode_function_original  *args
        end
      end

      def mode_function(siz, origin, gamma, kappa = 0.0, alpha = 0.0)#, alpha = 0.0)
        alpha = kappa / 3
        g2, k2, a2 = gamma**2, kappa**2, alpha**2
        a = a2/( (g2 - a2)*(alpha+kappa) )
        k = k2/( (g2 - k2)*(alpha+kappa) )
        gp = 0.5 * gamma / ( (gamma-alpha)*(gamma+kappa) )
        gm = 0.5 * gamma / ( (gamma-kappa)*(gamma+alpha) )
        Array.new(siz) do |i|
          t = i - origin
          if t > 0
            gp * Math.exp(-gamma*t) -  a * Math.exp(-alpha*t)
          else
            gm * Math.exp(+gamma*t) -  k * Math.exp(+kappa*t)
          end
        end
      end

      def mode_function2(siz, origin, gamma, kappa = 0.0, alpha = 0.0)
        b, c  = (kappa+gamma)/(gamma-kappa), 2*kappa/(gamma-kappa)
        Array.new(siz) do |i|
          t = i - origin
          if t > 0
            Math.exp(-gamma*t)
          else
            b * Math.exp(gamma*t) -  c * Math.exp(kappa*t)
          end
        end
      end

      def mode_function_sym(siz, origin, gamma, kappa = 0.0, alpha = 0.0)
        Array.new(siz) do |i|
          t = (i - origin).abs
          gamma * Math.exp(-gamma*t) -  kappa * Math.exp(-kappa*t)
        end
      end

      def mode_function_original(siz, origin, gamma, kappa = 0.0, alpha = 0.0)
        Array.new(siz) do |i| Math.exp -gamma*(i - origin).abs end
      end

      def mode_sideband(siz, omega)
        Array.new(siz) { |i| Math.sin(omega*i) }
      end

    end

  end

end
