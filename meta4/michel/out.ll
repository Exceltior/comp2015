@_false = global i1 0
@_true = global i1 1
@_k = global i1 0
@_m = global double 0.000000e+00
@argc_ = global i32 0
@argv_ = global i8** null
define i32 @main(i32 %argc, i8** %argv) {
%1 = alloca i32
%2 = alloca i8**
store i32 %argc, i32* %1
store i8** %argv, i8*** %2
%3 = load i32* %1
store i32 %3, i32* @argc_
%4 = load i8*** %2
store i8** %4, i8*** @argv_

%5 = add i32 28, 0
%6 = add i32 6, 0
%7 = srem i32 %5, %6
%8 = call i32 @modi(i32 %7, i32 %6)
%9 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str_3, i32 0, i32 0), i32 %8)
%10 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([2 x i8]* @.str_0, i32 0, i32 0))
ret i32 0
}
@.str_0 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.str_1 = private unnamed_addr constant [2 x i8] c" \00"
@.str_2 = private unnamed_addr constant [6 x i8] c"%.12E\00"
@.str_3 = private unnamed_addr constant [3 x i8] c"%d\00"
@.str_4 = private unnamed_addr constant [3 x i8] c"%s\00"
@.str_5 = private unnamed_addr constant [5 x i8] c"TRUE\00"
@.str_6 = private unnamed_addr constant [6 x i8] c"FALSE\00"

declare i32 @atoi(i8*)
declare i32 @printf(i8*, ...)
define i32 @valparam(i32 %pos){
%1 = alloca i32
store i32 %pos, i32* %1
%2 = load i32* %1
%3 = sext i32 %2 to i64
%4 = load i8*** @argv_
%5 = getelementptr inbounds i8** %4, i64 %3
%6 = load i8** %5
%7 = call i32 @atoi(i8* %6)
ret i32 %7
}
define void @print_boolean(i1 %_b){
br i1 %_b, label %if_bool, label %else_bool
if_bool:
 call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([5 x i8]* @.str_5, i32 0, i32 0))
 br label %end_bool
else_bool:
 call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str_6, i32 0, i32 0))
 br label %end_bool
end_bool: ret void
}
define i32 @paramcount(){
 %1 = load i32* @argc_
 %2 = sub i32 %1, 1
ret i32 %2
}
define i32 @abs(i32 %a){
%1 = icmp slt i32 %a, 0
br i1 %1, label %la, label %lb
la:
%2 = sub i32 0, %a
ret i32 %2
lb:
ret i32 %a
}define i32 @modi(i32 %a, i32 %c){
	%1 = icmp slt i32 %a, 0
	br i1 %1, label %la, label %lb
	la:
 %2 = add i32 %a, %c
 ret i32 %2
lb: 
 ret i32 %a
}