(define one-arbitrary-number 9999999)

(define (make-rand)
  (let ((s (make-random-state)))
    (lambda (m)
      (cond ((eq? m 'generate) (random one-arbitrary-number s))
            ((eq? m 'reset) (lambda (new-s) (set! s new-s)))
            (else (error "Unknown request -- RAND" m))))))

(define rand (make-rand))

(define s (make-random-state))
((rand 'reset) (make-random-state s))
(display (rand 'generate))
(newline)
(display (rand 'generate))
(newline)
(display (rand 'generate))
(newline)
((rand 'reset) (make-random-state s))
(display (rand 'generate))
(newline)
(display (rand 'generate))
(newline)
(display (rand 'generate))
(newline)
