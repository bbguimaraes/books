(define (cons x y) (* (expt 2 x) (expt 3 y)))
(define (car z) (count-perfect-divisions z 2))
(define (cdr z) (count-perfect-divisions z 3))

(define (count-perfect-divisions n d)
  (define (iter x i)
    (if (not (= (remainder x d) 0))
      i
      (iter (/ x d) (+ i 1))))
  (iter n 0))

(define c (cons 14 15))
(display (car c))
(newline)
(display (cdr c))
(newline)
