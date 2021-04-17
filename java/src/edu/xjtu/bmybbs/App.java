package edu.xjtu.bmybbs;

class App {
	public static void main(String[] args) {
		if (args.length == 3) {
			if (args[0].equals("index")) {
				Indexer.Index(args[1], args[2]);
			}
		} else {
			System.err.println("使用参数错误，正确的调用方式：\n"
				+ "\tindex boardname timestamp\n"
				+ "或者\n"
				+ "\tsearch boardname querystr");
		}
	}
}

