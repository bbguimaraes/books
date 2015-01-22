(define (reverse l)
  (define (iter l result)
    (if (null? l)
      result
      (iter (cdr l) (cons (car l) result))))
  (iter l '()))
(display (reverse (list 1 4 9 16 25)))
(newline)
(display (reverse '()))
(newline)
