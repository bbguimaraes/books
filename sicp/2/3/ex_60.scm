(define (element-of-set? x set)
  (and (not (null? set))
       (or (equal? x (car set))
           (element-of-set? x (cdr set)))))

(define s (list 2 3 2 1 3 2 2))
(display (element-of-set? 2 s))
(newline)
(display (element-of-set? 31415 s))
(newline)

(define (adjoin-set x set) (cons x set))

(display (adjoin-set 2 s))
(newline)
(display (adjoin-set 31415 s))
(newline)

(define (intersection-set set1 set2)
  (cond ((or (null? set1) (null? set2)) '())
        ((element-of-set? (car set1) set2)
         (cons (car set1)
               (intersection-set (cdr set1) set2)))
        (else (intersection-set (cdr set1) set2))))

(display (intersection-set s (list 2 3 5 31415)))
(newline)

(define (union-set set1 set2)
  (append set1 set2))

(display (union-set (list 1 3 5 7) (list 2 3 4 6)))
(newline)
