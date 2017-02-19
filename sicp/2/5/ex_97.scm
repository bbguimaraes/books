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
(define (mul x y) (apply-generic dispatch-table 'mul x y))
(define (div x y) (apply-generic dispatch-table 'div x y))

(define (install-scheme-number-package dispatch-table)
(define (gcd a b)
  (if (= b 0) a (gcd b (remainder a b))))
(define (tag x) (attach-tag 'scheme-number x))
  (append
    dispatch-table
    (list
      (list 'add '(scheme-number scheme-number) (lambda (x y) (tag (+ x y))))
      (list 'sub '(scheme-number scheme-number) (lambda (x y) (tag (- x y))))
      (list 'mul '(scheme-number scheme-number) (lambda (x y) (tag (* x y))))
      (list 'div '(scheme-number scheme-number) (lambda (x y) (tag (/ x y))))
      (list 'gcd '(scheme-number scheme-number) (lambda (x y) (tag (gcd x y))))
      (list 'make 'scheme-number (lambda (x) (tag x))))))

(define (install-rational-package dispatch-table)
  ;; internal procedures
  (define (numer x) (car x))
  (define (denom x) (cdr x))
  (define (make-rat n d) (reduce n d))
  (define (add-rat x y)
    (make-rat (add (mul (numer x) (denom y))
                   (mul (numer y) (denom x)))
              (mul (denom x) (denom y))))
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
  (define (mul-term-by-all-terms t1 L)
    (if (empty-termlist? L)
      (the-empty-termlist)
      (let ((t2 (first-term L)))
        (adjoin-term
          (make-term (+ (order t1) (order t2))
                     (mul (coeff t1) (coeff t2)))
          (mul-term-by-all-terms t1 (rest-terms L))))))
  (define (mul-poly p1 p2)
    (define (mul-terms L1 L2)
      (if (empty-termlist? L1)
        (the-empty-termlist)
        (add-terms (mul-term-by-all-terms (first-term L1) L2)
                   (mul-terms (rest-terms L1) L2))))
    (if (same-variable? (variable p1) (variable p2))
      (make-poly (variable p1)
                 (mul-terms (term-list p1)
                            (term-list p2)))
      (error "Polys not in same var -- MUL-POLY"
             (list p1 p2))))
  (define (div-terms L1 L2)
    (if (empty-termlist? L1)
      (list (the-empty-termlist) (the-empty-termlist))
      (let ((t1 (first-term L1))
            (t2 (first-term L2)))
        (if (> (order t2) (order t1))
          (list (the-empty-termlist) L1)
          (let ((new-c (div (coeff t1) (coeff t2)))
                (new-o (- (order t1) (order t2))))
            (let ((rest-of-result
                    (div-terms
                      (sub-terms
                        L1
                        (mul-term-by-all-terms (make-term new-o new-c) L2))
                      L2)))
              (list
                (add-terms
                  (list (make-term new-o new-c))
                  (car rest-of-result))
                (cadr rest-of-result))))))))
  (define (sub-terms L1 L2)
    (add-terms
      L1
      (map (lambda (term) (make-term (order term) (negate (coeff term))))
           L2)))
  (define (div-poly p1 p2)
    (if (same-variable? (variable p1) (variable p2))
      (let ((res (div-terms (term-list p1)
                            (term-list p2))))
        (list
          (make-poly (variable p1) (car res))
          (make-poly (variable p1) (cadr res))))
      (error "Polys not in same var -- DIV-POLY"
             (list p1 p2))))
  (define (scale-terms n L)
    (map (lambda (t) (make-term (order t) (* n (coeff t)))) L))
  (define (integerizing-factor L1 L2)
    (expt (coeff (first-term L2))
          (+ 1 (order (first-term L1)) (- (order (first-term L2))))))
  (define (gcd-terms a b)
    (if (empty-termlist? b)
      (scale-terms
        (/ 1 (reduce-left greatest-common-divisor '() (map coeff a))) a)
      (gcd-terms b (pseudoremainder-terms a b))))
  (define (pseudoremainder-terms L1 L2)
    (let ((L1 (scale-terms (integerizing-factor L1 L2) L1)))
      (cadr (div-terms L1 L2))))
  (define (gcd-poly p1 p2)
    (if (same-variable? (variable p1) (variable p2))
      (make-poly (variable p1) (gcd-terms (term-list p1) (term-list p2)))
      (error "Polys not in same var -- GCD-POLY"
             (list p1 p2))))
  (define (reduce-terms n d)
    (let ((nd-gcd (gcd-terms n d))
          (higher (if (> (order (first-term n)) (order (first-term d))) n d)))
      (let ((factor (integerizing-factor higher nd-gcd)))
        (cons (car (div-terms (scale-terms factor n) nd-gcd))
              (car (div-terms (scale-terms factor d) nd-gcd))))))
  (define (reduce-poly p1 p2)
    (if (same-variable? (variable p1) (variable p2))
      (let ((nd (reduce-terms (term-list p1) (term-list p2))))
        (cons (make-poly (variable p1) (car nd))
              (make-poly (variable p1) (cdr nd))))
      (error "Polys not in same var -- REDUCE-POLY"
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
      (list 'div '(polynomial polynomial)
            (lambda (p1 p2)
              (map (lambda (x) (attach-tag 'polynomial x))
                   (div-poly p1 p2))))
      (list 'gcd '(polynomial polynomial)
            (lambda (p1 p2) (tag (gcd-poly p1 p2))))
      (list 'reduce '(polynomial polynomial)
            (lambda (p1 p2)
              (let ((nd (reduce-poly p1 p2)))
                (cons (tag (car nd)) (tag (cdr nd))))))
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

(define (make-rational n d) ((get dispatch-table 'make 'rational) n d))
(define (make-polynomial var terms)
  ((get dispatch-table 'make 'polynomial) var terms))
(define (negate x) (apply-generic dispatch-table 'negate x))

(define dispatch-table
  (install-polynomial-package
    (install-rational-package
      (install-scheme-number-package
        (install-=zero?-package
          (list
            (list 'negate '(scheme-number) (lambda (n) (- n)))))))))

(define (greatest-common-divisor x y) (apply-generic dispatch-table 'gcd x y))
(define (reduce n d) (apply-generic dispatch-table 'reduce n d))

(define p1 (make-polynomial 'x '((1 1)(0 1))))
(define p2 (make-polynomial 'x '((3 1)(0 -1))))
(define p3 (make-polynomial 'x '((1 1))))
(define p4 (make-polynomial 'x '((2 1)(0 -1))))

(define rf1 (make-rational p1 p2))
(define rf2 (make-rational p3 p4))

(display rf1)
(newline)
(display rf2)
(newline)
(display (add rf1 rf2))
(newline)
