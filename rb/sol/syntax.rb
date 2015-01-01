require './sol'


#I) initialization of a Vector instance

# 1 from an Array instance

  def ary(n); Array.new(n){rand}; end

  a = ary(10).vec shape: [1], type: :float  #, and any other flag as hash
  b = ary(16).vec                           #default shape is 1, default type is double


# 1.1 setting shape
#   shapes are regular array of int where each int
#   the length of the array is the number of dimensions
#     except for dim 1 where it is explicitely stated that there is only one dim
#     the integers inside the array say how many elements are along that dimension
#     the order is rows first, column second (C order)

# 1.1.1 with dim
  a = ary(16).vec as: [1,16]    #1dim -> 1x16
  b = ary(16).vec dim: 2        #2dim -> 4x4
  c = ary(12).vec dim: 2        #2dim -> 3x4
  d = ary(13).vec dim: 2        #2dim -> 4x4   pad with zeros
  d = ary(13).vec dim: 3        #3dim -> 2x3x3 pad with zeros

# 1.1.2 with an hypercube shape
  m = ary(16).vec shape: :sqr   #2dim -> 4x4
  n = ary(20).vec shape: :sqr   #2dim -> 5x5
  o = ary(20).vec shape: :cub   #3dim -> 3x3x3
 
# 1.1.3 with arrays
  p = ary(16).vec shape: [4,4]  #4x4
  q = ary(16).vec shape: [2,8]  #2x8

# 1.1.4 with incomplete arrays
  p = ary(16).vec shape: [4,4]     #4x4
  q = ary(16).vec shape: [nil,2,4] #cuts the ary in 4x4 blocks with 3 dim unknown 
                                   #nil is a wild card

# 2 by copy (using memcpy)

    a = ary(16).vec as: 1        # 1x16
    b = ary(16).vec as: 2        # 4x4
    c = a.cpy                    # 1x16
    d = a.cpy as: 2              # 4x4
    d = a.cpy as: [4,5]          # 4x5 and 0 padding (1st cr8, zn memcpy, zn pad)

# if b has mem assigned
    a.cpy to: b
    b.cpy from: a
    b.cpy from: a, as: [4,1]

  # 2.1) quick copy

    b = !a
    c = a.!

end

# 3 with constructor

    #need size and type
    a = Vec.new as: [1,6], typ: :double     #does not know the size if no data, only init memory
    b = Vec.new( as: [16,16], typ: :double) do |i,j|  
      #return value of b[i][j] 
    end

# 3.1 alternate constructor

    a = vec.double [4,4]    #object or method ??
    b = vec.double [1,16]
    c = vec.int [1,16]
    c = vec.as  [3,4]       #default to double


# 4 shape and stride info

    a.shape
    a.stride

# 5 feed values

    a = vec.double([4,4]).fill 1.0
    b = vec.double([2,8]).zero  #special case


# 6 cr8 ranges and grids



#=begin 

