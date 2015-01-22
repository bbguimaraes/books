(define nil '())
(define empty-board '())

(define (flatmap proc seq)
  (fold-right append nil (map proc seq)))

(define (enumerate-interval low high)
  (if (> low high)
    nil
    (cons low (enumerate-interval (+ low 1) high))))

(define (adjoin-position row col rest)
  (cons (list row col) rest))

(define (safe? k positions)
  (define (same-row p1 p2) (= (car p1) (car p2)))
  (define (same-diag p1 p2)
    (= (abs (- (car p2) (car p1)))
       (abs (- (cadr p2) (cadr p1)))))
  (let ((new (car positions))
        (positions (cdr positions)))
    (or (null? positions)
        (and (null? (filter (lambda (p) (same-row new p)) positions))
             (null? (filter (lambda (p) (same-diag new p)) positions))))))

(define (queens board-size)
  (define (queens-cols k)
    (if (= k 0)
      (list empty-board)
      (filter
        (lambda (positions) (safe? k positions))
        (flatmap
          (lambda (rest-of-queens)
            (map (lambda (new-row)
                   (adjoin-position new-row k rest-of-queens))
                 (enumerate-interval 1 board-size)))
          (queens-cols (- k 1))))))
  (queens-cols board-size))

(for-each (lambda (row) (display row)(newline)) (queens 8))
