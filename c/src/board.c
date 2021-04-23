#include <string.h>
#include "jni_utils.h"
#include "ytht/fileop.h"
#include "bmy/convcode.h"
#include "ythtbbs/cache.h"
#include "ythtbbs/board.h"
#include "ythtbbs/article.h"

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

/*
 * Class:     edu_xjtu_bmybbs_ythtbbs_Board
 * Method:    getAllBoards
 * Signature: ()Ljava/util/List;
 */
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

/*
 * Class:     edu_xjtu_bmybbs_ythtbbs_Board
 * Method:    getBoardByName
 * Signature: (Ljava/lang/String;)Ledu/xjtu/bmybbs/ythtbbs/Board;
 */
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
	(*env)->ReleaseStringUTFChars(env, jstr_bname, bname);
	if (board == NULL)
		return NULL;

	jstring jboard_name = (*env)->NewStringUTF(env, board->header.filename);
	if (jboard_name == NULL)
		return NULL;

	jobject jboard = jni_utils_allocate_new_object_by_classname(env, &hasException, "edu/xjtu/bmybbs/ythtbbs/Board", "(Ljava/lang/String;)V", jboard_name);
	(*env)->DeleteLocalRef(env, jboard_name);

	return jboard;
}

/*
 * Class:     edu_xjtu_bmybbs_ythtbbs_Board
 * Method:    getAllArticlesAfterTimestamp
 * Signature: (J)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_edu_xjtu_bmybbs_ythtbbs_Board_getAllArticlesAfterTimestamp(JNIEnv *env, jobject jThis, jlong jTimestamp) {
	jclass clazz;
	jfieldID fid;
	jstring jstr_board, jstr_owner, jstr_title;
	size_t count;
	int i;
	const char *board;
	const struct fileheader *article_arr = NULL, *x = NULL, EMPTY;
	char buf[160], title_utf[sizeof(EMPTY.title) * 2];
	struct mmapfile mf = { .ptr = NULL };

	clazz = (*env)->GetObjectClass(env, jThis);                           // create 1
	fid = (*env)->GetFieldID(env, clazz, "name", "Ljava/lang/String;");
	(*env)->DeleteLocalRef(env, clazz);                                   // release 1
	if (fid == NULL) {
		return NULL;
	}
	jstr_board = (*env)->GetObjectField(env, jThis, fid);                 // create 2
	board = (*env)->GetStringUTFChars(env, jstr_board, NULL);             // create 3
	if (board == NULL) {
		(*env)->DeleteLocalRef(env, jstr_board);                          // release 2.1
		return NULL;
	}

	snprintf(buf, sizeof(buf), MY_BBS_HOME "/boards/%s/.DIR", board);
	(*env)->ReleaseStringUTFChars(env, jstr_board, board);                // release 3
	if (mmapfile(buf, &mf) < 0) {
		(*env)->DeleteLocalRef(env, jstr_board);                          // release 2.2
		return NULL;
	}

	article_arr = (const struct fileheader *) mf.ptr;
	count = mf.size / sizeof(struct fileheader);

	jboolean hasException;
	memset(&hasException, 0, sizeof(jboolean));
	// 创建容器
	jobject list_article = jni_utils_allocate_new_object_by_classname(env, &hasException, "java/util/ArrayList", "()V");
	if (list_article != NULL) {
		for (i = count - 1; i >= 0; i--) {
			x = &article_arr[i];
			if (x->filetime < (long) jTimestamp)
				break;

			jstr_owner = (*env)->NewStringUTF(env, x->owner);             // create 4
			if (jstr_owner == NULL)
				continue;
			g2u(x->title, sizeof(EMPTY.title), title_utf, sizeof(title_utf));
			jstr_title = (*env)->NewStringUTF(env, title_utf);            // create 5
			if (jstr_title == NULL) {
				(*env)->DeleteLocalRef(env, jstr_title);                  // release 4.1
				continue;
			}

			jobject jarticle = jni_utils_allocate_new_object_by_classname(env, &hasException, "edu/xjtu/bmybbs/ythtbbs/Article", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JJ)V", jstr_board, jstr_title, jstr_owner, (jlong) x->filetime, (jlong) x->thread);
			(*env)->DeleteLocalRef(env, jstr_title);                      // release 5
			(*env)->DeleteLocalRef(env, jstr_owner);                      // release 4
			if (jarticle == NULL)
				continue;
			jni_utils_call_method_by_name(env, &hasException, list_article, "add", "(Ljava/lang/Object;)Z", jarticle);
			(*env)->DeleteLocalRef(env, jarticle);
		}
	}

	(*env)->DeleteLocalRef(env, jstr_board);                              // release 2
	mmapfile(NULL, &mf);
	return list_article;
}

