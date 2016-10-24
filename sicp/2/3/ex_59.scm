(define (element-of-set? x set)
  (and (not (null? set))
       (or (equal? x (car set))
           (element-of-set? x (cdr set)))))

(define (union-set set1 set2)
  (append (filter (lambda (x) (not (element-of-set? x set2))) set1)
          set2))

(display (union-set (list 1 3 5 7) (list 2 3 4 6)))
(newline)
