package edu.xjtu.bmybbs;

import edu.xjtu.bmybbs.ythtbbs.Article;

public class Utils {
	public static void DumpArticle(String board, String timestamp) {
		Article a = new Article(board, null, null, Long.parseLong(timestamp));
		System.out.println(a.getContent());
	}
}
