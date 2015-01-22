(define (sum term a next b)
  (if (> a b)
    0
    (+ (term a)
       (sum term (next a) next b))))

(define (cube x) (* x x x))
(define (inc x) (+ x 1))

(define (simpsons-rule f a b n)
  (define (with-h h)
    (define (y k) (f (+ a (* k h))))
    (define (term k)
      (* (cond ((or (= k 0) (= k n)) 1)
               ((odd? k) 4)
               (else 2))
         (y k)))
    (* (/ h 3.0) (sum term 0 inc n)))
  (with-h (/ (- b a) n)))

(display (simpsons-rule cube 0 1 100.0))
(newline)
(display (simpsons-rule cube 0 1 1000.0))
(newline)
