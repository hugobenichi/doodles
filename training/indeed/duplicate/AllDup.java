import java.util.Set;
import java.util.HashSet;

public class AllDup {

  public Iterable<String> duplicateIn(Iterable<String> source) {
    Set<String> seen = new HashSet<>();
    Set<String> dups = new HashSet<>();
    for (String s : source) {
      if (seen.contains(s)) { dups.add(s); }
      seen.add(s);
    }
    return dups;
  }

}
