import java.util.Arrays;
import java.util.List;
import java.util.LinkedList;

enum Op{

    And('&') { boolean calc(boolean x, boolean y) { return x&y;} },
    Or('|')  { boolean calc(boolean x, boolean y) { return x|y;} },
    Xor('^') { boolean calc(boolean x, boolean y) { return x^y;} };

    private final char symbol;

    Op(char c) { this.symbol = c;}

    abstract boolean calc(boolean x, boolean y);
      
    public static Op parse(char c) {
        Op o = null;
        switch(c) {
            case '&': o = And; break;
            case '|': o = Or;  break;
            case '^': o = Xor; break;
        }
        return o;
    }

    public List<String> combine(List<String> left, List<String> right) {
        List<String> comb = new LinkedList<String>();
        for (String l : left)
            for (String r: right)
                comb.add('('+ l + this.symbol + r +')');
        return comb;
    }

    public List<String> nest_match(String left, String right, boolean l_rez, boolean r_rez) {
        List<String> l_matches = Boolexp.match(left, l_rez);
        if (l_matches == null) return null;
        List<String> r_matches = Boolexp.match(right, r_rez);
        if (r_matches == null) return null;
        return combine(l_matches, r_matches);
    }

    public List<String> match(String left, String right, boolean rez) {
        List<String> matching_exp = new LinkedList<String>();

        if ( this.calc(false, false) == rez ) {
            List<String> rz = nest_match(left, right, false, false);
            if (rz != null) matching_exp.addAll(rz);
        }
        if ( this.calc(true, false) == rez ) {
            List<String> rz = nest_match(left, right, false, true);
            if (rz != null) matching_exp.addAll(rz);
                         rz = nest_match(left, right, true, false);
            if (rz != null) matching_exp.addAll(rz);
        }
        if ( this.calc(true, true) == rez ) {
            List<String> rz = nest_match(left, right, true, true);
            if (rz != null) matching_exp.addAll(rz);
        }

        if ( matching_exp.isEmpty()) return null;
        return matching_exp;
    }

}


public class Boolexp {

    public static String parse(String c, boolean rez) {
        if (c.equals("1") &&  rez) return c;
        if (c.equals("0") && !rez) return c;
        return null;
    }

    public static List<String> match(String exp, boolean rez){
        List<String> matching_exp = new LinkedList<String>();

        if (exp.length() == 1) {
            String p = parse(exp, rez);
            if (p != null) matching_exp.add(p);
            else matching_exp = null;
        } else {

            for (int i = 1; i < exp.length(); i += 2) {
                List<String> matches = Op.parse(exp.charAt(i)).match(
                    exp.substring(0, i),
                    exp.substring(i+1, exp.length()),
                    rez
                );
                if (matches != null && !matches.isEmpty())
                    matching_exp.addAll(matches);  
            }

            if (matching_exp.isEmpty()) matching_exp = null;
        }

        return matching_exp;
    } 

    public static void main(String argv[]) {
        String exp = "1^0|0|1";

        List<String> ret = Boolexp.match(exp, false);

        if (ret != null) for(String s: ret) System.out.println(s);
    } 

}
