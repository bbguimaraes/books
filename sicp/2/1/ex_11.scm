(define (mul-interval x y)
  (let ((p1 (* (lower-bound x) (lower-bound y)))
        (p2 (* (lower-bound x) (upper-bound y)))
        (p3 (* (upper-bound x) (lower-bound y)))
        (p4 (* (upper-bound x) (upper-bound y))))
    (make-interval (min p1 p2 p3 p4)
                   (max p1 p2 p3 p4))))

(define (make-interval a b) (if (<= a b) (cons a b) (make-interval b a)))
(define (lower-bound x) (car x))
(define (upper-bound x) (cdr x))

(define (print-interval i)
  (display "(")
  (display (lower-bound i))
  (display ",")
  (display (upper-bound i))
  (display ")"))

(define i1 (make-interval 6.12 7.48))
(define i2 (make-interval 4.46 4.93))

(print-interval (mul-interval i1 i2))
(newline)

(define (mul-interval x y)
  (define (p? x) (>= x 0))
  (define (n? x) (< x 0))
  (define (ret x0 x1 y0 y1) (make-interval (* x0 x1) (* y0 y1)))
  (let ((x0 (lower-bound x))
        (x1 (upper-bound x))
        (y0 (lower-bound y))
        (y1 (upper-bound y)))
    (cond
      ((p? x0)
       (cond
         ((p? y0) (ret x0 y0 x1 y1))
         ((p? y1) (ret x1 y0 x1 y1))
         (else    (ret x1 y0 x0 y1))))
      ((p? x1)
       (cond
         ((p? y0) (ret x0 y1 x1 y1))
         ((p? y1) (make-interval (min (* x0 y1) (* x1 y0))
                                 (max (* x0 y0) (* x1 y1))))
         (else    (ret x1 y0 x0 y0))))
      (else
        (cond
          ((p? y0) (ret x0 y1 x1 y0))
          ((p? y1) (ret x0 y1 x0 y0))
          (else    (ret x1 x1 x0 y0)))))))


(print-interval (mul-interval i1 i2))
(newline)
