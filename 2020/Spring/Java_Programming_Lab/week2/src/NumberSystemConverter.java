import java.util.Scanner;

public class NumberSystemConverter {
    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);

        int n = scan.nextInt();

        System.out.println("b " + Integer.toBinaryString(n));
        System.out.println("o " + Integer.toOctalString(n));
        System.out.println("h " + Integer.toHexString(n));

        scan.close();
    }

}