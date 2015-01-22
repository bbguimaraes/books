;(define right-split (split beside below))
;(define up-split (split below beside))
(define (split first second)
  (if (= n 0)
    painter
    (let ((smaller (split painter (- n 1))))
      (first painter (second smaller smaller)))))
