(define (compose f g)
  (lambda (x) (f (g x))))

(define (inc x) (+ x 1))

(display ((compose square inc) 6))
(newline)

(display ((compose (compose square inc) inc) 6))
(newline)

(display ((compose (compose inc square) inc) 6))
(newline)

(display ((compose (compose inc inc) square) 6))
(newline)
