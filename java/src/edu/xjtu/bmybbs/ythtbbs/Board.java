package edu.xjtu.bmybbs.ythtbbs;

import java.util.List;

public class Board {
	private String name;

	public Board(String bname) {
		this.name = bname;
	}

	public static native List<Board> getAllBoards();

	static {
		System.loadLibrary("bmybbs-java");
	}
}
