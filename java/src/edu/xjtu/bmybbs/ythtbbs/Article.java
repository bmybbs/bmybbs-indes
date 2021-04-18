package edu.xjtu.bmybbs.ythtbbs;

import edu.xjtu.bmybbs.Constants;

public class Article {
	private String board;
	private String title;
	private String owner;
	private String filename;

	public Article(String board, String title, String owner, long timestamp) {
		this.board = board;
		this.title = title;
		this.owner = owner;
		this.filename = String.format("M.%d.A", timestamp);
	}

	public String getFilename() {
		return this.filename;
	}

	public String getOwner() {
		return this.owner;
	}

	public String getTitle() {
		return this.title;
	}

	public String getPath() {
		return String.format("%s/%s/%s", Constants.BOARDSPATH, this.board, this.filename);
	}

	public native String getContent();

	static {
		System.loadLibrary("bmybbs-java");
	}
}
