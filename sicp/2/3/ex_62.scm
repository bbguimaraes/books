(define (union-set set1 set2)
  (cond ((null? set1) set2)
        ((null? set2) set1)
        (else
          (let ((car-set1 (car set1))
                (car-set2 (car set2)))
            (if (< car-set1 car-set2)
              (cons car-set1
                    (union-set (cdr set1) set2))
              (cons car-set2
                    (union-set set1 (cdr set2))))))))

(display (union-set (list 1 3 5 7) (list 2 3 4 6)))
(newline)
(display (union-set (list 2 3 4 6) (list 1 3 5 7)))
(newline)
(display (union-set (list 1 3    ) (list 2 3 4 6)))
(newline)
(display (union-set (list 2 3 4 6) (list 1 3    )))
(newline)
(display (union-set (list     5 7) (list 2 3 4 6)))
(newline)
(display (union-set (list 2 3 4 6) (list     5 7)))
(newline)
(display (union-set (list 1 3 5 7) (list 2 3    )))
(newline)
(display (union-set (list 2 3    ) (list 1 3 5 7)))
(newline)
(display (union-set (list 1 3 5 7) (list     4 6)))
(newline)
(display (union-set (list     4 6) (list 1 3 5 7)))
(newline)