(defn
  fib
  [n]
  (loop [a 1, b 1, x '(1 1)]
    ( if(= n (count x))
	  x
     (recur (+ a b) a (cons (+ a b)  x ))
	)
  )
)

(def nbrs [4 7 14])

(doseq 
  [n nbrs] 
  (println (fib n) ) 
)

