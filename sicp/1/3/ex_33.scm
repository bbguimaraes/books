(define (filtered-accumulate filter combiner null-value term a next b)
  (if (> a b)
    null-value
    (if (filter a)
      (combiner (term a)
                (filtered-accumulate
                  filter combiner null-value term (next a) next b))
      (filtered-accumulate filter combiner null-value term (next a) next b))))

(define (identity x) x)
(define (inc x) (+ x 1))

(define (prime? n)
 (define (smallest-divisor n)
   (define (find-divisor n test-divisor)
     (define (divides? a b)
       (= (remainder b a) 0))
     (cond ((> (square test-divisor) n) n)
           ((divides? test-divisor n) test-divisor)
           (else (find-divisor n (+ test-divisor 1)))))
   (find-divisor n 2))
  (= n (smallest-divisor n)))

(define (sum-squares-primes a b)
  (filtered-accumulate prime? + 0 square a inc b))

(display (sum-squares-primes 2 10))
(newline)

(define (product-relative-primes n)
  (define (relative-prime? x) (= (gcd n x) 1))
  (filtered-accumulate relative-prime? * 1 identity 1 inc n))

(display (product-relative-primes 10))
(newline)

(define (filtered-accumulate filter combiner null-value term a next b)
  (define (iter a result)
    (cond ((> a b) result)
          ((filter a) (iter (next a) (combiner (term a) result)))
          (else (iter (next a) result))))
  (iter a null-value))

(newline)
(display (sum-squares-primes 2 10))
(newline)
(display (product-relative-primes 10))
(newline)
