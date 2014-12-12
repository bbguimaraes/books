(define (inc x) (+ x 1))
(define (dec x) (- x 1))
(define (add a b)
  (if (= a 0)
      b
      (inc (add (dec a) b))))
(display (add 2 7))
(define (add a b)
  (if (= a 0)
      b
      (add (dec a) (inc b))))
(newline)
(display (add 2 7))
