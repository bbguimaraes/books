(define count -1)
(define (f x)
  (set! count (+ count 1))
  (if (= (remainder count 2) 0) 0 x))

(display (+ (f 0) (f 1)))
(newline)
(display (+ (f 1) (f 0)))
(newline)
