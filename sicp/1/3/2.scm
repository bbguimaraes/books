(define (sum term a next b)
  (define (iter a result)
    (if (> a b)
        result
        (iter (next a) (+ result (term a)))))
  (iter a 0))

(define (pi-sum a b)
  (sum (lambda (x) (/ 1.0 (* x (+ x 2))))
       a
       (lambda (x) (+ x 4))
       b))

(display (* 8 (pi-sum 1 1000)))
(newline)

(define (integral f a b dx)
  (* (sum f
          (+ a (/ dx 2.0))
          (lambda (x) (+ x dx))
          b)
      dx))

(display (integral (lambda (x) (* x x x)) 0 1 0.001))
(newline)

(display ((lambda (x y z) (+ x y (square z))) 1 2 3))
(newline)

(define (f x y)
  (define (f-helper a b)
    (+ (* x (square a))
       (* y b)
       (* a b)))
  (f-helper (+ 1 (* x y))
            (- 1 y)))

(newline)
(display (f 2 3))
(newline)

(define (f x y)
  ((lambda (a b)
     (+ (* x (square a))
        (* y b)
        (* a b)))
   (+ 1 (* x y))
   (- 1 y)))

(display (f 2 3))
(newline)

(define (f x y)
  (let ((a (+ 1 (* x y)))
        (b (- 1 y)))
    (+ (* x (square a))
       (* y b)
       (* a b))))
  
(display (f 2 3))
(newline)

(define x 5)
(display (+ (let ((x 3))
              (+ x (* x 10)))
            x))
(newline)

(define x 2)
(display (let ((x 3)
              (y (+ x 2)))
           (* x y)))
(newline)

(define (f x y)
  (define a (+ 1 (* x y)))
  (define b (- 1 y))
  (+ (* x (square a))
     (* y b)
     (* a b)))

(display (f 2 3))
(newline)
