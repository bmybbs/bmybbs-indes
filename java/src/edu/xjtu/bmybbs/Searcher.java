package edu.xjtu.bmybbs;

import com.google.gson.Gson;
import edu.xjtu.bmybbs.ythtbbs.Article;
import edu.xjtu.bmybbs.ythtbbs.Board;
import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.cn.smart.SmartChineseAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.FSDirectory;

import java.io.File;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class Searcher {
	public static void Search(String bname, String querystr) {
		Board board = new Board(bname);
		String path = board.getRIndexPath();

		File f = new File(path);
		querystr = querystr.trim();
		if (!f.exists() || !f.isDirectory() || querystr.length() == 0) {
			return;
		}

		IndexReader reader;
		try {
			reader = DirectoryReader.open(FSDirectory.open(Paths.get(path)));
		} catch (IOException e) {
			System.err.format("无法打开索引: %s\n", path);
			return;
		}

		IndexSearcher searcher = new IndexSearcher(reader);
		Analyzer analyzer = new SmartChineseAnalyzer();

		QueryParser parser = new QueryParser("contents", analyzer);
		Query query;
		try {
			query = parser.parse(querystr);
		} catch (ParseException e) {
			System.err.format("查询输入有误: %s\n", querystr);
			return;
		}
		TopDocs result;
		ScoreDoc[] hits;
		try {
			result = searcher.search(query, 1000);
		} catch (IOException e) {
			System.err.println("无法查询");
			return;
		}
		hits = result.scoreDocs;
		List<Article> articles = new ArrayList<>();
		for (ScoreDoc sd : hits) {
			Document doc = null;
			try {
				doc = searcher.doc(sd.doc);
			} catch (IOException e) {
				System.err.format("无法获取内容: ScoreDoc %d\n", sd.doc);
				continue;
			}
			articles.add(new Article(bname, doc.get(Constants.FIELD_TITLE), doc.get(Constants.FIELD_OWNER), doc.get(Constants.FIELD_TIMESTAMP), doc.get(Constants.FIELD_THREAD)));
		}
		outputArticles(articles);
	}

	private static void outputArticles(List<Article> articles) {
		if (articles != null) {
			System.out.println(new Gson().toJson(articles));
		}
	}
}
