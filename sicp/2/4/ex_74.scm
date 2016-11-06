(define (attach-tag type-tag contents)
  (cons type-tag contents))
(define (type-tag datum)
  (if (pair? datum)
    (car datum)
    (error "Bad tagged datum -- TYPE-TAG" datum)))
(define (contents datum)
  (if (pair? datum)
    (cdr datum)
    (error "Bad tagged datum -- CONTENTS" datum)))
(define (get dispatch-table op type-tags)
  (if (null? dispatch-table)
    #f
    (let ((x (car dispatch-table)))
      (if (and
            (eq? (car x) op)
            (eq? (cadr x) type-tags))
        (caddr x)
        (get (cdr dispatch-table) op type-tags)))))

(define (get-record file division name)
  (if (null? file)
    #f
    (let ((x (car file)))
      (if (equal? name (car x))
        (attach-tag division x)
        (get-record (cdr file) division name)))))

; name age occupation salary
(define file-a
  (list
    (list "Bob" 25 "programmer" 20)))

; name occupation_code 100-salary birth_timestamp
(define file-b
  (list
    (list "Jennifer" 5 60 94668401)
    (list "Billy" 1 78 946684800)))

(display (get-record file-a 'division-a "Billy"))
(newline)
(display (get-record file-a 'division-a "Bob"))
(newline)
(display (get-record file-b 'division-b "Billy"))
(newline)
(display (get-record file-b 'division-b "Bob"))
(newline)

(define (get-salary dispatch-table record)
  ((get dispatch-table 'get-salary (type-tag record)) (contents record)))

(define (install-insatiable-package dispatch-table)
  (define (get-salary-division-a record) (cadddr record))
  (define (get-salary-division-b record) (- 100 (caddr record)))
  (append
    dispatch-table
    (list
      (list 'get-salary 'division-a get-salary-division-a)
      (list 'get-salary 'division-b get-salary-division-b))))

(define dispatch-table (install-insatiable-package '()))

(newline)
(display (get-salary dispatch-table (get-record file-a 'division-a "Bob")))
(newline)
(display (get-salary dispatch-table (get-record file-b 'division-b "Billy")))
(newline)

(define (find-employee-record files name)
  (if (null? files)
    #f
    (let ((division (caar files))
          (records (cadar files)))
      (let ((record (get-record records division name)))
        (if record
          record
          (find-employee-record (cdr files) name))))))

(define files
  (list
    (list 'division-a file-a)
    (list 'division-b file-b)))

(newline)
(display (get-salary dispatch-table (find-employee-record files "Bob")))
(newline)
(display (get-salary dispatch-table (find-employee-record files "Billy")))
(newline))
