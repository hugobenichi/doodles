public class Anagram {

	public static boolean isAnagram(String a, String b) {
	
		if( a.length() != b.length() ) return false;
		
		char[] a_chars = a.toCharArray(), 
		       b_chars = b.toCharArray();
		       
		java.util.Arrays.sort(a_chars);
		java.util.Arrays.sort(b_chars);		
	
		for(int i = 0, l = a.length(); i < l; i++ ) {
			if( a_chars[i] != b_chars[i] ) return false;
		}
	
		return true;
	}
	
	public static void main(String[] argv) {
	
		if( argv.length >= 2)
			System.out.println( isAnagram(argv[0], argv[1]) );
	
		System.exit(0);
	
	}
	
	
}
