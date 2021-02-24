import java.util.*;

public class ContactList {
    HashMap<String, String> map = new HashMap<>();

    void show() {
        TreeMap<String, String> tm = new TreeMap<>(this.map);
        Iterator<String> iteratorKey = tm.keySet().iterator();
        while (iteratorKey.hasNext()) {
            String key = iteratorKey.next();
            System.out.println(key + " " + tm.get(key));
        }
    }

    void find(String name) {
        boolean flag = true;
        for (String key : map.keySet()) {
            if (key.equals(name)) {
                System.out.println(map.get(key));
                flag = false;
                break;
            }
        }
        if (flag) {
            System.out.println("error");
        }
    }

    void add(String name, String number) {
        for (String key : map.keySet()) {
            if (key.equals(name)) {
                System.out.println("error");
                return;
            }
        }
        map.put(name, number);
    }

    void delete(String name) {
        boolean flag = true;
        String key_to_remove = new String();
        for (String key : map.keySet()) {
            if (key.equals(name)) {
                key_to_remove = key;
                flag = false;
                break;
            }
        }
        if (flag) {
            System.out.println("error");
        } else {
            map.remove(key_to_remove);
        }
    }

    public static void main(String[] args) {
        ContactList list = new ContactList();
        
        @SuppressWarnings("resource")
        Scanner sc = new Scanner(System.in);

        while (true) {
            String s = sc.nextLine();
            String[] arg = s.split(" ");

            switch (arg[0]) {
                case "show":
                    list.show();
                    break;

                case "find":
                    if (arg.length != 2) {
                        System.out.println("error");
                    } else {
                        list.find(arg[1]);
                    }
                    break;

                case "add":
                    if (arg.length != 3) {
                        System.out.println("error");
                    } else {
                        list.add(arg[1], arg[2]);
                    }
                    break;

                case "delete":
                    if (arg.length != 2) {
                        System.out.println("error");
                    } else {
                        list.delete(arg[1]);
                    }
            }
        }
    }
}