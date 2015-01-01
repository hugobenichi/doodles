import java.util.*;

class TrieNode {

    private final TreeMap<Character,TrieNode> suffix; // all possible suffix
    private boolean is_leaf;                          // yes if makes a word

    public TrieNode() {
        this.suffix = new TreeMap<Character,TrieNode>();
        is_leaf = false;
    }

    public boolean find(char[] string, int index) {
        if (index == string.length) {
            return this.is_leaf;
        }

        Character next_c = Character.valueOf(string[index]);

        if (suffix.containsKey(next_c))
            return suffix.get(next_c).find(string, index+1);

        return false; 
    }

    public boolean add(char[] string, int index) {
        if (index == string.length) {
            this.is_leaf = true;            // we just found the end of a word
            return false;
        }

        Character next_c = Character.valueOf(string[index]);
        boolean new_node = false;

        if (!suffix.containsKey(next_c)) {
            suffix.put(next_c, new TrieNode());
            new_node = true;
        }

        return new_node | suffix.get(next_c).add(string, index+1);
    }

}

public class Trie {

    private final TrieNode root;

    public Trie() {
        root = new TrieNode();
    }

    public boolean insert(String s) {
        if (s == null || s.length() == 0) return false;
        return root.add(s.toCharArray(), 0); 
    }

    public boolean contains(String s) {
        if (s == null || s.length() == 0) return false;
        return root.find(s.toCharArray(), 0); 
    }

    public static Trie fromCol(Iterable<String> string_col) {
        Trie dic = new Trie();
        for (String s : string_col) dic.insert(s);
        return dic; 
    }

    public static void main(String argv[]) {

        Trie dic = new Trie();
    
        String[] strings = {"fo", "foo", "bar", "baz", "fistule", "etron", "tos"};

        for (String s: strings) dic.insert(s);

        System.out.println(dic.contains("foo"));
        System.out.println(dic.contains("barr"));
        System.out.println(dic.contains(""));
        System.out.println(dic.contains("aaa"));
        System.out.println(dic.contains("atron"));
        System.out.println(dic.contains("etron"));

        System.exit(0);
    }

}
