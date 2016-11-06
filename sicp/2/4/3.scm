(define (list-eq? x y)
  (cond
    ((null? x) (null? y))
    ((null? y) (null? x))
    ((not (eq? (car x) (car y))) #f)
    (else (list-eq? (cdr x) (cdr y)))))

(define (get dispatch-table op type-tags)
  (if (null? dispatch-table)
    #f
    (let ((x (car dispatch-table)))
      (if (and
            (eq? (car x) op)
            (if (pair? type-tags)
              (list-eq? (cadr x) type-tags)
              (eq? (cadr x) type-tags)))
        (caddr x)
        (get (cdr dispatch-table) op type-tags)))))

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

(define (install-rectangular-package dispatch-table)
  ;; internal procedures
  (define (real-part z) (car z))
  (define (imag-part z) (cdr z))
  (define (make-from-real-imag x y) (cons x y))
  (define (magnitude z)
    (sqrt (+ (square (real-part z))
             (square (imag-part z)))))
  (define (angle z)
    (atan (imag-part z) (real-part z)))
  (define (make-from-mag-ang r a)
    (cons (* r (cos a)) (* r (sin a))))
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
           (lambda (x y) (tag (make-from-real-imag x y))))
      (list 'make-from-mag-ang 'rectangular
           (lambda (r a) (tag (make-from-mag-ang r a)))))))

(define (install-polar-package dispatch-table)
  ;; internal procedures
  (define (magnitude z) (car z))
  (define (angle z) (cdr z))
  (define (make-from-mag-ang r a) (cons r a))
  (define (real-part z)
    (* (magnitude z) (cos (angle z))))
  (define (imag-part z)
    (* (magnitude z) (sin (angle z))))
  (define (make-from-real-imag x y)
    (cons (sqrt (+ (square x) (square y)))
          (atan x y)))
  ;; interface to the rest of the system
  (define (tag x) (attach-tag 'polar x))
  (append
    dispatch-table
    (list
      (list 'real-part '(polar) real-part)
      (list 'imag-part '(polar) imag-part)
      (list 'magnitude '(polar) magnitude)
      (list 'angle '(polar) angle)
      (list 'make-from-real-imag 'polar
           (lambda (x y) (tag (make-from-real-imag x y))))
      (list 'make-from-mag-ang 'polar
           (lambda (r a) (tag (make-from-mag-ang r a)))))))

(define (apply-generic dispatch-table op . args)
  (let ((type-tags (map type-tag args)))
    (let ((proc (get dispatch-table op type-tags)))
      (if proc
        (apply proc (map contents args))
        (error
          "No method for these types -- APPLY-GENERIC"
          (list op type-tags))))))

(define (real-part dispatch-table z)
  (apply-generic dispatch-table 'real-part z))
(define (imag-part dispatch-table z)
  (apply-generic dispatch-table 'imag-part z))
(define (magnitude dispatch-table z)
  (apply-generic dispatch-table 'magnitude z))
(define (angle dispatch-table z)
  (apply-generic dispatch-table 'angle z))

(define (make-from-real-imag dispatch-table x y)
  ((get dispatch-table 'make-from-real-imag 'rectangular) x y))
(define (make-from-mag-ang dispatch-table r a)
  ((get dispatch-table 'make-from-mag-ang 'polar) r a))

(define (t dispatch-table)
  (let ((x (make-from-real-imag dispatch-table 3 4))
        (y (make-from-mag-ang dispatch-table 3 4)))
    (display x)
    (newline)
    (display (real-part dispatch-table x))
    (newline)
    (display (magnitude dispatch-table x))
    (newline)
    (display y)
    (newline)
    (display (real-part dispatch-table y))
    (newline)
    (display (magnitude dispatch-table y))
    (newline)))

(let
  ((dispatch-table (install-rectangular-package '())))
  (let ((dispatch-table (install-polar-package dispatch-table)))
    (t dispatch-table)))

(define (make-from-real-imag x y)
  (define (dispatch op)
    (cond
      ((eq? op 'real-part) x)
      ((eq? op 'imag-part) y)
      ((eq? op 'magnitude)
       (sqrt (+ (square x) (square y))))
      ((eq? op 'angle) (atan y x))
      (else (error "Unknown op -- MAKE-FROM-REAL-IMAG" op))))
  dispatch)
(define (apply-generic op arg) (arg op))
(define (real-part z) (apply-generic 'real-part z))
(define (magnitude z) (apply-generic 'magnitude z))

(newline)
(let ((x (make-from-real-imag 3 4)))
  (display x)
  (newline)
  (display (real-part x))
  (newline)
  (display (magnitude x))
  (newline))
