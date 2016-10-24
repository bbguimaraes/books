(define a 1)
(define b 2)

(display (list a b))
(newline)
(display (list 'a 'b))
(newline)
(display (list 'a b))

(newline)
(display (car '(a b c)))
(newline)
(display (cdr '(a b c)))

(define (memq item x)
  (cond ((null? x) false)
        ((eq? item (car x)) x)
        (else (memq item (cdr x)))))

(newline)
(display (memq 'apple '(pear banana prune)))
(newline)
(display (memq 'apple '(x (apple sauce) y apple pear)))
(newline)
