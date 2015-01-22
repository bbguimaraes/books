(define (cont-frac n d k)
  (define (iter i result)
    (if (= i 0)
      result
      (iter (- i 1) (/ (n i) (+ (d i) result)))))
  (iter k 0))

(define (euler-e k)
  (cont-frac (lambda (i) 1.0)
             (lambda (i) (if (= (remainder (+ i 1) 3) 0)
                             (* 2 (/ (+ i 1) 3))
                             1))

             k))

(display (+ 2 (euler-e 1000)))
(newline)
