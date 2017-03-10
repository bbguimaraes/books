(define (estimate-integral P x1 x2 y1 y2 trials)
  (define (experiment)
    (define (random-in-range low high)
      (let ((range (- high low)))
        (+ low (random range))))
    (P (random-in-range x1 x2) (random-in-range y1 y2)))
  (* (* (- x2 x1) (- y2 y1))
     (monte-carlo trials experiment)))

(define (monte-carlo trials experiment)
  (define (iter trials-remaining trials-passed)
    (cond ((= trials-remaining 0)
           (/ trials-passed trials))
          ((experiment)
           (iter (- trials-remaining 1) (+ trials-passed 1)))
          (else
            (iter (- trials-remaining 1) trials-passed))))
  (iter trials 0))

(define one-arbitrary-number 33333)

(define (inside-circle-predicate xc yc r)
  (let ((r2 (square r)))
    (lambda (x y) (<= (+ (square (- x xc)) (square (- y yc))) r2))))

(display
  (estimate-integral
    (inside-circle-predicate 5 7 3)
    2.0 8.0 4.0 10.0
    one-arbitrary-number))
(newline)

(display
  (estimate-integral
    (inside-circle-predicate 0 0 1)
    -1.0 1.0 -1.0 1.0
    one-arbitrary-number))
(newline)
