import java.io.Console;

public class Parrot {
    public static void main(String[] args) {
        Console cns = System.console();

        if (cns == null) {
            System.out.println("FUUUCK!");
            return;
        }

        String name = cns.readLine(
            "Enter your name: "
        );

        for (int i = 0; i < 10; ++i) {
            System.out.println(name);
        }
    }
}