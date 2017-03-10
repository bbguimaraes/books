(define (make-simplified-withdraw balance)
  (lambda (amount)
    (set! balance (- balance amount))
    balance))

(define W (make-simplified-withdraw 25))
(map display (list (W 20) " " (W 10) "\n"))

(define (make-decrementer balance)
  (lambda (amount)
    (- balance amount)))

(define D (make-decrementer 25))
(map display (list (D 20) " " (D 10) "\n"))

(define D1 (make-decrementer 25))
(define D2 (make-decrementer 25))
(define W1 (make-simplified-withdraw 25))
(define W2 (make-simplified-withdraw 25))

(map display (list (D1 20) " " (D1 20) " " (D2 20) "\n"))
(map display (list (W1 20) " " (W1 20) " " (W2 20) "\n"))

(define (factorial n)
  (let ((product 1)
        (counter 1))
    (define (iter)
      (if (> counter n)
        product
        (begin (set! product (* counter product))
               (set! counter (+ counter 1))
               (iter))))
    (iter)))

(display (factorial 5))
(newline)
