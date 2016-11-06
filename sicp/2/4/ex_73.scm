(define (variable? e) (symbol? e))
(define (same-variable? v1 v2)
  (and (variable? v1) (variable? v2) (eq? v1 v2)))
(define (addend e) (cadr e))
(define (augend e) (caddr e))
(define (make-sum a1 a2) (list '+ a1 a2))
(define (multiplier e) (cadr e))
(define (multiplicand e) (caddr e))
(define (make-product m1 m2) (list '* m1 m2))
(define (base e) (cadr e))
(define (exponent e) (caddr e))
(define (make-exponentiation b e) (list '** b e))

(define (get dispatch-table op type-tags)
  (if (null? dispatch-table)
    #f
    (let ((x (car dispatch-table)))
      (if (and
            (eq? (car x) op)
            (eq? (cadr x) type-tags))
        (caddr x)
        (get (cdr dispatch-table) op type-tags)))))

(define (deriv-sum dispatch-table exp var)
  (make-sum (deriv dispatch-table (addend exp) var)
            (deriv dispatch-table (augend exp) var)))
(define (deriv-product dispatch-table exp var)
  (make-sum
    (make-product (multiplier exp)
                  (deriv dispatch-table (multiplicand exp) var))
    (make-product (deriv dispatch-table (multiplier exp) var)
                  (multiplicand exp))))
(define (deriv-exponentiation dispatch-table exp var)
  (make-product
    (make-product
      (exponent exp)
      (make-exponentiation
        (base exp)
        (make-sum (exponent exp) '-1)))
    (deriv dispatch-table (base exp) var)))

(define (install-deriv-package dispatch-table)
  (append
    dispatch-table
    (list
      (list 'deriv '+ deriv-sum)
      (list 'deriv '* deriv-product)
      (list 'deriv '** deriv-exponentiation))))

(define (deriv dispatch-table exp var)
  (cond
    ((number? exp) 0)
    ((variable? exp) (if (same-variable? exp var) 1 0))
    (else
      (let ((proc (get dispatch-table 'deriv (operator exp))))
        (if proc
          (proc dispatch-table exp var)
          (error "No method for these types -- DERIV" exp var))))))

(define (operator exp) (car exp))
(define (operands exp) (cdr exp))

(define (t dispatch-table)
  (define (deriv2 exp args) (deriv dispatch-table exp args))
  (display (deriv2 '(+ x 3) 'x))
  (newline)
  (display (deriv2 '(* x y) 'x))
  (newline)
  (display (deriv2 '(* (* x y) (+ x 3)) 'x))
  (newline)
  (display (deriv2 '(** (+ x 2) 3) 'x))
  (newline))

(define dispatch-table (install-deriv-package '()))
(t dispatch-table)

(define (make-sum a1 a2)
  (cond ((=number? a1 0) a2)
        ((=number? a1 0) a1)
        ((and (number? a1) (number? a1)) (+ a1 a2))
        (else (list '+ a1 a2))))

(define (make-product m1 m2)
  (cond ((or (=number? m1 0) (=number? m2 0)) 0)
        ((=number? m1 1) m2)
        ((=number? m2 1) m1)
        ((and (number? m1) (number? m2)) (* m1 m2))
        (else (list '* m1 m2))))

(define (make-exponentiation b e)
  (cond ((=number? e 0) 1)
        ((and (number? b) (number? e)) (expt b e))
        (else (list '** b e))))

(define (=number? exp num)
  (and (number? exp) (= exp num)))

(newline)
(t dispatch-table)

(define (deriv dispatch-table exp var)
  (cond
    ((number? exp) 0)
    ((variable? exp) (if (same-variable? exp var) 1 0))
    (else
      (let ((proc (get dispatch-table (operator exp) 'deriv)))
        (if proc
          (proc dispatch-table exp var)
          (error "No method for these types -- DERIV" exp var))))))

(define (install-deriv-package dispatch-table)
  (append
    dispatch-table
    (list
      (list '+ 'deriv deriv-sum)
      (list '* 'deriv deriv-product)
      (list '** 'deriv deriv-exponentiation))))

(newline)
(t (install-deriv-package '()))
