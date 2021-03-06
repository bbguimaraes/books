(define (book-* a b)
  (if (= b 0)
    0
    (+ a (* a (- b 1)))))

(define (fast-mul a b)
  (define (double x)
    (+ x x))
  (define (havle x)
    (/ x 2))
  (define (*-iter b n a)
    (cond ((= n 0) a)
          ((odd? n) (*-iter b (- n 1) (+ b a)))
          (else (*-iter (double b) (/ n 2) a))))
  (*-iter b a 0))

(map
  (lambda (x)
    (let
      ((book2 (book-* 2 x)) (impl2 (fast-mul 2 x))
       (book3 (book-* 3 x)) (impl3 (fast-mul 3 x))
       (book4 (book-* 4 x)) (impl4 (fast-mul 4 x)))
      (if (< book2 10) (display " "))
      (display book2)
      (display " = ")
      (if (< impl2 10) (display " "))
      (display impl2)
      (display "  ")
      (if (< book3 10) (display " "))
      (display book3)
      (display " = ")
      (if (< impl3 10) (display " "))
      (display impl3)
      (display "  ")
      (if (< book4 10) (display " "))
      (display book4)
      (display " = ")
      (if (< impl4 10) (display " "))
      (display impl4)
      (newline)))
  '(1 2 3 4 5 6 7 8 9 10))
