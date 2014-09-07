

    (defn 
      l_rev_tail 
      [x y] 
      (if (empty? x) 
        y
        (l_rev_tail (rest x) (cons (first x) y) )
      )
    )

(defn 
  l_rev 
  [z] 
  (l_rev_tail z '() ) 
)


(defn 
  l_rev2
  [z]  
  ( loop [ x z, y '() ]
    (if (empty? x) 
      y
      (recur (rest x) (cons (first x) y) )
    )
  )
)
  
  
(def a '(1 2 3 4 5) )

(println a "into" (l_rev2 a) )

