(define (sqrt-iter guess x)
  (define new-guess (improve guess x))
  (if (good-enough? guess new-guess)
    new-guess
    (sqrt-iter new-guess x)))

(define (improve guess x)
  (average guess (/ x guess)))

(define (average x y)
  (/ (+ x y) 2))

(define (good-enough? old-guess guess)
  (< (abs (- guess old-guess)) 0.001))

(define (sqrt x)
  (sqrt-iter 1.0 x))

(display (sqrt 9))
(newline)
(display (sqrt (+ 100 37)))
(newline)
(display (sqrt (+ (sqrt 2) (sqrt 3))))
(newline)
(display (square (sqrt 1000)))
(newline)
