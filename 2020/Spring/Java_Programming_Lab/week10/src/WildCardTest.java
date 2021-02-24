import java.util.ArrayList;

public class WildCardTest {
    public static void main(String[] args) {
        ArrayList<Integer> intList = new ArrayList<Integer>();
        for(int i=0;i<5;i++) {
            intList.add(i);
        }
        ArrayList<Double> doubleList = new ArrayList<Double>();
        for(int i=0;i<5;i++) {
            doubleList.add((double)i);
        }
        ArrayList<Number> numList = new ArrayList<Number>();
        for(int i=0;i<5;i++) {
            numList.add((double)i);
            numList.add(i);
        }
        System.out.println(sum(intList));
        System.out.println(sum(doubleList));
        System.out.println(sum(numList));
    }
    public static double sum(ArrayList<? extends Number> list) {
        double total = 0;
        for (int i = 0; i < list.size(); i++) {
            total += list.get(i).doubleValue();
        }
        return total;
    }
}
