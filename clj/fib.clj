(defn fib [n]
  (loop [a 1, b 1, vx [a b]]
    (if (= n (count vx))
	    vx
      (let [ab (+ a b)]
        (recur ab a (conj vx ab))))))

(def nbrs [4 7 14])

(doseq [r (map fib nbrs)] println)
(doseq [n nbrs]
  (println (fib n)))

(run! println (map fib [ 5 10 15]))
