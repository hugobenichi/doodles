public class TestTrie {

    public static void test_read_file(String where, String[] to_look_for) {
        Trie dic = Trie.fromCol(LineReader.from(where));

        for (String s : to_look_for)
            System.out.println(s + ": " + dic.contains(s));

    }

    public static void main(String argv[]) {

        String us_dict = "/usr/share/dict/american-english";

        String[] strings = {"fo", "foo", "fool", "footwork", "seafood"};

        test_read_file(us_dict, strings);

        System.exit(0);
    }

}
