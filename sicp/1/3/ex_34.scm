(define (f g)
  (g 2))

(display (f square))
(display " ")
(display (f (lambda (z) (* z (+ z 1)))))
(display " ")
(display (f f))
(newline)
