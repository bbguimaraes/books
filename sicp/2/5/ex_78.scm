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

(define dispatch-table
  (install-scheme-number-package '()))

(define (t x y)
  (map display (list x " " y))
  (newline)
  (display (add x y))
  (newline)
  (display (sub x y))
  (newline)
  (display (mul x y))
  (newline)
  (display (div x y))
  (newline))

(t 2 3)
