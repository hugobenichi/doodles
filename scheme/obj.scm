(import (scheme case-lambda)
        (srfi 1)
        (srfi 26)
        (srfi 69))

(define (println x) (display x) (newline))
(define (debug x) (println x) x)

(define make-obj
  (lambda (type ls)
    (let ((table (make-hash-table eq?)))
      (for-each (lambda (ls) hash-table-set! table (car ls) (cadr ls)) ls)
      (debug table)
      (case-lambda
        (() type)
        ((key) (hash-table-ref/default table key #f))
        ((key val) (hash-table-set! table key val))))))

(define ls '((x 1) (y 2) (z 3)))
(define obj (make-obj 'mytype ls))

(println (obj 'x))
(println (obj 'y))
(println (obj 'z))
(println (obj))


(define genmethod-hashtable (make-hash-table eq?))
(define defgenmethod
  (lambda (symbol)
    (hash-table-set! genmethod-hashtable symbol (make-hash-table eq?))))
(define addgenmethod
  (lambda (symbol type fn)
    (hash-table-set! (hash-table-ref genmethod-hashtable symbol) type fn)))
(define getgenmethod
  (lambda (symbol obj . args)
    (let ((type   (obj))
          (vtable (hash-table-ref genmethod-hashtable symbol)))
      (apply ((hash-table-ref vtable type) obj) args))))

(define myobj
  (case-lambda
    (() 'myobj)
    ((x) 'foo)))

(defgenmethod 'show)
(addgenmethod 'show
              (myobj)
              (lambda (obj)
                (lambda ls (display (obj 4)) (display ls) (newline))))
(getgenmethod 'show myobj 1 2 3)


step 1
define a macro to create a symbol and a dispatch table for that table

(define (make-dispatch-table symbol key-fn)
  (let* [(table (make-hash-table eq?))]
         (dispatch-get (lambda (key-fn) (hash-table-ref table symbol)
