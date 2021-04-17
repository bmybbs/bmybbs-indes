# 复制当前文件为 config.cmake 并依据本地环境修改路径

set(JAVA_INCLUDE
	/path/to/$JAVA_HOME/include            # JDK 解压缩后有个 include 目录，提供 jni.h
	/path/to/$JAVA_HOME/include/$OS_type   # 依据操作系统，例如 linux，提供 jni_md.h，被 jni.h 依赖
)

# 以下是 bmybbs 需要的头文件和库

set(BMYBBS_INCLUDE
	/path/to/bmybbs/include
)

set(BMYBBS_LIBRARIES
	/path/to/bmybbs/libytht
	/path/to/bmybbs/libbmy
	/path/to/bmybbs/libythtbbs
)

