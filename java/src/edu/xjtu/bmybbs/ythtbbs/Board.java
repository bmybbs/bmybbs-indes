package edu.xjtu.bmybbs.ythtbbs;

import edu.xjtu.bmybbs.Constants;

import java.util.List;

public class Board {
	private String name;

	public Board(String bname) {
		this.name = bname;
	}

	public String getName() {
		return name;
	}

	public String getRIndexPath() {
		return String.format("%s/%s/%s", Constants.BOARDSPATH, this.name, Constants.RECENT_INDEX);
	}

	public static native List<Board> getAllBoards();

	public static native Board getBoardByName(String bname);

	public native List<Article> getAllArticlesAfterTimestamp(long timestamp);

	static {
		System.loadLibrary("bmybbs-java");
	}
}
