public class Compress {

	public static String comp(String s) {
		StringBuffer comp = new StringBuffer();	
		for ( int i = 0, l = s.length(); i < l;  ) {
			int count = 0;
			char letter =  s.charAt(i);
			while( i < l && letter == s.charAt(i) ) {
				count++;
				i++;
			}
			comp.append(letter);
			if( count > 1) {
				comp.append(count);
			}		
		}

		return comp.toString();
	}
	
	public static void main(String[] args){
	
		if( args.length > 0) {
			System.out.println( comp(args[0]) );
		}
	
		System.exit(0);
	}
}
