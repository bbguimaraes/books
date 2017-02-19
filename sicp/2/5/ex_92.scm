(define (apply-generic dispatch-table op . args)
  (define (coerce type x)
    (let ((coercion-proc (get coercion-table (type-tag x) type)))
      (if coercion-proc
        (coercion-proc x)
        x)))
  (define (apply-coerced type args)
    (let ((coerced-args (map (lambda (x) (coerce type x)) args)))
      (and
        (not (any false? coerced-args))
        (let ((proc (get dispatch-table op (map type-tag coerced-args))))
          (and proc (apply proc (map contents coerced-args)))))))
  (let ((type-tags (map type-tag args)))
    (let ((proc (get dispatch-table op type-tags)))
      (if proc
        (apply proc (map contents args))
        (any (lambda (type) (apply-coerced type args)) type-tags)))))

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
            (lambda (n) (apply-generic dispatch-table '=zero? n)))
      (list '=zero? '(polynomial)
            (lambda (n) (every =zero? (map cadr (cdr n))))))))

(define (install-polynomial-package dispatch-table)
  ;; internal procedures
  ;; representation of poly
  (define (make-poly variable term-list)
    (cons variable term-list))
  (define (variable p) (car p))
  (define (term-list p) (cdr p))
  (define (variable? x) (symbol? x))
  (define (same-variable? x y)
    (and (variable? x) (variable? y) (eq? x y)))
  ;; representation of terms and term lists
  (define (adjoin-term term term-list)
    (if (=zero? (coeff term))
      term-list
      (cons term term-list)))
  (define (the-empty-termlist) '())
  (define (first-term term-list) (car term-list))
  (define (rest-terms term-list) (cdr term-list))
  (define (empty-termlist? term-list) (null? term-list))
  (define (make-term order coeff) (list order coeff))
  (define (order term) (car term))
  (define (coeff term) (cadr term))
  (define (order-0? p) (= 0 (order (first-term (term-list p)))))
  (define (convert-poly v p)
    (if (eq? v (variable p))
      p
      (make-poly
        v (list (list 0 (make-polynomial (variable p) (term-list p)))))))
  (define (add-poly p1 p2)
    (define (inner v p1 p2)
      (make-poly v (add-terms (term-list p1) (term-list p2))))
    (cond
      ((same-variable? (variable p1) (variable p2))
       (inner (variable p1) p1 p2))
      ((order-0? p1) (inner (variable p2) p1 p2))
      ((order-0? p2) (inner (variable p1) p1 p2))
      (else (let ((v (max-variable (variable p1) (variable p2))))
              (inner v (convert-poly v p1) (convert-poly v p2))))))
  (define (add-terms L1 L2)
    (cond ((empty-termlist? L1) L2)
          ((empty-termlist? L2) L1)
          (else
            (let ((t1 (first-term L1)) (t2 (first-term L2)))
              (cond ((> (order t1) (order t2))
                     (adjoin-term
                       t1 (add-terms (rest-terms L1) L2)))
                    ((< (order t1) (order t2))
                     (adjoin-term
                       t2 (add-terms L1 (rest-terms L2))))
                    (else
                      (adjoin-term
                        (make-term (order t1)
                                   (add (coeff t1) (coeff t2)))
                        (add-terms (rest-terms L1)
                                   (rest-terms L2)))))))))
  (define (mul-poly p1 p2)
    (define (mul-terms L1 L2)
      (if (empty-termlist? L1)
        (the-empty-termlist)
        (add-terms (mul-term-by-all-terms (first-term L1) L2)
                   (mul-terms (rest-terms L1) L2))))
    (define (mul-term-by-all-terms t1 L)
      (if (empty-termlist? L)
        (the-empty-termlist)
        (let ((t2 (first-term L)))
          (adjoin-term
            (make-term (+ (order t1) (order t2))
                       (mul (coeff t1) (coeff t2)))
            (mul-term-by-all-terms t1 (rest-terms L))))))
    (define (inner v p1 p2)
      (make-poly v (mul-terms (term-list p1) (term-list p2))))
    (cond
      ((same-variable? (variable p1) (variable p2))
       (inner (variable p1) p1 p2))
      ((order-0? p1) (inner (variable p2) p1 p2))
      ((order-0? p2) (inner (variable p1) p1 p2))
      (else (let ((v (max-variable (variable p1) (variable p2))))
              (inner v (convert-poly v p1) (convert-poly v p2))))))
  ;; interfaces to rest of the system
  (define (tag p) (attach-tag 'polynomial p))
  (append
    dispatch-table
    (list
      (list 'add '(polynomial polynomial)
            (lambda (p1 p2) (tag (add-poly p1 p2))))
      (list 'mul '(polynomial polynomial)
            (lambda (p1 p2) (tag (mul-poly p1 p2))))
      (list 'make 'polynomial
            (lambda (var terms) (tag (make-poly var terms)))))))

(define (scheme-number->polynomial n)
  (make-polynomial 'a (list (list 0 n))))

(define coercion-table
  (list
    (list 'scheme-number 'polynomial scheme-number->polynomial)))

(define dispatch-table
  (install-polynomial-package
    (install-scheme-number-package
      (install-=zero?-package '()))))

(define (make-polynomial var terms)
  ((get dispatch-table 'make 'polynomial) var terms))
(define (=zero? n) (apply-generic dispatch-table '=zero? n))
(define (add x y) (apply-generic dispatch-table 'add x y))
(define (mul x y) (apply-generic dispatch-table 'mul x y))

(let ((p1 (make-polynomial 'x (list (list 2 1) (list 1 2) (list 0 3))))
      (p2 (make-polynomial 'x (list (list 2 4) (list 1 5) (list 0 6)))))
  (display (add p1 p2))
  (newline)
  (display (mul p1 p2))
  (newline))

(define (variable<? s0 s1)
  (char<? (string-ref (symbol->string s0) 0)
          (string-ref (symbol->string s1) 0)))

(define (max-variable v0 v1) (if (variable<? v0 v1) v1 v0))

(define (t p1 p2)
  (display (add p1 p2))
  (newline)
  (display (add p2 p1))
  (newline)
  (display (mul p1 p2))
  (newline)
  (display (mul p2 p1))
  (newline))

(t (make-polynomial 'x (list (list 2 1) (list 0 4)))
   (make-polynomial 'y (list (list 2 2) (list 1 1))))
(newline)
(t (make-polynomial 'x (list (list 2 1) (list 1 2) (list 0 3)))
   (make-polynomial 'y (list (list 2 4) (list 1 5) (list 0 6))))
