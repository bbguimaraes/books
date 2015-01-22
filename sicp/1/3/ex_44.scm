(define (smooth f)
  (lambda (x) (/ (+ (f (- x dx))
                    (f x)
                    (f (+ x dx)))
                 3)))

(define dx 1)

(define (compose f g)
  (lambda (x) (f (g x))))

(define (repeated f n)
  (if (= n 0)
    (lambda (x) x)
    (compose f (repeated f (- n 1)))))

(define (n-fold-smooth f n)
  ((repeated smooth n) f))

(display (map (n-fold-smooth square 1) '(1.0 2.0 3.0 4.0 5.0)))
(newline)
(display (map (n-fold-smooth square 2) '(1.0 2.0 3.0 4.0 5.0)))
(newline)
(display (map (n-fold-smooth square 3) '(1.0 2.0 3.0 4.0 5.0)))
(newline)
(display (map (n-fold-smooth square 4) '(1.0 2.0 3.0 4.0 5.0)))
(newline)
(display (map (n-fold-smooth square 5) '(1.0 2.0 3.0 4.0 5.0)))
(newline)
