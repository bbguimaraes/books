(define (cbrt-iter guess x)
  (define new-guess (improve guess x))
  (if (good-enough? guess new-guess)
    new-guess
    (cbrt-iter new-guess x)))

(define (improve guess x)
  (/ (+ (/ x (* guess guess))
        (* 2 guess))
     3))

(define (good-enough? old-guess guess)
  (< (abs (- guess old-guess)) 0.001))

(define (cbrt x)
  (cbrt-iter 1.0 x))

(display (cbrt 9))
(newline)
(display (cbrt (+ 100 37)))
(newline)
(display (cbrt (+ (cbrt 2) (cbrt 3))))
(newline)
(display (square (cbrt 1000)))
(newline)