# II) operations

  #0) reshaping

    a = ary(16).vec as: 1  # 1x
    b = ary(16).vec as: 2  # 4x4
    c = ary(12).vec as: 2  # 3x4

    #no bit moving, no cpy, just reverse shape and stride

    a.shape       #=> [1,16]  
    a.stride      #=> [0,1] 
    a.transpose   #in the vec case nothing happens on bits
    a.shape       #=> [16,1]
    a.stride      #=> [1,0]

    b.shape       #=> [4,4]
    b.stride      #=> [4,1] 
    b.transpose  
    b.shape       #=> [4,4]
    b.stride      #=> [1,4] 

    c.shape       #=> [3,4]
    c.stride      #=> [4,1] 
    c.transpose  
    c.shape       #=> [4,3]
    c.stride      #=> [1,4] 

    #bit moving, with cpy (a,b,c reinit)

    a.transpose!  #in the vec case nothing happens on bits too
    a.shape       #=> [16,1]
    a.stride      #=> [1,0]

    b.transpose!  
    b.shape       #=> [4,4]
    b.stride      #=> [4,1]  #bits have been moved

    c.transpose!  
    c.shape       #=> [4,3]  #bits have been moved around
    c.stride      #=> [3,1] 

    #there is no reshape with bit moving without cpy, instead use:

    a.!.transpose     #cpy without bit moving
    a.!.transpose!    #useless

    #to do reshape with bit moving without cpy, use asignement:

    a = a.transpose!
    a.copy from: a.transpose!

    #alias for transpose

    ~a    #without copy  (higher precedence, return self)
    ~~a   #does nothing

    a.t!     #with copy

    #be carefull !~a does not work (but no meaning to copy the inplace transpose)

  #1) 1->0

    a = ary(16).vec as: [16]
    b = ary(16).vec as: [4,4]

    a.sum  #=> fixnum / 
    a.min
    a.max
    a.norm

    b.trace  # <=> b.diag.sum
    b.norm


  #1.1) 2->0

    a.o b   #dot product
    a.dot b
    a^b

  #2) 1->1

    a = ary(16).vec

    #no parameter
    #special functions can be choosed at compile time (x86 or sse or fast)
    a.log; a.log2; a.ln; a.log10; a.exp
    a.cos; a.sin; a.tan
    a.cosh; a.sinh; a.tanh
    a.inv; a.sqr; a.cub; a.sqrt
    a.isqrt #inverse square root
    a.abs  #this use the good fabs or abs 
    
    #arity 1 operations with copy

    a.log! => a.!.log
 
  #2.1) 1,0 -> 1 with scalar

    a = ary(16).vec as: [16]   
    #all operations in inplace, point wise

    3 * a
    a * 3
    a + 3
    a - 3
    a / 3
    3 / a   #this first inv a and then mult by 3
    a ** 3  
   

  #3) 2 -> 1
  
    a = ary(16).vec as: [1,16]
    b = ary(16).vec as: [1,16]
    c = ary(16).vec as: [4,4]

    #all operations are point wise, inplace, ignore shape (! see slicing to do broadcasting)
  
      a + b
      a * b
      a - b
      a / b
      a ** b

    #with copy operations to avoid overwrite

      !a+b
      !a*b
      ...  
      !(a+b)  # this first puts a+b in a and then copy the results

    #outer product, by default alloc new memory,
    # use layout over order to determine matrix shape

      a | b   #does same as !a*b because a.shape = b.shape = [1,16] (the same as !a*b)
     ~a | b   #makes a matrix [16,16], rows are 'a' ordered and cols are 'b' ordered
      a | ~b  #makes a matrix [16,16], rows are 'b' ordered and cols are 'a' ordered

    #to make a grid

      a = range #interface to choose
      grid = ~!a|a   

    #3) matrix product
    # if special case shape the ** operator is changed to outer product or matrix product

      x = [1,2,3,4].vec
      y = [1,1,1,1].vec
      z = [1,-1,1,-1].diagonal

      #the rule on shape is [a,b] [b,c] -> [a,c] with loop on c
      
      x ** ~!y = 10  # [1,4][4,1] -> [1,1] (scalar) and loop on 4, this is the dot product
      x ** ~y = [[1,2,3,4],[1,2,3,4],[1,2,3,4],[1,2,3,4]] # [4,1][1,4] -> [4,4]


      z ** ~x = [[1],[-2],[3],[-4]] # [4,4][4,1]:[4,1] matrice vector product
      x ** z  = [1,-2,3,-4]  # [1,4][4,4]:[1,4] reversed matrice vector product
      
      #broadcasting in action

      z ** x = ?  # [4,4][1,4]:[4,4]  # 1) the bigger object wins
                                      # 2) the smaller dimension are padded pereiodically
        # in that case x becomes [[1,2,3,4],[1,2,3,4],[1,2,3,4],[1,2,3,4]]
        # in other words x ** ~padder where padder is [1,1,...] with length of winning dim (4)
        # I have to chose between to behavior:
        #
        #         (A B C)       
        # (a b c)               (Aa Bb Cc) 
        # (d e f)           =>  (Ad Be Cf)          #looped multiply apply
        # (g h i)               (Ag Bh Ci)  
        #
        # or
        #                              (A B C) 
        #                              (A B C)       #normal matrix product
        #                              (A B C) 
        #                       (a b c) 
        #                   =>  (d e f)
        #                       (g h i) 
        #
        # I opted for the second one because I don t have easy access to that operation
        #

# III) local reshape, slicing

  #1) mat to vec
    a = ary(16).vec as: 2

    a.log as: [8]   #log on first column
    a.as([8]).log   #create a temp vector to the same mem address
                    #only need to create the shape info
                    #less overhead on log (no args)


  #diagonal

#broadcast problematic

#from NArray, Operation between [nx,1] and [1,ny] arrays makes [nx,ny].
filt = 1 / (x**2 + y**2 + 1)



#experimental syntax

x = [1.0,2.0,3.0,4.0].vec( as: [1,4])

x > 4             #spash operator use to extend an array, makes a copy by default
                  #this copy the vector 4 times with memcpy and arrange the shape
                  # [[1,2,3,4]] - > 
                  # [[1,2,3,4],[1,2,3,4],[1,2,3,4],[1,2,3,4]]
~x > 4            # [[1,2,3,4]] -> [[1],[2],[3],[4]]  ->
                  # [[1,1,1,1],[2,2,2,2],[3,3,3,3],[4,4,4,4]]

#from NArray, Operation between [nx,1] and [1,ny] arrays makes [nx,ny].
filt = 1 / (x**2 + y**2 + 1) # wants to convert to SoL syntax -> 

x = range(l)

1 / (  ( !x**2 > l) + ~!x**2.> l + 1 )
1 / ( !x**(2)>(l), ~!x**2.> l) + 1 )

1 / ( (x.sqr! > l) + (~!x.sqr > l) + 1 )
1 / ( !x.sqr.>(l) + ~!x.sqr.>(l) + 1 )

#turn on broadcasting for automatic splashing
#or use subclass

x = range(4, args).!.broadcast

1 / ( !x.sqr + ~!x.sqr + 1 )
1 / ( !x**2 + ~!x**2 + 1 )     #this will create ane xtnded matrix to match the broadcast rule

y = [1,1,1,1].vec

1 / ( (x | y) + (y | x) + 1  ) #in that case the order | is not commutative anymore
1 / ( (~!y|x) + (y|~!x) + 1  ) 


x % y


#broadcast with precedence and polymorphism


#periodic slicing with module operator%

x = [1,2,3,4].vec
y = [1,1,1,1].vec
z = ~! x | y
z = ~! [1,2,3,4].vec | [1,1,1,1].vec

x.as [4,1]

=end
