(define (test)
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
  (newline))

(define (expt b n)
  (if (= n 0)
    1
    (* b (expt b (- n 1)))))

(test)

(define (expt b n)
  (define (expt-iter b counter product)
    (if (= counter 0)
      product
      (expt-iter b
                 (- counter 1)
                 (* b product))))
  (expt-iter b n 1))

(test)

(define (expt b n)
  (cond ((= n 0) 1)
        ((even? n) (square (expt b (/ n 2))))
        (else (* b (expt b (- n 1))))))

(test)
