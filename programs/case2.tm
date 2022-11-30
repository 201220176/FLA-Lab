; 问题1：判断输入长度是否为N^2
; Input: a string of  1's, e.g. '1111111'

; the finite set of states
#Q = {0,s1,s2,s3,s4,s5,s6,true,true_0,true_1,true_2,true_3,false,false_0,false_1,false_2,false_3,false_4,halt}

; the finite set of input symbols
#S = {1}

; the complete set of tape symbols
#G = {1,_,t,r,u,e,f,l,a,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt}

; the number of tapes
#N = 3

; the transition functions
;："<旧状态> <旧符号组> <新符号组> <方向组> <新状态>

; State 0: start state
0 ___ ___ l** true ;输入为空
0 1__ 1__ *** s1

; State s1: tape1 和 tape2 的长度增加1 
s1 1__ 111 *** s2

; State s2: 将head0,head1 和 head2左移至头部 
s2 111 111 lll s2
s2 _11 _11 *ll s2
s2 1__ 1__ l** s2
s2 ___ ___ rrr s3

; State s3: 右移head2和head0,即逐位乘法
s3 111 111 r*r s3 
s3 11_ 11_ *r* s4 ;tape2消耗完成,tape0还有剩余,右移head1,进行下一位
s3 _1_ _1_ *r* s5 ;tape2和tape0同时消耗完全,检查tape1是否有剩余
s3 _11 _11 l** false ;tape0消耗完,tape2还有剩余,false

; State s4: 
s4 11_ 11_ **l s6 ;tape1有剩余,则恢复head2
s4 1__ 1__ *** s1 ;tape1无剩余,增加长度

; State s5: 
s5 ___ ___ l** true ;全部同时消耗完,接收
s5 _1_ _1_ l** false ;tape1有剩余,拒绝

; State s6: 
s6 111 111 **l s6
s6 11_ 11_ **r s3 

;State true:进行打印结果步骤,先清空纸带
true 1__ ___ l** true
true ___ ___ r** true_0

;State true_0:进行打印结果
true_0 ___ t__ r** true_1

;State true_1:进行打印结果
true_1 ___ r__ r** true_2

;State true_2:进行打印结果
true_2 ___ u__ r** true_3

;State true_3:进行打印结果
true_3 ___ e__ *** halt

;State false:进行打印结果步骤,先清空纸带
false 1__ ___ l** false
false 111 ___ l** false
false 11_ ___ l** false
false ___ ___ r** false_0

;State false_0:进行打印结果
false_0 ___ f__ r** false_1

;State false_1:进行打印结果
false_1 ___ a__ r** false_2

;State false_2:进行打印结果
false_2 ___ l__ r** false_3

;State false_3:进行打印结果
false_3 ___ s__ r** false_4

;State false_4:进行打印结果
false_4 ___ e__ *** halt

