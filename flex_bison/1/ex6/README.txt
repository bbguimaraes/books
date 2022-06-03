$ perl -ne 'for $i (1..128) { print }' /proc/kallsyms | shuf > /tmp/in.txt
$ du -h /tmp/in.txt
1.2G    /tmp/in.txt
$ time ex6/wc < /tmp/in.txt
27855360 91905536 1236452736

real    0m0.361s
user    0m0.187s
sys     0m0.173s
$ time ex6/wc_no_simd < /tmp/in.txt
27855360 91905536 1236452736

real    0m1.484s
user    0m1.314s
sys     0m0.166s
$ time wc/wc < /tmp/in.txt
27855360 91905536 1236452736

real    0m6.077s
user    0m5.831s
sys     0m0.233s
$ time wc < /tmp/in.txt
  27855360   91905536 1236452736

real    0m8.556s
user    0m8.363s
sys     0m0.187s
$ time cat < /tmp/in.txt > /dev/null

real    0m0.167s
user    0m0.000s
sys     0m0.166s
