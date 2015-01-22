(define (iterative-improve good-enough? next)
  (define (iter guess)
    (let ((next-guess (next guess)))
      (if (good-enough? guess next-guess)
        guess
        (iter (next guess)))))
  iter)

(define (fixed-point f first-guess)
  ((iterative-improve (lambda (v1 v2) (< (abs (- v1 v2)) 0.00001))
                      f)
   first-guess))

(define (log2 x) (/ (log x) (log 2)))

(define (average x y) (/ (+ x y) 2))

(define (average-damp f)
  (lambda (x) (average x (f x))))

(define (compose f g)
  (lambda (x) (f (g x))))

(define (repeated f n)
  (if (= n 0)
    (lambda (x) x)
    (compose f (repeated f (- n 1)))))

(define (nth-root n)
  (lambda (x)
    (fixed-point ((repeated average-damp (floor (log2 n)))
                  (lambda (y) (/ x (expt y (- n 1)))))
                 1.0)))

(newline)
(map (lambda (x) (map display
                      (list x " " ((nth-root x) (expt 3 x)) "\n")))
     '(1 2 3 4 5 6 7 8 9 10))
