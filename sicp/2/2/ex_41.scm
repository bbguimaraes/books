(define nil '())

(define (flatmap proc seq)
  (fold-right append nil (map proc seq)))

(define (enumerate-interval low high)
  (if (> low high)
    nil
    (cons low (enumerate-interval (+ low 1) high))))

(define (unique-triplets n)
  (flatmap (lambda (i)
             (flatmap (lambda (j)
                        (map (lambda (k) (list i j k))
                             (enumerate-interval 1 (- j 1))))
                      (enumerate-interval 1 (- i 1))))
           (enumerate-interval 1 n)))

(display (unique-triplets 6))

(define (s-sum-triplets n s)
  (filter (lambda (triplet) (= (fold-right + 0 triplet) s))
          (unique-triplets n)))

(newline)
(display (s-sum-triplets 6 10))

(newline)
