(define (expt b n)
  (define (expt-iter b n a)
    (cond ((= n 0) a)
          ((odd? n) (expt-iter b (- n 1) (* b a)))
          (else (expt-iter (* b b) (/ n 2) a))))
  (expt-iter b n 1))

(display (expt 2 1))
(display " ")
(display (expt 2 2))
(display " ")
(display (expt 2 3))
(display " ")
(display (expt 2 4))
(display " ")
(display (expt 2 5))
(display " ")
(display (expt 2 6))
(display " ")
(display (expt 2 7))
(display " ")
(display (expt 2 8))
(display " ")
(display (expt 2 9))
(display " ")
(display (expt 2 10))
(newline)
