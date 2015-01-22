(define (add-interval x y)
  (make-interval (+ (lower-bound x) (lower-bound y))
                 (+ (upper-bound x) (upper-bound y))))

(define (mul-interval x y)
  (let ((p1 (* (lower-bound x) (lower-bound y)))
        (p2 (* (lower-bound x) (upper-bound y)))
        (p3 (* (upper-bound x) (lower-bound y)))
        (p4 (* (upper-bound x) (upper-bound y))))
    (make-interval (min p1 p2 p3 p4)
                   (max p1 p2 p3 p4))))

(define (div-interval x y)
  (mul-interval x
                (make-interval (/ 1.0 (upper-bound y))
                               (/ 1.0 (lower-bound y)))))

(define (make-interval a b) (cons a b))
(define (lower-bound x) (car x))
(define (upper-bound x) (cdr x))

(define (sub-interval x y)
  (make-interval (- (lower-bound x) (lower-bound y))
                 (- (upper-bound x) (upper-bound y))))

(define i1 (make-interval 6.12 7.48))
(define i2 (make-interval 4.46 4.93))

(define (interval-width i)
  (- (upper-bound i) (lower-bound i)))

(display (interval-width i1))
(newline)

(display (interval-width i2))
(newline)

(display (+ (interval-width i1) (interval-width i2)))
(display " ")
(display (interval-width (add-interval i1 i2)))
(newline)

(display (- (interval-width i1) (interval-width i2)))
(display " ")
(display (interval-width (sub-interval i1 i2)))
(newline)

(display (* (interval-width i1) (interval-width i2)))
(display " ")
(display (interval-width (mul-interval i1 i2)))
(newline)

(display (/ (interval-width i1) (interval-width i2)))
(display " ")
(display (interval-width (div-interval i1 i2)))
(newline)
