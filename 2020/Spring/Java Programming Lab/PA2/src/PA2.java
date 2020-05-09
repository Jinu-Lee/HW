import java.io.*;
import java.util.*;

public class PA2 {
    public static void main(String[] args) throws FileNotFoundException, IOException {
        try {
            @SuppressWarnings("resource")
            Scanner scn = new Scanner(System.in);
            String s = scn.nextLine();
            String[] arg = s.split(" ");
            int num = Integer.parseInt(arg[0]);
            char mode = arg[1].charAt(0);
            String file_name = new String();
            for (int i = 2; i < arg.length; i++) {
                file_name = file_name + arg[i];
                if (i != arg.length - 1)
                    file_name = file_name + " ";
            }
            if (num <= 0)
                throw new BombInputException();
            if ("dDrR".indexOf(mode) == -1)
                throw new ModeInputException();

            Board board = new Board();
            
            if(file_name.length() != 0)
            {
                InputStream fis = new FileInputStream(file_name);
                InputStreamReader isr = new InputStreamReader(fis);
                BufferedReader br = new BufferedReader(isr);
                
                for (int i = 0; i < 10; i++) {
                    String temp = br.readLine();
                    for (int j = 0; j < temp.length(); j++) {
                        char c = Character.toUpperCase(temp.charAt(j));
                        if (c != ' ')
                            board.assign(i, j, c);
                    }
                }
                
                fis.close(); isr.close(); br.close();
            }
            else
            {
                board.random();
            }

            int score = 0;
            for (int i = 0; i < num;) {
                if (Character.toUpperCase(mode) == 'D')
                    board.print();
                String input = scn.nextLine();
                int x = (input.length() == 3) ? 9 : (int) input.charAt(1) - '1';
                int y = (int) (input.charAt(0) - 'A');
                try {
                    if (board.hit(x, y))
                        score += 1;
                } catch (HitException e) {
                    System.out.println("Try again");
                    continue;
                }
                i++;
            }
            board.print();
            System.out.println("Score " + score);
        } catch (BombInputException e) {
            System.out.println(e.toString());
        } catch (ModeInputException e) {
            System.out.println(e.toString());
        }
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

    boolean hit(int x, int y) {
        if (ship.indexOf(board[x][y]) != -1) {
            System.out.println("Hit " + board[x][y]);
            board[x][y] = Character.toLowerCase(board[x][y]);
            return true;
        } else if (Character.isLowerCase(board[x][y]) || board[x][y] == 'X') {
            throw new HitException();
        } else {
            board[x][y] = 'X';
            System.out.println("Miss");
        }
        return false;
    }

    void random() {
        ship temp = new AircraftCarrier(); add_random(temp, true);
        temp = new Destroyer(); add_random(temp, false);
        temp = new Battleship(); add_random(temp, true); add_random(temp, false);
        temp = new Submarine(); add_random(temp, true); add_random(temp, false);
        temp = new PatrolBoat(); add_random(temp, true); add_random(temp, false); add_random(temp, true); add_random(temp, false);
    }

    void add_random(ship s, Boolean mode) {
        Random random = new Random();
        int x = random.nextInt(11 - s.size);
        int y = random.nextInt(11 - s.size);
        while(!check_ship(x, y, s.size, mode)) {
            x = random.nextInt(11 - s.size);
            y = random.nextInt(11 - s.size);
        }
        
        int dx, dy;
        if (mode) { dx = 1; dy = 0; } 
        else { dx = 0; dy = 1; }
        for(int i = 0; i < s.size; i++) {
            board[x + i * dx][y + i * dy] = s.type;
        }
    }

    Boolean check_ship(int x, int y, int length, Boolean mode) {
        int dx, dy;
        if (mode) { dx = 1; dy = 0; } 
        else { dx = 0; dy = 1; }

        for(int i = 0; i < length; i++) {
            if(board[x + i * dx][y + i * dy] != 0 || i > 10) return false;
        }
        return true;
    }
}