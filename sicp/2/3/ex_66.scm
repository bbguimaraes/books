(define (entry tree) (car tree))
(define (left-branch tree) (cadr tree))
(define (right-branch tree) (caddr tree))
(define (make-tree entry left right)
  (list entry left right))

(define (key x) (car x))
(define (value x) (cadr x))

(define (lookup given-key set-of-records)
  (if (null? set-of-records)
    false
    (let ((x (entry set-of-records)))
      (cond ((equal? given-key (key x)) x)
            ((< given-key (key x))
             (lookup given-key (left-branch set-of-records)))
            ((> given-key (key x))
             (lookup given-key (right-branch set-of-records)))))))

(define s
  (make-tree
    '(4 "d")
    (make-tree
      '(2 "b")
      (make-tree '(1 "a") '() '())
      (make-tree '(3 "c") '() '()))
    (make-tree
      '(6 "f")
      (make-tree '(5 "e") '() '())
      (make-tree '(7 "g") '() '()))))
(display (value (lookup 6 s)))
(newline)
