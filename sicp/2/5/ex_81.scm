(define (apply-generic dispatch-table op . args)
  (define (coerce t1 t2)
    (and (not (eq? t1 t2))
         (get coercion-table t1 t2)))
  (let ((type-tags (map type-tag args)))
    (let ((proc (get dispatch-table op type-tags)))
      (if proc
        (apply proc (map contents args))
        (if (= (length args) 2)
          (let ((type1 (car type-tags))
                (type2 (cadr type-tags))
                (a1 (car args))
                (a2 (cadr args)))
            (let ((t1->t2 (coerce type1 type2))
                  (t2->t1 (coerce type2 type1)))
              (cond (t1->t2 (apply-generic dispatch-table op (t1->t2 a1) a2))
                    (t2->t1 (apply-generic dispatch-table op a1 (t2->t1 a2)))
                    (else
                      (error "No method for these types"
                             (list op type-tags))))))
          (error "No method for these types"
                 (list op type-tags)))))))

;(define (apply-generic dispatch-table op . args)
;  (let ((type-tags (map type-tag args)))
;    (let ((proc (get dispatch-table op type-tags)))
;      (if proc
;          (apply proc (map contents args))
;          (if (= (length args) 2)
;              (let ((type1 (car type-tags))
;                    (type2 (cadr type-tags))
;                    (a1 (car args))
;                    (a2 (cadr args)))
;                (let ((t1->t2 (get coercion-table type1 type2))
;                      (t2->t1 (get coercion-table type2 type1)))
;                  (cond (t1->t2
;                         (apply-generic dispatch-table op (t1->t2 a1) a2))
;                        (t2->t1
;                         (apply-generic dispatch-table op a1 (t2->t1 a2)))
;                        (else
;                         (error "No method for these types"
;                                (list op type-tags))))))
;              (error "No method for these types"
;                     (list op type-tags)))))))

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

(define (make-scheme-number n) ((get dispatch-table 'make 'scheme-number) n))

(define dispatch-table
  (install-scheme-number-package '()))

(define (scheme-number->complex n)
  (make-complex-from-real-imag (contents n) 0))
(define (scheme-number->scheme-number n) n)
(define (complex->complex z) z)

(define coercion-table
  (list
    (list 'scheme-number 'complex scheme-number->complex)
    (list 'scheme-number 'scheme-number scheme-number->scheme-number)
    (list 'complex 'complex complex->complex)))

(apply-generic
  dispatch-table
  'uncomment-the-original-apply-generic-to-warm-your-house
  (make-scheme-number 1)
  (make-scheme-number 3))
