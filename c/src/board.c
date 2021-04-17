#include <string.h>
#include "jni_utils.h"
#include "ythtbbs/cache.h"
#include "ythtbbs/board.h"

static int getAllBoards_callback(struct boardmem *board, int curr_idx, va_list ap) {
	(void) curr_idx;

	JNIEnv *env = va_arg(ap, JNIEnv *);
	jobject list_board = va_arg(ap, jobject);
	jstring jboard_name = (*env)->NewStringUTF(env, board->header.filename);
	if (jboard_name == NULL)
		return -1;

	jboolean hasException;
	memset(&hasException, 0, sizeof(jboolean));
	jobject jboard = jni_utils_allocate_new_object_by_classname(env, &hasException, "edu/xjtu/bmybbs/ythtbbs/Board", "(Ljava/lang/String;)V", jboard_name);
	(*env)->DeleteLocalRef(env, jboard_name);
	if (jboard == NULL)
		return -1;

	jni_utils_call_method_by_name(env, &hasException, list_board, "add", "(Ljava/lang/Object;)Z", jboard);
	(*env)->DeleteLocalRef(env, jboard);
	return 0;
};

JNIEXPORT jobject JNICALL Java_edu_xjtu_bmybbs_ythtbbs_Board_getAllBoards(JNIEnv *env, jclass clazz) {
	(void) clazz;
	jboolean hasException;
	memset(&hasException, 0, sizeof(jboolean));

	// 创建容器
	jobject list_board = jni_utils_allocate_new_object_by_classname(env, &hasException, "java/util/ArrayList", "()V");
	if (list_board == NULL)
		return NULL;

	ythtbbs_cache_utmp_resolve();
	ythtbbs_cache_UserTable_resolve();
	ythtbbs_cache_Board_resolve();

	ythtbbs_cache_Board_foreach_v(getAllBoards_callback, env, list_board);

	return list_board;
}

JNIEXPORT jobject JNICALL Java_edu_xjtu_bmybbs_ythtbbs_Board_getBoardByName(JNIEnv *env, jclass clazz, jstring jstr_bname) {
	(void) clazz;
	jboolean hasException;
	memset(&hasException, 0, sizeof(jboolean));
	if (jstr_bname == NULL)
		return NULL;

	const char *bname = (*env)->GetStringUTFChars(env, jstr_bname, &hasException);

	ythtbbs_cache_utmp_resolve();
	ythtbbs_cache_UserTable_resolve();
	ythtbbs_cache_Board_resolve();

	const struct boardmem *board = ythtbbs_cache_Board_get_board_by_name(bname);
	if (board == NULL)
		return NULL;

	jstring jboard_name = (*env)->NewStringUTF(env, board->header.filename);
	if (jboard_name == NULL)
		return NULL;

	jobject jboard = jni_utils_allocate_new_object_by_classname(env, &hasException, "edu/xjtu/bmybbs/ythtbbs/Board", "(Ljava/lang/String;)V", jboard_name);
	(*env)->DeleteLocalRef(env, jboard_name);

	return jboard;
}

