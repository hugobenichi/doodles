(define (cons! obj ls)
  (define new_pair (cons (car ls) (cdr ls))
  (set-cdr! ls new_pair)
  (set-car! ls obj))
  ls)

(define (ensure_has table key zero)
  (define pair (assoc key table))
  (if (not pair)
    (begin
      (set! pair '(key (val)))
      (cons! pair table))
    'void)
  pair)

(define (insert table key val comp)
  (if (null? table)
    '(key (val))
    (begin
      (define group (ensure_has table key '())) ; (ensure table has key with value)
      (set-cdr! group (cons val (cdr group)))
      (table))))
