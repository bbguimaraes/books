(display (cons (cons 1 2)
               (cons 3 4)))
(newline)
(display (cons (cons 1
               (cons 2 3))
               4))
(newline)

(display (cons 1
               (cons 2
                     (cons 3
                           (cons 4 'nil)))))
(newline)

(define one-through-four (list 1 2 3 4))
(display one-through-four)
(newline)

(display (car one-through-four))
(newline)
(display (cdr one-through-four))
(newline)
(display (car (cdr one-through-four)))
(newline)
(display (cadr one-through-four))
(newline)
(display (cons 10 one-through-four))
(newline)
(display (cons 5 one-through-four))
(newline)

(define (list-ref items n)
  (if (= n 0)
    (car items)
    (list-ref (cdr items) (- n 1))))
(define squares (list 1 4 9 16 25))
(display (list-ref squares 3))
(newline)

(define (length items)
  (if (null? items)
    0
    (+ 1 (length (cdr items)))))
(define odds (list 1 3 5 7))
(display (length odds))
(newline)

(define (length items)
  (define (iter a count)
    (if (null? a)
      count
      (iter (cdr a) (+ count 1))))
  (iter items 0))
(display (length odds))
(newline)

(define (append list1 list2)
  (if (null? list1)
    list2
    (cons (car list1) (append (cdr list1) list2))))
(display (append squares odds))
(newline)
(display (append odds squares))

(define (scale-list items factor)
  (if (null? items)
    items
    (cons (* (car items) factor)
          (scale-list (cdr items) factor))))
(newline)
(newline)
(display (scale-list (list 1 2 3 4 5) 10))
(newline)

(define (map proc items)
  (if (null? items)
    items
    (cons (proc (car items))
          (map proc (cdr items)))))
(display (map abs (list -10 2.5 -11.6 17)))
(newline)
(display (map (lambda (x) (* x x))
              (list 1 2 3 4)))
(newline)

(define (scale-list items factor)
  (map (lambda (x) (* x factor))
       items))
(display (scale-list (list 1 2 3 4 5) 10))
(newline)
