(define (f n)
  (if (< n 3)
    n
    (+ (f (- n 1))
       (* 2 (f (- n 2)))
       (* 3 (f (- n 3))))))
(display (f 5))
(newline)

(define (f n)
  (if (< n 3)
    n
    (f-iter 0 1 2 (- n 3))))
(define (f-iter f-3 f-2 f-1 count)
  (define new-f (+ f-1 (* 2 f-2) (* 3 f-3)))
  (if (= count 0)
    new-f
    (f-iter f-2 f-1 new-f (- count 1))))
(display (f 5))
(newline)
