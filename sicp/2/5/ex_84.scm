(define (apply-generic dispatch-table op . args)
  (let ((type-tags (map type-tag args)))
    (let ((proc (get dispatch-table op type-tags)))
      (if proc
        (apply proc (map contents args))
        (apply apply-coerced (cons op args))))))

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

(define (install-rational-package dispatch-table)
  ;; internal procedures
  (define (numer x) (car x))
  (define (denom x) (cdr x))
  (define (make-rat n d)
    (let ((g (gcd n d)))
      (cons (/ n g) (/ d g))))
  (define (add-rat x y)
    (make-rat (+ (* (numer x) (denom y))
                 (* (numer y) (denom x)))
              (* (denom x) (denom y))))
  (define (sub-rat x y)
    (make-rat (- (* (numer x) (denom y))
                 (* (numer y) (denom x)))
              (* (denom x) (denom y))))
  (define (mul-rat x y)
    (make-rat (* (numer x) (numer y))
              (* (denom x) (denom y))))
  (define (div-rat x y)
    (make-rat (* (numer x) (denom y))
              (* (denom x) (numer y))))
  ;; interface to the rest of the system
  (define (tag x) (attach-tag 'rational x))
  (append
    dispatch-table
    (list
      (list 'add '(rational rational) (lambda (x y) (tag (add-rat x y))))
      (list 'sub '(rational rational) (lambda (x y) (tag (sub-rat x y))))
      (list 'mul '(rational rational) (lambda (x y) (tag (mul-rat x y))))
      (list 'div '(rational rational) (lambda (x y) (tag (div-rat x y))))
      (list 'make 'rational (lambda (n d) (tag (make-rat n d)))))))

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
(define (make-rational n d) ((get dispatch-table 'make 'rational) n d))
(define (make-complex-from-real-imag x y)
  ((get dispatch-table 'make-from-real-imag 'complex) x y))

(define dispatch-table
  (install-scheme-number-package
    (install-rational-package
      (install-complex-package
        (install-rectangular-package '())))))

(define (apply-coerced op . args)
  (let ((to-type (higher-type tower (map type-tag args))))
    (let ((raised (map (lambda (x) (raise-to tower to-type x)) args)))
      (apply apply-generic (append (list dispatch-table op) raised)))))

(define (higher-type tower types)
  (define (higher-type-iter h types)
    (if (null? types)
      h
      (let ((higher (if (higher-type? tower h (car types)) h (car types))))
        (higher-type-iter higher (cdr types)))))
  (higher-type-iter (car types) (cdr types))))

(define (higher-type? tower t0 t1)
  (and
    (null? tower)
    (let ((from-type (caar tower))
          (to-type (cadar tower)))
      (and
        (eq? t0 from-type)
        (and
          (not (eq? t1 to-type))
          (higher-type? tower to-type t1)))
      (higher-type? (cdr tower) t0 t1))))

(define (raise-to tower type x)
  (if (eq? type (type-tag x))
    x
    (raise-to tower type (raise tower x))))

(define (raise tower x)
  (cond
    ((null? tower) #f)
    ((eq? (caar tower) (type-tag x)) ((caddar tower) x))
    (else (raise (cdr tower) x))))

(define (raise-scheme-number x) (make-rational (contents x) 1))
(define (raise-rational x)
  (let ((x (contents x))) (make-complex-from-real-imag (/ (car x) (cdr x)) 0)))

(define tower
  (list
    (list 'scheme-number 'rational raise-scheme-number)
    (list 'rational 'complex raise-rational)))

(display (add (make-scheme-number 1) (make-scheme-number 2)))
(newline)
(display (add (make-scheme-number 1) (make-rational 2 3)))
(newline)
(display (add (make-scheme-number 1) (make-complex-from-real-imag 2 3)))
(newline)
(display (add (make-rational 2 3) (make-complex-from-real-imag 2 3)))
(newline)

(define (add . args)
  (apply
    apply-generic
    (append (list dispatch-table 'add) args)))

(define (add3-rat x y z)
  (add (cons 'rational x) (add (cons 'rational y) (cons 'rational z))))
(define dispatch-table
  (append
    dispatch-table
    (list (list 'add '(rational rational rational) add3-rat))))

(display
  (add
    (make-scheme-number 1)
    (make-scheme-number 2)
    (make-rational 3 4)))
(newline)

(define (add3-complex x y z)
  (add (cons 'complex x) (add (cons 'complex y) (cons 'complex z))))
(define dispatch-table
  (append
    dispatch-table
    (list (list 'add '(complex complex complex) add3-complex))))

(display
  (add
    (make-scheme-number 1)
    (make-rational 2 3)
    (make-complex-from-real-imag 4 5)))
(newline)
