(begin
  (display "hello world")
  (newline))

;; print utilities

(import (srfi 26)) ; currying notation

(define print
  (lambda args (for-each display args)))

(define prints
  (lambda args
    (for-each (cut print <> #\space) args)))
; OR
(define prints2
  (lambda args
    (for-each display (concatenate (map (cut list <> #\space) args)))))

(define println
  (lambda args (apply prints args) (newline)))

(println 1 2 3)
(printsln 1 2 3)
(prints2 1 2 3)(newline)

;; util
(define (inc n) (+ n 1))
(define (dec n) (- n 1))

;; functional

(define (compose f g)
  (lambda (x) (g (f x))))
(println "compose" ((compose inc inc) 0))

(define-syntax partial
  (syntax-rules ()
    ((partial fn args ...) (lambda (x) (fn args ... x)))))
(println "partial" ((partial + 1 (+ 1 1) 3) 4))

;; TODO ... macro for function composition, , macro for pipeline application

;; collections


;; streams
(define (constant-streams c)
  (letrec ((pair (cons c (lambda () pair))))
    pair))

(let ((s (constant-streams 2)))
  (printsln (car s) (car ((cdr s))) (car ((cdr ((cdr s)))))))

;; IO list

(import (srfi 133)) ; vector ops

(define iolist-print
  (lambda (x)
    (cond
      ((string? x)    (display x))
      ((char? x)      (display x))
      ((number? x)    (display x))
      ((pair? x)      (iolist-print (car x)) (iolist-print (cdr x)))
      ((vector? x)    (vector-for-each iolist-print x))
      ((procedure? x) (iolist-print (x)))
      ((null? x)      #f) ;nothing
      ('default       (display x)))))

