; 问题1：将输入的01串循环右移，保持起始位置不偏移
; Input: a string of 0's and 1's, e.g. '1001001'

; the finite set of states
#Q = {0,s1,s2,s3,s4,s5,s6,halt}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt}

; the number of tapes
#N = 2

; the transition functions
;："<旧状态> <旧符号组> <新符号组> <方向组> <新状态>

; State 0: start state
0 0_ 0_ ** s1
0 1_ 1_ ** s1
0 __ __ ** halt ;输入为空

; State s1: copy the string to the 2nd tape 
s1 0_ 00 rr s1
s1 1_ 11 rr s1
s1 __ __ ll s2

; State s2: move 1st head to the left
s2 ** ** l* s2
s2 _* _* r* s3


; State s3: copy the last char on 2nd tape,to the head of 1st tape
s3 *0 0_ rl s4
s3 *1 1_ rl s4

; State s4: move the head2 to the head of 2nd tape
s4 ** ** *l s4
s4 __ __ lr halt ;只有一个输入字符
s4 *_ *_ *r s5

; State s5: copy the string to the 1st tape
s5 *0 00 rr s5
s5 *1 11 rr s5
s5 __ __ ll s6

; State s6: clean the 2nd type
s6 ** *_ *l s6
s6 *_ *_ *r halt


