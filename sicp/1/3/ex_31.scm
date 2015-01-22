(define (product term a next b)
  (define (iter a result)
    (if (> a b)
      result
      (iter (next a) (* result (term a)))))
  (iter a 1))

(define (identity x) x)
(define (inc x) (+ x 1))
(define (factorial n)
  (product identity 1 inc n))

(display (factorial 6))
(newline)

(define (pi-product n)
  (define (term x)
    (define (result n d)
      (/ n d))
    (if (odd? x) (result (+ x 1) (+ x 2))
                 (result (+ x 2) (+ x 1))))
  (* (product term 1.0 inc n)
     4.0))

(display (pi-product 100.0))
(newline)
(display (pi-product 1000.0))
(newline)

(define (product term a next b)
  (define (iter a)
    (if (> a b)
      1
      (* (term a) (iter (next a)))))
  (iter a))

(display (factorial 6))
(newline)
(display (pi-product 100.0))
(newline)
(display (pi-product 1000.0))
(newline)
