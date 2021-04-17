package edu.xjtu.bmybbs;

import edu.xjtu.bmybbs.ythtbbs.Board;

import java.util.Arrays;
import java.util.List;

public class Indexer {
	static private List<String> UNINDEX_BOARDS = Arrays.asList(
		"syssecurity",
		"millionairesrec",
		"newcomers"
	);

	public static void Index(String bname, String timestamp) throws IllegalArgumentException {
		if (bname == null)
			throw new IllegalArgumentException("版面名称参数为空");

		if (timestamp == null)
			throw new IllegalArgumentException("时间戳参数为空");

		long t;
		try {
			t = Long.parseLong(timestamp);
		} catch (NumberFormatException ex) {
			throw new IllegalArgumentException("时间戳不是有效的数值");
		}

		if ("ALL".equals(bname)) {
			List<Board> boards = Board.getAllBoards();
			for (Board board : boards) {
				if (!UNINDEX_BOARDS.contains(board.getName()))
					index_files(board, t);
			}
		} else {
			Board board = Board.getBoardByName(bname);
			if (board == null) {
				throw new IllegalArgumentException("版面不存在");
			} else {
				if (!UNINDEX_BOARDS.contains(board.getName()))
					index_files(board, t);
			}
		}
	}

	private static void index_files(Board board, long timestamp) {
	}
}
