



class Foo { 
    static { System.out.println("stat block from class Foo"); } 
    public static void foo(){System.out.println("foo from foo");}
}


class SuperHW {

    static { System.out.println("stat block from SuperHW"); }
    
    protected void foo(){ System.out.println("foo"); }

    public SuperHW() {}

}
//interface Bar { static { System.out.println("stat block from class Bar"); } }

public class DoBreak extends SuperHW {

    static { System.out.println("stat block from HW"); Foo.foo();}

    public void foo(){ 
        super.foo();
        System.out.println("bar");
    }

    public static String join(String seperator, Object ... items){
        StringBuffer string = new StringBuffer();
        /*
        for(Object x : items) {
            string.append( x.toString() );
            string.append(seperator);
        }
        */
        /*
        
        
        */
        return string.substring(0, string.length() - seperator.length());
    }

    public static String join(Object ... items){
        return join("", items);
    }

    public static void main(String argv[]) {

        //System.out.println(join( ",:", 1 , 4, 2, 7, 4, 8));
        System.out.println(join( " ", "foo", "bar", 4));


/*
        int state = 0;
        
        try { if (argv.length > 0) state = Integer.parseInt(argv[0]); }
        catch (NumberFormatException orz) {}
        
        System.out.println(state);
        
        boolean err = state == 0 ? true : false;
        int lvl = 0;
        
        do {

            if (err) break;
            System.out.println("do A"); 
            err = state == 1 ? true : false;
            lvl++;
        
            if (err) break;
            System.out.println("do B"); 
            err = state == 2 ? true : false;
            lvl++;
        
            if (err) break;
            System.out.println("do C"); 
            err = state == 3 ? true : false;
            lvl++;
        
            if (err) break;
            
            System.out.println("reached inner state");

        } while(false);
    
        switch(lvl) {
            case 3: System.out.println("undo C"); 
            case 2: System.out.println("undo B");
            case 1: System.out.println("undo A");
            default: System.out.println("out");
        }
*/

/*
        SuperHW f = new SuperHW();
        SuperHW b = new HW();
        f.foo();
        b.foo();
*/

        System.exit(0);
    }

}
