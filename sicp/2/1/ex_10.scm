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

(define (print-interval i)
  (display "(")
  (display (lower-bound i))
  (display ",")
  (display (upper-bound i))
  (display ")"))

(define i1 (make-interval 6.12 7.48))
(print-interval (div-interval i1 (make-interval 0 0)))
(newline)

(define (interval-width i)
  (- (upper-bound i) (lower-bound i)))

(define (div-interval x y)
  (if (= (interval-width y) 0) (error "Dividing by zero-width interval"))
  (mul-interval x
                (make-interval (/ 1.0 (upper-bound y))
                               (/ 1.0 (lower-bound y)))))

(print-interval (div-interval i1 (make-interval 0 0)))
(newline)
