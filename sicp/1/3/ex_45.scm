(define (average x y) (/ (+ x y) 2))

(define (average-damp f)
  (lambda (x) (average x (f x))))

(define tolerance 0.00001)
(define (fixed-point f first-guess)
  (define (close-enough? v1 v2)
    (< (abs (- v1 v2)) tolerance))
  (define (try guess)
    (let ((next (f guess)))
      (if (close-enough? guess next)
        next
        (try next))))
  (try first-guess))

;(define (sqrt x)
;  (fixed-point (lambda (y) (/ x y))))

(define (sqrt x)
  (fixed-point (average-damp (lambda (y) (/ x y)))
               1.0))

(display (sqrt 9))
(newline)

(define (cube-root x)
  (fixed-point (average-damp (lambda (y) (/ x (* y y))))
               1.0))

(display (cube-root 27))
(newline)

;(define (fourth-root x)
;  (fixed-point (average-damp (lambda (y) (/ x (* y y y))))
;               1.0))

(define (fourth-root x)
  (fixed-point (average-damp (average-damp (lambda (y) (/ x (* y y y)))))
               1.0))

(display (fourth-root 81))
(newline)

(define (compose f g)
  (lambda (x) (f (g x))))

(define (repeated f n)
  (if (= n 0)
    (lambda (x) x)
    (compose f (repeated f (- n 1)))))

(define (log2 x) (/ (log x) (log 2)))

(define (nth-root n)
  (lambda (x)
    (fixed-point ((repeated average-damp (floor (log2 n)))
                  (lambda (y) (/ x (expt y (- n 1)))))
                 1.0)))

(newline)
(map (lambda (x) (map display
                      (list x " " ((nth-root x) (expt 3 x)) "\n")))
     '(1 2 3 4 5 6 7 8 9 10))
