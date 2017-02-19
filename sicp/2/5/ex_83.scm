(define (make-integer x)
  (cons 'integer x))
(define (make-rational x)
  (cons 'rational x))
(define (make-real x)
  (cons 'real x))
(define (make-complex x)
  (cons 'complex x))

(define (raise tower x)
  (cond
    ((null? tower) #f)
    ((eq? (car x) (caar tower))
     ((cadar tower) x))
    (else
      (raise (cdr tower) x))))

(define tower
  (list
    (list 'integer (lambda (x) (make-rational (cdr x))))
    (list 'rational (lambda (x) (make-real (cdr x))))
    (list 'real (lambda (x) (make-complex (cdr x))))))

(let ((raise (lambda (x) (raise tower x))))
  (display (raise (make-integer 0)))
  (newline)
  (display (raise (make-rational 0)))
  (newline)
  (display (raise (make-real 0)))
  (newline)
  (display (raise (make-complex 0)))
  (newline))
