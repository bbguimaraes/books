(define (make-mobile left right)
  (list left right))
(define (make-branch length structure)
  (list length structure))

(define (left-branch m) (car m))
(define (right-branch m) (cadr m))
(define (branch-length b) (car b))
(define (branch-structure b) (cadr b))

(define (structure-weight s) (if (pair? s) (total-weight s) s))
(define (total-weight m)
  (if (null? m)
    0
    (+ (structure-weight (branch-structure (left-branch m)))
       (structure-weight (branch-structure (right-branch m))))))

(define m1 (make-mobile (make-branch 1 5) (make-branch 2 10)))
(display m1)
(newline)
(display (total-weight m1))

(define m2
  (make-mobile
    (make-branch
      1
      (make-mobile
        (make-branch 3 5)
        (make-branch 4 4)))
    (make-branch 2 10)))

(newline)
(display m2)
(newline)
(display (total-weight m2))

(define (branch-weight b)
  (let ((s (branch-structure b)))
    (* (branch-length b)
       (if (not (pair? s))
         (structure-weight s)
         (+ (branch-weight (left-branch s))
            (branch-weight (right-branch s)))))))

(define (branch-balanced? b)
  (let ((s (branch-structure b)))
    (or (not (pair? s))
        (and (mobile-balanced? s)))))

(define (mobile-balanced? m)
  (or (null? m)
      (and (= (branch-weight (left-branch m))
              (branch-weight (right-branch m)))
           (branch-balanced? (left-branch m))
           (branch-balanced? (right-branch m)))))

(define m3
  (make-mobile
    (make-branch
      5
      (make-mobile
        (make-branch 1 8)
        (make-branch 6 4)))
    (make-branch 2 80)))
(define m4
  (make-mobile
    (make-branch
      5
      (make-mobile
        (make-branch 2 8)
        (make-branch 4 4)))
    (make-branch 2 80)))

(newline)
(display (mobile-balanced? m1))
(newline)
(display (mobile-balanced? m2))
(newline)
(display (mobile-balanced? m3))
(newline)
(display (mobile-balanced? m4))
(newline)



(define (make-mobile left right)
  (cons left right))
(define (make-branch length structure)
  (cons length structure))
(define (right-branch m) (cdr m))
(define (branch-structure b) (cdr b))

(define m1 (make-mobile (make-branch 1 5) (make-branch 2 10)))
(define m2
  (make-mobile
    (make-branch
      1
      (make-mobile
        (make-branch 3 5)
        (make-branch 4 4)))
    (make-branch 2 10)))
(define m3
  (make-mobile
    (make-branch
      5
      (make-mobile
        (make-branch 1 8)
        (make-branch 6 4)))
    (make-branch 2 80)))
(define m4
  (make-mobile
    (make-branch
      5
      (make-mobile
        (make-branch 2 8)
        (make-branch 4 4)))
    (make-branch 2 80)))

(newline)
(display m1)
(newline)
(display (total-weight m1))
(newline)
(display m2)
(newline)
(display (total-weight m2))
(newline)
(display (mobile-balanced? m1))
(newline)
(display (mobile-balanced? m2))
(newline)
(display (mobile-balanced? m3))
(newline)
(display (mobile-balanced? m4))
(newline)
