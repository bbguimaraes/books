(define (double f) (lambda (x) (f (f x))))

(define (inc x) (+ x 1))

(display ((double inc) 5))
(newline)

(display (((double (double double)) inc) 5))
(newline)
