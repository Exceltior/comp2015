declare i32 @puts(i8* nocapture) nounwind

define i32 @main() {
	%tmp_str0 = getelementptr [4 x i8]* @.global_str0, i64 0, i64 0
	call i32 @puts(i8* %tmp_str0)
	%tmp_str1 = getelementptr [5 x i8]* @.global_str1, i64 0, i64 0
	call i32 @puts(i8* %tmp_str1)
	ret i32 0
}

@.global_str0 = private unnamed_addr constant [4 x i8] c"ola\00"
@.global_str1 = private unnamed_addr constant [5 x i8] c"ola2\00"

