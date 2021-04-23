add_library(bmybbs-java SHARED
	article.c
	board.c
	jni_utils.c
)

target_link_libraries(bmybbs-java ythtbbs)

