(define (subsets s)
  (if (null? s)
    (list s)
    (let ((e (car s))
          (rest (subsets (cdr s))))
      (append rest
              (map (lambda (x) (cons e x)) rest)))))

(define s (list 1 2 3))
(display (subsets s))
(newline)
