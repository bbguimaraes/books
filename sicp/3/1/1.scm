(define balance 100)

(define (withdraw amount)
  (if (>= balance amount)
    (begin (set! balance (- balance amount))
           balance)
    "Insufficient funds"))

(display (withdraw 25))
(newline)
(display (withdraw 25))
(newline)
(display (withdraw 60))
(newline)
(display (withdraw 15))
(newline)

(define new-withdraw
  (let ((balance 100))
    (lambda (amount)
      (if (>= balance amount)
        (begin (set! balance (- balance amount))
               balance)
        "Insufficient funds"))))

(newline)
(display (new-withdraw 25))
(newline)
(display (new-withdraw 25))
(newline)
(display (new-withdraw 60))
(newline)
(display (new-withdraw 15))
(newline)

(define (make-withdraw balance)
  (lambda (amount)
    (if (>= balance amount)
      (begin (set! balance (- balance amount))
             balance)
      "Insufficient funds")))

(define W1 (make-withdraw 100))
(define W2 (make-withdraw 100))
(newline)
(display (W1 50))
(newline)
(display (W2 70))
(newline)
(display (W2 40))
(newline)
(display (W1 40))
(newline)

(define (make-account balance)
  (define (withdraw amount)
    (if (>= balance amount)
      (begin (set! balance (- balance amount))
             balance)
      "Insufficient funds"))
  (define (deposit amount)
    (set! balance (+ balance amount))
    balance)
  (define (dispatch m)
    (cond ((eq? m 'withdraw) withdraw)
          ((eq? m 'deposit) deposit)
          (else (error "Unknown request -- MAKE-ACCOUNT"
                       m))))
  dispatch)

(define acc (make-account 100))
(newline)
(display ((acc 'withdraw) 50))
(newline)
(display ((acc 'withdraw) 60))
(newline)
(display ((acc 'deposit) 40))
(newline)
(display ((acc 'withdraw) 60))
(newline)

(define acc2 (make-account 100))
(newline)
(display ((acc2 'withdraw) 50))
(newline)
