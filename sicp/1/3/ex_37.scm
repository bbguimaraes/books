(define (phi k)
  (cont-frac (lambda (i) 1.0)
             (lambda (i) 1.0)
             k))

(define (cont-frac n d k)
  (define (rec i)
    (if (> i k)
      0
      (/ (n i) (+ (d i) (rec (+ i 1))))))
  (rec 1))

(display (/ 1 (phi 100)))
(newline)

(define (cont-frac n d k)
  (define (iter i result)
    (if (= i 0)
      result
      (iter (- i 1) (/ (n i) (+ (d i) result)))))
  (iter k 0))

(display (/ 1 (phi 100)))
(newline)
