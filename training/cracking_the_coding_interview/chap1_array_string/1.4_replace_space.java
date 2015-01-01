public class Replace {

	public static String replaceSpaces(String str_arg){
	
		int length = str_arg.length(), spaceCount = 0, i = 0;
		
		for(i = 0; i < length; i++) {
			if(str_arg.charAt(i) == ' ') {
				spaceCount++;
			}
		}
		
		int newLength = length + spaceCount * 2;
		char[] str = new char[newLength];
		
		for(i = 0; i < length; i++) {
			str[i] = str_arg.charAt(i);
		}		
	
		str[newLength] = '\0';
		for( i = length -1; i >= 0; i--){
			if( str[i] == ' ') {
				str[newLength -1] = '0';
				str[newLength -2] = '2';
				str[newLength -3] = '%';
			} else {
				str[newLength-1] = str[i];
				newLength = newLength -1;
			}
		}
	
		return new String(str);
	
	}
	
	public static void main(String[] argv){
		if( argv.length > 0 ){
			System.out.println( replaceSpaces( argv[0] ) );
		}
		System.exit(0);
	}
	
}
