(define (smallest-divisor n)
  (define (find-divisor n test-divisor)
    (define (divides? a b)
      (= (remainder b a) 0))
    (cond ((> (square test-divisor) n) n)
          ((divides? test-divisor n) test-divisor)
          (else (find-divisor n (next test-divisor)))))
  (find-divisor n 2))

(define (next x)
  (if (= x 2) 3 (+ x 2)))

(define (prime? n)
  (= n (smallest-divisor n)))

(define (timed-prime-test n)
  (newline)
  (display n)
  (start-prime-test n (runtime)))

(define (start-prime-test n start-time)
  (if (prime? n)
    (report-prime (- (runtime) start-time))))

(define (report-prime elapsed-time)
  (display " *** ")
  (display elapsed-time))

(define (search-for-primes start end)
  (define (search-for-primes-iter i)
    (cond ((<= i end)
      (timed-prime-test i)
      (search-for-primes-iter (+ i 1)))))
  (search-for-primes-iter start))

(search-for-primes 3 1000)
