(define (make-point x y) (cons x y))
(define (x-point p) (car p))
(define (y-point p) (cdr p))

(define (make-segment start end) (cons start end))
(define (start-segment l) (car l))
(define (end-segment l) (cdr l))

(define (segment-length l)
  (let ((start (start-segment l)) (end (end-segment l)))
    (let ((dx (- (x-point end) (x-point start)))
          (dy (- (y-point end) (y-point start))))
    (sqrt (+ (* dx dx) (* dy dy))))))

(define (make-rect bt-left up-right) (cons bt-left up-right))
(define (rect-bt-left r) (car r))
(define (rect-bt-right r) (make-point (x-point (cdr r))
                                      (y-point (car r))))
(define (rect-up-right r) (cdr r))
(define (rect-up-left r) (make-point (x-point (car r))
                                     (y-point (cdr r))))

(define (rect-perimeter r)
  (let ((bt-left (rect-bt-left r))
        (bt-right (rect-bt-right r))
        (up-right (rect-up-right r))
        (up-left (rect-up-left r)))
    (+ (segment-length (make-segment bt-left bt-right))
       (segment-length (make-segment bt-right up-right))
       (segment-length (make-segment up-right up-left))
       (segment-length (make-segment up-left bt-left)))))

(define (rect-area r)
  (let ((bt-left (rect-bt-left r))
        (bt-right (rect-bt-right r))
        (up-left (rect-up-left r)))
    (* (- (x-point bt-right) (x-point bt-left))
       (- (y-point up-left) (y-point bt-left)))))


(define r1 (make-rect (make-point 1 2) (make-point 5 4)))
(display (rect-perimeter r1))
(newline)
(display (rect-area r1))
(newline)

(define (make-rect c w h) (cons c (cons w h)))
(define (rect-bt-left r)
  (let ((c (car r)) (w (car (cdr r))) (h (cdr (cdr r))))
    (make-point (- (x-point c) (/ w 2))
                (- (y-point c) (/ h 2)))))
(define (rect-bt-right r)
  (let ((c (car r)) (w (car (cdr r))) (h (cdr (cdr r))))
    (make-point (+ (x-point c) (/ w 2))
                (- (y-point c) (/ h 2)))))
(define (rect-up-right r)
  (let ((c (car r)) (w (car (cdr r))) (h (cdr (cdr r))))
    (make-point (+ (x-point c) (/ w 2))
                (+ (y-point c) (/ h 2)))))
(define (rect-up-left r)
  (let ((c (car r)) (w (car (cdr r))) (h (cdr (cdr r))))
    (make-point (- (x-point c) (/ w 2))
                (+ (y-point c) (/ h 2)))))

(define r1 (make-rect (make-point 3 3) 4 2))
(display (rect-perimeter r1))
(newline)
(display (rect-area r1))
(newline)