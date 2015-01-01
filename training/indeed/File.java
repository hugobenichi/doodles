import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.charset.StandardCharsets;
import java.io.BufferedReader;
import java.io.BufferedWriter;

public class File {

  public static void main(String[] argv) {
    if (argv.length <= 1) System.exit(1);

    Path in = Paths.get(argv[0]);
    Path out = Paths.get(argv[1]);

try {
    BufferedReader rdr = Files.newBufferedReader(in, StandardCharsets.UTF_8);
    BufferedWriter wtr = Files.newBufferedWriter(out, StandardCharsets.UTF_8);

    for (;;) {
      String line = rdr.readLine();
      if (line == null) break;
      wtr.write(line);
      wtr.newLine();
    }
    wtr.close();
} catch (IOException ioe) {}

    System.exit(0);

  }

}
