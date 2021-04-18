package edu.xjtu.bmybbs;

import edu.xjtu.bmybbs.ythtbbs.Article;
import edu.xjtu.bmybbs.ythtbbs.Board;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;

import java.io.File;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;

public class Indexer {
	static private List<String> UNINDEX_BOARDS = Arrays.asList(
		"syssecurity",
		"millionairesrec",
		"newcomers"
	);

	public static void Index(String bname, String delta_day) {
		if (bname == null) {
			System.err.println("版面名称参数为空");
			return;
		}

		if (delta_day == null) {
			System.err.println("天数参数为空");
			return;
		}

		long t;
		try {
			t = "-1".equals(delta_day) ? 0 : (System.currentTimeMillis() / 1000 - Integer.parseInt(delta_day) * 86400 /* 24h */);
		} catch (NumberFormatException ex) {
			System.err.println("天数不是有效的数值");
			return;
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
				System.err.println("版面不存在");
			} else {
				if (!UNINDEX_BOARDS.contains(board.getName()))
					index_files(board, t);
			}
		}
	}

	private static void index_files(Board board, long timestamp) {
		Directory dir;
		try {
			dir = FSDirectory.open(Paths.get(board.getRIndexPath()));
		} catch (IOException ex) {
			System.err.format("无法打开或创建索引: %s\n", board.getRIndexPath());
			return;
		}

		Analyzer analyzer = new StandardAnalyzer();
		IndexWriterConfig iwc = new IndexWriterConfig(analyzer);
		iwc.setOpenMode(IndexWriterConfig.OpenMode.CREATE_OR_APPEND);
		IndexWriter writer;
		try {
			writer = new IndexWriter(dir, iwc);
		} catch (IOException e) {
			System.err.format("无法创建 writer: %s\n", board.getRIndexPath());
			return;
		}

		List<Article> articles = board.getAllArticlesAfterTimestamp(timestamp);
		for (Article article : articles) {
			String articlePath = article.getPath();
			File file = new File(articlePath);
			if (!file.exists()) {
				System.err.format("文件不存在: %s\n", articlePath);
				continue;
			}

			Document doc = new Document();

			// StringField: stored, not analyzed and indexed
			doc.add(new StringField("name", article.getFilename(), Field.Store.YES));
			doc.add(new StringField("owner", article.getOwner(), Field.Store.YES));
			doc.add(new StringField("title", article.getTitle(), Field.Store.YES));
			doc.add(new TextField("contents", article.getContent(), Field.Store.NO));

			try {
				writer.addDocument(doc);
			} catch (IOException e) {
				System.err.format("无法索引文件: %s\n", articlePath);
			}
		}
	}
}
