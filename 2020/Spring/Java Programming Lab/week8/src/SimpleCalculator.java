import java.util.*;
import java.io.*;

@SuppressWarnings("serial")
class OutOfRangeException extends RuntimeException {
    public String toString() {
        return "OutOfRangeException";
    }
}

@SuppressWarnings("serial")
class AddZeroException extends RuntimeException {
    public String toString() {
        return "AddZeroException";
    }
}

@SuppressWarnings("serial")
class SubtractZeroException extends RuntimeException {
    public String toString() {
        return "SubtractZeroException";
    }
}

public class SimpleCalculator {
    boolean oper = false;   //true = +, false = -
    int a = 0, b = 0;

    void check_zero_exception() {
        if (a == 0 || b == 0) {
            if (oper)
                throw new AddZeroException();
            else
                throw new SubtractZeroException();
        }
    }

    void check_out_of_range(int x) {
        if (x < 0 || x > 1000)
            throw new OutOfRangeException();
    }

    void input(String s) {
        String[] arg;
        if (s.contains("+")) {
            oper = true;
            arg = s.split("\\+");
        } else {
            arg = s.split("\\-");
        }
        a = Integer.parseInt(arg[0]);
        b = Integer.parseInt(arg[1]);
    }

    int add() {
        int result = a + b;
        check_out_of_range(result);
        return result;
    }

    int subtract() {
        int result = a - b;
        check_out_of_range(result);
        return result;
    }

    public static void main(String[] args) {
        SimpleCalculator sc = new SimpleCalculator();
        Scanner scn = new Scanner(System.in);
        String s = scn.nextLine();
        scn.close();
        int res = 0;
        try {
            sc.input(s);
            sc.check_zero_exception();
            sc.check_out_of_range(sc.a);
            sc.check_out_of_range(sc.b);

            if (sc.oper)
                res = sc.add();
            else
                res = sc.subtract();

            System.out.println(res);
        } catch (OutOfRangeException e) {
            System.out.println(e.toString());
        } catch (AddZeroException e) {
            System.out.println(e.toString());
        } catch (SubtractZeroException e) {
            System.out.println(e.toString());
        }
    }
}