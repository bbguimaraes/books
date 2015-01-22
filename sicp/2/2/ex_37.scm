(define m (list (list 1 2 3 4) (list 4 5 6 6) (list 6 7 8 9)))
(display m)

(define nil (cdr (list 0)))

(define (accumulate op initial sequence)
  (if (null? sequence)
    initial
    (op (car sequence)
        (accumulate op initial (cdr sequence)))))

(define (accumulate-n op init seqs)
  (if (null? (car seqs))
    nil
    (cons (accumulate op init (map car seqs))
          (accumulate-n op init (map cdr seqs)))))

(define (dot-product v w)
  (accumulate + 0 (map * v w)))

(define (matrix-*-vector m v)
  (map (lambda (row) (dot-product row v)) m))

(define (transpose m)
  (accumulate-n cons nil m))

(define (matrix-*-matrix m n)
  (let ((cols (transpose n)))
    (map (lambda (row) (matrix-*-vector cols row))
         m)))

(newline)
(display (dot-product (car m) (cadr m)))
(newline)
(display (matrix-*-vector m (list 2 3 4 5)))
(newline)
(display (transpose m))
(newline)
(display (matrix-*-matrix (transpose m) m))
(newline)
