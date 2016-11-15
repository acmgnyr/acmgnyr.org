import java.util.*;

public class TFSudoku {

    private static Scanner in = new Scanner(System.in);
    private static final int GRID_SIZE = 6;
    private static final List<Character> ALL
        = Arrays.asList('1','2','3','4','5','6','7','8','9');

    private static class Square implements Comparable<Square> {
        public boolean twoVals;
        public char topVal, bottomVal;
        public int row, col;
        Square(int r, int c) {
            twoVals = false;
            topVal = '-';
            bottomVal = '*';
            row = r; col = c;
        }
        public String toString() {
            String result = "";
            result += topVal;
            if (twoVals) {
                result += "/" + bottomVal;
            }
            return result;
        }
        public int compareTo(Square other) {
            return availAll().size() - other.availAll().size();
        }
        TreeSet<Character> availAll() { 
            TreeSet<Character> result
                = new TreeSet<Character>(availRow.get(row));
            result.retainAll(availCol.get(col));
            result.retainAll(availBlock.get(blockNum(row, col)));
            return result;
        }
    }


    private static Square[][] tab = new Square[GRID_SIZE][GRID_SIZE];
    private static PriorityQueue<Square> toDo = new PriorityQueue<Square>();
    private static ArrayList<TreeSet<Character>> availRow,
        availCol, availBlock;

    private static ArrayList<TreeSet<Character>> initAvail() {
        ArrayList<TreeSet<Character>> result
            = new ArrayList<TreeSet<Character>>();
        for (int i = 0; i < GRID_SIZE; i++) {
            result.add(new TreeSet<Character>(ALL));
        }
        return result;
    }

    private static int blockNum(int row, int col) {
        return (row & (~1)) + (col >= GRID_SIZE/2 ? 1 : 0);
    }

    private static void initAttempt() {
        availRow = initAvail();
        availCol = initAvail();
        availBlock = initAvail();
        for (int row = 0; row < tab.length; row++) {
            for (int col = 0; col < tab[row].length; col++) {
                if (Character.isDigit(tab[row][col].topVal)) {
                    char d = tab[row][col].topVal;
                    availRow.get(row).remove(d);
                    availCol.get(col).remove(d);
                    availBlock.get(blockNum(row,col)).remove(d);
                    if (tab[row][col].twoVals 
                        && !Character.isDigit(tab[row][col].bottomVal))
                    {
                        toDo.add(tab[row][col]);
                    }
                } else {
                    toDo.add(tab[row][col]);
                }
                if (tab[row][col].twoVals
                    && Character.isDigit(tab[row][col].bottomVal))
                {
                    char d = tab[row][col].bottomVal;
                    availRow.get(row).remove(d);
                    availCol.get(col).remove(d);
                    availBlock.get(blockNum(row,col)).remove(d);
                }
            }
        }
    }

    private static boolean attempt() {
        if (toDo.isEmpty()) { return true; }
        Square curr = toDo.poll();
        int row = curr.row;
        int col = curr.col;
        int block = blockNum(row, col);
        boolean firstNotSet
                = !Character.isDigit(curr.topVal);
        TreeSet<Character> avail = curr.availAll();
        for (char p : avail) {
            if (firstNotSet) {
                curr.topVal = p;
                availRow.get(row).remove(p);
                availCol.get(col).remove(p);
                availBlock.get(block).remove(p);
            }
            if (curr.twoVals
                    && !Character.isDigit(curr.bottomVal))
            {
                for (char q : avail.tailSet(p)) {
                    if (q > curr.topVal) {
                        curr.bottomVal = q;
                        availRow.get(row).remove(q);
                        availCol.get(col).remove(q);
                        availBlock.get(block).remove(q);
                        if (attempt()) { return true; }
                        availRow.get(row).add(q);
                        availCol.get(col).add(q);
                        availBlock.get(block).add(q);
                        curr.bottomVal = '-';
                    }
                }
            } else if (!curr.twoVals
                || curr.topVal < curr.bottomVal)
            {
                if (attempt()) { return true; }
            }
            if (firstNotSet) {
                availRow.get(row).add(p);
                availCol.get(col).add(p);
                availBlock.get(block).add(p);
                curr.topVal = '-';
            }
        }
        toDo.add(curr);
        return false;
    }

    private static void readTable(Square[][] t)
    {
        for (int row = 0; row < t.length; row++) {
            for (int col = 0; col < t[row].length; col++) {
                String s = in.next();
                t[row][col] = new Square(row, col);
                t[row][col].topVal = s.charAt(0);
                if (s.length() > 1) {
                    t[row][col].twoVals = true;
                    t[row][col].bottomVal = s.charAt(2);
                }
            }
        }
    }

    private static void processTable(Square[][] t)
    {
        initAttempt();
        attempt();
        for (int row = 0; row < t.length; row++) {
            for (int col = 0; col < t[row].length; col++) {
                System.out.print(t[row][col] + " ");
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        int p = in.nextInt();
        for (int i = 1; i <= p; i++) {
            int d = in.nextInt();
            readTable(tab);
            System.out.println(d);
            processTable(tab);
        }

    }

}
