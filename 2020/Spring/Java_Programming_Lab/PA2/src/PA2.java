import java.io.*;
import java.util.*;

public class PA2 {
    int num_of_bomb;
    char mode;
    String file_name = new String();
    int score = 0;
    Board board = new Board();

    public static void main(String[] args) throws IOException {
        PA2 p = new PA2();
        try {
            p.input();
        } catch (Exception e) {
            System.out.println(e.toString());
            return;
        }
        p.setup_board();
        p.play_game();      
    }

    void input() {
        @SuppressWarnings("resource")
        Scanner scn = new Scanner(System.in);
        String s = scn.nextLine();
        String[] arg = s.split(" ");
        num_of_bomb = Integer.parseInt(arg[0]);
        mode = arg[1].charAt(0);
        for (int i = 2; i < arg.length; i++) {
            file_name = file_name + arg[i];
            if (i != arg.length - 1)
                file_name = file_name + " ";
        }
        if (num_of_bomb <= 0)
            throw new BombInputException();
        if ("dDrR".indexOf(mode) == -1)
            throw new ModeInputException();
    }

    void setup_board() throws IOException {
        try {
            InputStream fis = new FileInputStream(file_name);
            InputStreamReader isr = new InputStreamReader(fis);
            @SuppressWarnings("resource")
            BufferedReader br = new BufferedReader(isr);
            for (int i = 0; i < 10; i++) {
                String temp = br.readLine();
                for (int j = 0; j < temp.length(); j++) {
                    char c = Character.toUpperCase(temp.charAt(j));
                    if (c != ' ')
                        board.assign(i, j, c);
                }
            }
        } catch (FileNotFoundException e) {
            board.random();
        }
    }


    void play_game() {
        Scanner scn = new Scanner(System.in);
        for (int i = 0; i < num_of_bomb;) {
            if (Character.toUpperCase(mode) == 'D')
                board.print();
            String input = scn.nextLine();
            int x = (input.length() == 3) ? 9 : (int) (input.charAt(1) - '1');
            int y = (int) (input.charAt(0) - 'A');
            try {
                char result = board.hit(x, y);
                if (result == 'a') score += 6;
                else if (result == 'b') score += 4;
                else if (result == 's' || result == 'd') score += 3;
                else if (result == 'p') score += 2;
            } catch (HitException e) {
                System.out.println("Try again");
                continue;
            }
            i++;
        }
        scn.close();
        board.print();
        System.out.println("Score " + score); 
    }
}

@SuppressWarnings("serial")
class BombInputException extends RuntimeException {
    public String toString() {
        return "BombInputException";
    }
}

@SuppressWarnings("serial")
class ModeInputException extends RuntimeException {
    public String toString() {
        return "ModeInputException";
    }
}

@SuppressWarnings("serial")
class HitException extends RuntimeException {
    public String toString() {
        return "HitException";
    }
}

class ship {
    char type;
    int size;
}

class AircraftCarrier extends ship {
    AircraftCarrier() {
        type = 'A';
        size = 6;
    }
}

class Battleship extends ship {
    Battleship() {
        type = 'B';
        size = 4;
    }
}

class Submarine extends ship {
    Submarine() {
        type = 'S';
        size = 3;
    }
}

class Destroyer extends ship {
    Destroyer() {
        type = 'D';
        size = 3;
    }
}

class PatrolBoat extends ship {
    PatrolBoat() {
        type = 'P';
        size = 2;
    }
}

class Board {
    char[][] board = new char[10][10];
    String ship = "ABSDP";

    void print() {
        System.out.println("     A  B  C  D  E  F  G  H  I  J  ");
        System.out.println("     -  -  -  -  -  -  -  -  -  -  ");
        for (int i = 0; i < 10; i++) {
            System.out.printf("%-2d |", i + 1);
            for (int j = 0; j < 10; j++) {
                if (board[i][j] == 0) {
                    System.out.print("   ");
                } else if (ship.indexOf(board[i][j]) != -1 || board[i][j] == 'X') {
                    System.out.print(" " + board[i][j] + " ");
                } else {
                    System.out.print(" X" + board[i][j]);
                }
            }
            System.out.println(" ");
        }
    }

    void assign(int x, int y, char c) {
        board[x][y] = c;
    }

    char hit(int x, int y) {
        if (ship.indexOf(board[x][y]) != -1) {
            System.out.println("Hit " + board[x][y]);
            board[x][y] = Character.toLowerCase(board[x][y]);
            return board[x][y];
        } else if (Character.isLowerCase(board[x][y]) || board[x][y] == 'X') {
            throw new HitException();
        } else {
            board[x][y] = 'X';
            System.out.println("Miss");
        }
        return 0;
    }

    void random() {
        ship temp = new AircraftCarrier();
        add_random(temp, true);
        temp = new Destroyer();
        add_random(temp, false);
        temp = new Battleship();
        add_random(temp, true);
        add_random(temp, false);
        temp = new Submarine();
        add_random(temp, true);
        add_random(temp, false);
        temp = new PatrolBoat();
        add_random(temp, true);
        add_random(temp, false);
        add_random(temp, true);
        add_random(temp, false);
    }

    void add_random(ship s, Boolean mode) {
        Random random = new Random();
        int x = random.nextInt(11 - s.size);
        int y = random.nextInt(11 - s.size);
        while (!check_ship(x, y, s.size, mode)) {
            x = random.nextInt(11 - s.size);
            y = random.nextInt(11 - s.size);
        }

        int dx, dy;
        if (mode) {
            dx = 1;
            dy = 0;
        } else {
            dx = 0;
            dy = 1;
        }
        for (int i = 0; i < s.size; i++) {
            board[x + i * dx][y + i * dy] = s.type;
        }
    }

    Boolean check_ship(int x, int y, int length, Boolean mode) {
        int dx, dy;
        if (mode) {
            dx = 1;
            dy = 0;
        } else {
            dx = 0;
            dy = 1;
        }

        for (int i = 0; i < length; i++) {
            if (board[x + i * dx][y + i * dy] != 0 || i > 10)
                return false;
        }
        return true;
    }
}