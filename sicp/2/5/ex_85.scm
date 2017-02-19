(define (apply-generic dispatch-table op . args)
  (let ((type-tags (map type-tag args)))
    (let ((proc (get dispatch-table op type-tags)))
      (if proc
        (let ((result (apply proc (map contents args))))
          (if (eq? op 'add)
            (drop result)
            result))
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

(define (type-tag datum)
  (if (pair? datum)
    (car datum)
    (error "Bad tagged datum -- TYPE-TAG" datum)))
(define (contents datum)
  (if (pair? datum)
    (cdr datum)
    (error "Bad tagged datum -- CONTENTS" datum)))

(define (drop x)
  (let ((projected (project x)))
    (if (and projected (equ? x (raise projected)))
      (drop projected)
      x)))

(define (project x) (apply-generic dispatch-table 'project x))
(define (raise x) (apply-generic dispatch-table 'raise x))
(define (equ? x y) (apply-generic dispatch-table 'equ? x y))

(define (project-complex x) (make-real (car x)))
(define (project-real x) (make-integer (round x)))
(define (raise-real x) (make-complex x 0))
(define (raise-integer x) (make-real x))
(define (equ?-complex x y)
  (and
    (= (car x) (car y))
    (= (cdr x) (cdr y))))
(define (equ?-real x y) (= x y))

(define dispatch-table
  (list
    (list 'project '(complex) project-complex)
    (list 'project '(real) project-real)
    (list 'project '(integer) (lambda (x) #f))
    (list 'raise '(real) raise-real)
    (list 'raise '(integer) raise-integer)
    (list 'equ? '(complex complex) equ?-complex)
    (list 'equ? '(real real) equ?-real)))

(define (make-complex r i) (cons 'complex (cons r i)))
(define (make-real n) (cons 'real n))
(define (make-integer n) (cons 'integer n))

(display (drop (make-complex 1.5 0)))
(newline)
(display (drop (make-complex 1 0)))
(newline)
(display (drop (make-complex 2 3)))
(newline)

(define (add-complex x y)
  (make-complex (+ (car x) (car y))
                (+ (cdr x) (cdr y))))

(define dispatch-table
  (append
    dispatch-table
    (list
      (list 'add '(complex complex) add-complex))))

(define (add . args)
  (apply apply-generic (append (list dispatch-table 'add) args)))

(display (add (make-complex 2.5 -1) (make-complex -1 1)))
(newline)
(display (add (make-complex 3 5) (make-complex -2 -5)))
(newline)
(display (add (make-complex 3 4) (make-complex -1 -1)))
(newline)
