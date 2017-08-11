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
(import (srfi 1))
(define prints2
  (lambda args
    (for-each display (concatenate (map (cut list <> #\space) args)))))

(define println
  (lambda args (apply prints args) (newline)))

(println 1 2 3)
(println 1 2 3)
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
  (println (car s) (car ((cdr s))) (car ((cdr ((cdr s)))))))

;; Objects

(define (make-obj assocls)
  (case-lambda
  ((sym) (cadr (assoc sym assocls)))
  ((sym new_val) (cadr (assoc sym assocls)))

(let ((obj (make-obj '((x 1) (y 3) (z -2)))))
  (println (obj 'x) (obj 'y) (obj 'z)))

;; IO list

(import (srfi 133)) ; vector ops

(define iolist-print
  (lambda (x)
    (cond
      ((string? x)    (display x))
      ((char? x)      (display x))
      ((number? x)    (display x))
      ((null? x)      #f) ;nothing
      ((pair? x)      (iolist-print (car x)) (iolist-print (cdr x)))
      ((vector? x)    (vector-for-each iolist-print x))
      ((procedure? x) (iolist-print (x)))
      ('default       (display x))))) ; should be nothing ?

;; better JS-like object

(define (make-obj assocls)
  (case-lambda
    ((sym)          (cadr (assoc sym assocls)))
    ((sym new_val)  (let* ((pair (assoc sym assocls))
                           (old_val (car pair)))
                      (set-car! pair new_val)
                      old_val))))

(let ((obj (make-obj '((x 1) (y 2) (z 3)))))
  (println (obj 'x) (obj 'y) (obj 'z))
  (obj 'x 4)
  (println (obj 'x) (obj 'y) (obj 'z)))
