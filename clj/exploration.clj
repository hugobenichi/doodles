(defn vec_to_map
  "returns a function that turns a seq into a map using the provided keywords
   seq, in the order of the keys. i.e [:a :b] -> [1 2] => {:a 1, :b 2}"
  [key_seq]
  (fn [val_seq]
    (into {} (map vector key_seq val_seq))))
