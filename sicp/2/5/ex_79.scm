(define (apply-generic dispatch-table op . args)
  (let ((type-tags (map type-tag args)))
    (let ((proc (get dispatch-table op type-tags)))
      (if proc
        (apply proc (map contents args))
        (error
          "No method for these types -- APPLY-GENERIC"
          (list op type-tags))))))

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
  (if (number? contents)
    contents
    (cons type-tag contents)))
(define (type-tag datum)
  (if (number? datum)
    'scheme-number
    (if (pair? datum)
      (car datum)
      (error "Bad tagged datum -- TYPE-TAG" datum))))
(define (contents datum)
  (if (number? datum)
    datum
    (if (pair? datum)
      (cdr datum)
      (error "Bad tagged datum -- CONTENTS" datum))))

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

(define (install-polar-package dispatch-table)
  ;; internal procedures
  (define (magnitude z) (car z))
  (define (real-part z)
    (* (magnitude z) (cos (angle z))))
  (define (imag-part z)
    (* (magnitude z) (sin (angle z))))
  (define (angle z) (cdr z))
  (define (make-from-mag-ang r a) (cons r a))
  ;; interface to the rest of the system
  (define (tag x) (attach-tag 'polar x))
  (append
    dispatch-table
    (list
      (list 'real-part '(polar) real-part)
      (list 'imag-part '(polar) imag-part)
      (list 'magnitude '(polar) magnitude)
      (list 'angle '(polar) angle)
      (list 'make-from-mag-ang 'polar
           (lambda (r a) (tag (make-from-mag-ang r a)))))))

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

(define (equ? x y) (apply-generic dispatch-table 'equ? x y))

(define (install-equ?-package d-table)
  (append
    d-table
    (list
      (list 'equ? '(scheme-number scheme-number)
            (lambda (x y) (= x y)))
      (list 'equ? '(rational rational)
            (lambda (x y) (and (= (car x) (car y))
                               (= (cdr x) (cdr y)))))
      (list 'equ? '(rectangular rectangular)
            (lambda (x y) (and (= (car x) (car y))
                               (= (cdr x) (cdr y)))))
      (list 'equ? '(polar polar)
            (lambda (x y) (and (= (car x) (car y))
                               (= (cdr x) (cdr y)))))
      (list 'equ? '(complex complex)
            (lambda (x y)
              (apply-generic dispatch-table 'equ? x y))))))

(define dispatch-table
  (install-equ?-package
    (install-complex-package
      (install-rational-package
        (install-polar-package
          (install-rectangular-package '()))))))

(define (make-scheme-number n) ((get dispatch-table 'make 'scheme-number) n))
(define (make-rational n d) ((get dispatch-table 'make 'rational) n d))
(define (make-complex-from-real-imag x y)
  ((get dispatch-table 'make-from-real-imag 'complex) x y))
(define (make-complex-from-mag-ang r a)
  ((get dispatch-table 'make-from-mag-ang 'complex) r a))

(define (t x y)
  (display (equ? x y))
  (display " ")
  (display (equ? x x))
  (display " ")
  (display (equ? y y))
  (newline))

(t 2 3))
(t (make-rational 1 2) (make-rational 3 4)))
(t (make-complex-from-real-imag 1 2) (make-complex-from-real-imag 3 4)))
(t (make-complex-from-mag-ang 1 2) (make-complex-from-mag-ang 3 4)))
