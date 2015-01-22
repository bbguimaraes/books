(define x (list (list 1 2) (list 3 4)))
(display x)
(newline)

(define (fringe l)
  (cond ((null? l) l)
        ((not (pair? (car l)))
         (cons (car l) (fringe (cdr l))))
        (else (append (fringe (car l)) (fringe (cdr l))))))

(display (fringe x))
(newline)
(display (fringe (list x x)))
(newline)
