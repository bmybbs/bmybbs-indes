add_library(bmybbs-java SHARED
	board.c
	jni_utils.c
)

target_link_libraries(bmybbs-java ythtbbs)

