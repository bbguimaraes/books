(define (apply-generic dispatch-table op . args)
  (define (coerce type x)
    (let ((coercion-proc (get coercion-table (type-tag x) type)))
      (if coercion-proc
        (coercion-proc x)
        x)))
  (define (apply-coerced type args)
    (let ((coerced-args (map (lambda (x) (coerce type x)) args)))
      (if (not coerced-args)
        (error "no method for these types"
               (list op (map type-tag args)))
        (let ((proc (get dispatch-table op (map type-tag coerced-args))))
          (if proc
            (apply proc coerced-args)
            (error "no method for these types"
                   (list op (map type-tag args))))))))
  (let ((type-tags (map type-tag args)))
    (let ((proc (get dispatch-table op type-tags)))
      (if proc
        (apply proc (map contents args))
        (apply-coerced (car type-tags) args)))))

(define (get table op type-tags)
  (if (null? table)
    #f
    (let ((x (car table)))
      (if (and
            (eq? (car x) op)
            (if (pair? type-tags)
              (list-eq? (cadr x) type-tags)
              (eq? (cadr x) type-tags)))
        (caddr x)
        (get (cdr table) op type-tags)))))

(define (list-eq? x y)
  (cond
    ((null? x) (null? y))
    ((null? y) (null? x))
    ((not (eq? (car x) (car y))) #f)
    (else (list-eq? (cdr x) (cdr y)))))

(define (attach-tag type-tag contents)
  (cons type-tag contents))
(define (type-tag datum)
  (if (pair? datum)
    (car datum)
    (error "Bad tagged datum -- TYPE-TAG" datum)))
(define (contents datum)
  (if (pair? datum)
    (cdr datum)
    (error "Bad tagged datum -- CONTENTS" datum)))

(define (add x y) (apply-generic dispatch-table 'add x y))
(define (sub x y) (apply-generic dispatch-table 'sub x y))
(define (mul x y) (apply-generic dispatch-table 'mul x y))
(define (div x y) (apply-generic dispatch-table 'div x y))

(define (install-scheme-number-package dispatch-table)
  (define (tag x) (attach-tag 'scheme-number x))
  (append
    dispatch-table
    (list
      (list 'add '(scheme-number scheme-number) (lambda (x y) (tag (+ x y))))
      (list 'sub '(scheme-number scheme-number) (lambda (x y) (tag (- x y))))
      (list 'mul '(scheme-number scheme-number) (lambda (x y) (tag (* x y))))
      (list 'div '(scheme-number scheme-number) (lambda (x y) (tag (/ x y))))
      (list 'make 'scheme-number (lambda (x) (tag x))))))

(define (real-part z) (apply-generic dispatch-table 'real-part z))
(define (imag-part z) (apply-generic dispatch-table 'imag-part z))
(define (magnitude z) (apply-generic dispatch-table 'magnitude z))
(define (angle z) (apply-generic dispatch-table 'angle z))

(define (install-rectangular-package dispatch-table)
  ;; internal procedures
  (define (real-part z) (car z))
  (define (imag-part z) (cdr z))
  (define (magnitude z)
    (sqrt (+ (square (real-part z))
             (square (imag-part z)))))
  (define (angle z)
    (atan (imag-part z) (real-part z)))
  (define (make-from-real-imag x y) (cons x y))
  ;; interface to the rest of the system
  (define (tag x) (attach-tag 'rectangular x))
  (append
    dispatch-table
    (list
      (list 'real-part '(rectangular) real-part)
      (list 'imag-part '(rectangular) imag-part)
      (list 'magnitude '(rectangular) magnitude)
      (list 'angle '(rectangular) angle)
      (list 'make-from-real-imag 'rectangular
           (lambda (x y) (tag (make-from-real-imag x y)))))))

(define (install-complex-package dispatch-table)
  ;; imported procedures from rectangular and polar packages
  (define (make-from-real-imag x y)
    ((get dispatch-table 'make-from-real-imag 'rectangular) x y))
  (define (make-from-mag-ang r a)
    ((get dispatch-table 'make-from-mag-ang 'polar) r a))
  ;; internal procedures
  (define (add-complex z1 z2)
    (make-from-real-imag (+ (real-part z1) (real-part z2))
                         (+ (imag-part z1) (imag-part z2))))
  (define (sub-complex z1 z2)
    (make-from-real-imag (- (real-part z1) (real-part z2))
                         (- (imag-part z1) (imag-part z2))))
  (define (mul-complex z1 z2)
    (make-from-real-imag (* (magnitude z1) (magnitude z2))
                         (+ (angle z1) (angle z2))))
  (define (div-complex z1 z2)
    (make-from-real-imag (/ (magnitude z1) (magnitude z2))
                         (- (angle z1) (angle z2))))
  ;; interface to the rest of the system
  (define (tag z) (attach-tag 'complex z))
  (append
    dispatch-table
    (list
      (list 'add '(complex complex) (lambda (z1 z2) (tag (add-complex z1 z2))))
      (list 'sub '(complex complex) (lambda (z1 z2) (tag (sub-complex z1 z2))))
      (list 'mul '(complex complex) (lambda (z1 z2) (tag (mul-complex z1 z2))))
      (list 'div '(complex complex) (lambda (z1 z2) (tag (div-complex z1 z2))))
      (list 'make-from-real-imag 'complex
            (lambda (x y) (tag (make-from-real-imag x y))))
      (list 'make-from-mag-ang 'complex
            (lambda (r a) (tag (make-from-mag-ang r a)))))))

(define (make-scheme-number n) ((get dispatch-table 'make 'scheme-number) n))
(define (make-complex-from-real-imag x y)
  ((get dispatch-table 'make-from-real-imag 'complex) x y))

(define (scheme-number->complex n)
  (make-complex-from-real-imag (contents n) 0))

(define coercion-table
  (list
    (list 'scheme-number 'complex scheme-number->complex)))

(define
  dispatch-table
  (install-scheme-number-package
    (install-complex-package
      (install-rectangular-package
        (list
          (list 'add3 '(scheme-number scheme-number scheme-number)
                (lambda (x y z) (+ x y z)))
          (list 'add3 '(complex complex complex)
                (lambda (x y z) (add (add x y) z))))))))

(define (add3 x y z) (apply-generic dispatch-table 'add3 x y z))

(display
  (add3
    (make-scheme-number 1)
    (make-scheme-number 2)
    (make-scheme-number 3)))
(newline)

(display
  (add3
    (make-complex-from-real-imag 3 4)
    (make-scheme-number 1)
    (make-scheme-number 2)))
(newline)
