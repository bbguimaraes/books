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

(define (make-scheme-number n) ((get dispatch-table 'make 'scheme-number) n))
(define (make-rational n d) ((get dispatch-table 'make 'rational) n d))
(define (make-complex-from-real-imag x y)
  ((get dispatch-table 'make-from-real-imag 'complex) x y))
(define (make-complex-from-mag-ang r a)
  ((get dispatch-table 'make-from-mag-ang 'complex) r a))

(define (=zero? n) (apply-generic dispatch-table '=zero? n))

(define (install-=zero?-package d-table)
  (append
    d-table
    (list
      (list '=zero? '(scheme-number)
            (lambda (n) (= 0 n)))
      (list '=zero? '(rational)
            (lambda (n) (or (= 0 (car n))
                            (= 0 (cdr n)))))
      (list '=zero? '(rectangular)
            (lambda (n) (= 0 (car n) (cdr n))))
      (list '=zero? '(polar)
            (lambda (n) (= 0 (car n) (cdr n))))
      (list '=zero? '(complex)
            (lambda (n) (apply-generic dispatch-table '=zero? n))))))

(define dispatch-table
  (install-=zero?-package
    (install-scheme-number-package
      (install-rational-package
        (install-complex-package
          (install-polar-package
            (install-rectangular-package '())))))))

(define (t n)
  (display (=zero? n))
  (display " ")
  (display (=zero? (sub n n)))
  (newline))

(t (make-scheme-number 1))
(t (make-rational 1 2))
(t (make-complex-from-real-imag 1 2))
(t (make-complex-from-mag-ang 1 2))
