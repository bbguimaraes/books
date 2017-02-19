(define (apply-generic dispatch-table op . args)
  (let ((type-tags (map type-tag args)))
    (let ((proc (get dispatch-table op type-tags)))
      (if proc
        (apply proc (map contents args))
        (error
          "No method for these types -- APPLY-GENERIC"
          (list op type-tags))))))

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
  (define (add-poly p1 p2)
    (if (same-variable? (variable p1) (variable p2))
      (make-poly (variable p1)
                 (add-terms (term-list p1)
                            (term-list p2)))
      (error "Polys not in same var -- ADD-POLY"
             (list p1 p2))))
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
  (define (sub-poly p1 p2)
    (define (negate-terms p)
      (cons
        (variable p)
        (map (lambda (term) (list (car term) (negate (cadr term))))
             (term-list p))))
    (tag (add-poly p1 (negate-terms p2))))
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
    (if (same-variable? (variable p1) (variable p2))
      (make-poly (variable p1)
                 (mul-terms (term-list p1)
                            (term-list p2)))
      (error "Polys not in same var -- MUL-POLY"
             (list p1 p2))))
  ;; interfaces to rest of the system
  (define (tag p) (attach-tag 'polynomial p))
  (append
    dispatch-table
    (list
      (list 'add '(polynomial polynomial)
            (lambda (p1 p2) (tag (add-poly p1 p2))))
      (list 'mul '(polynomial polynomial)
            (lambda (p1 p2) (tag (mul-poly p1 p2))))
      (list 'sub '(polynomial polynomial)
            (lambda (p1 p2) (tag (sub-poly p1 p2))))
      (list 'make 'polynomial
            (lambda (var terms) (tag (make-poly var terms)))))))

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

(define (make-polynomial var terms)
  ((get dispatch-table 'make 'polynomial) var terms))
(define (negate x) (apply-generic dispatch-table 'negate x))
(define (add x y) (apply-generic dispatch-table 'add x y))
(define (sub x y) (apply-generic dispatch-table 'sub x y))

(define dispatch-table
  (install-polynomial-package
    (install-scheme-number-package
      (install-=zero?-package
        (list
          (list 'negate '(scheme-number) (lambda (n) (- n))))))))

(define p0 (make-polynomial 'x (list (list 2 5) (list 1 3) (list 0 7))))
(define p1 (make-polynomial 'x (list (list 2 1) (list 0 4))))

(display p0)
(newline)
(display p1)
(newline)
(display (add p0 p1))
(newline)
(display (sub p0 p1))
(newline)
