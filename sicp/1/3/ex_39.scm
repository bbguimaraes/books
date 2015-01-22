(define (cont-frac n d k)
  (define (iter i result)
    (if (= i 0)
      result
      (iter (- i 1) (/ (n i) (+ (d i) result)))))
  (iter k 0))

(define (tan-cf x k)
  (cont-frac (lambda (i) (if (= i 1) x (- (* x x))))
             (lambda (i) (+ 1 (* (- i 1) 2.0)))
             k))

(display (tan-cf 3.14159 100))
(newline)
(display (tan-cf (/ 3.14159 3) 100))
(newline)
(display (tan-cf (/ 3.14159 6) 100))
(newline)
