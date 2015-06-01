# myGrep

>> A small grep like C program.

>> To run:   ./executable regularExp inputString

eg:
1)  ./a.out (abc) abc                 =====> match!
2)  ./a.out (abc)(de)+ abcde          =====> match!
3)  ./a.out (abc)(de)+ abcdedede      =====> match!
4)  ./a.out (abc)(de)+ abcdededea      =====> no match!

>> Prints 'match!' if both string are same/similar else prints 'no match!'.

>> Each operator with minimum value and maximum value of occurence is stored in a config file.
+ 1 ∞
* 0 ∞
? 0 1

>> Since ∞ is practically impossible, value of INT_MAX is used.
