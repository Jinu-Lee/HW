class Animal {

}

abstract class Mammal extends Animal {
    abstract String bark();
    abstract String getFood();
}

abstract class Reptile extends Animal {
    abstract String getFood();
}

final class Cat extends Mammal {
    private String name;
    private float weight;
    private String nameSlave;

    Cat(String n1, float w, String n2) {
        name = n1;
        weight = w;
        nameSlave = n2;
    }

    String getName() { return name; }
    void setName(String n) { name = n; }

    float getWeight() { return weight; }
    void setWeight(float w) { weight = w; }

    String getNameSlave() { return nameSlave; }
    void setNameSlave(String n) { nameSlave = n; }

    String bark() {
        String s = "Meow";
        System.out.println(s);
        return s;
    }

    String getFood() {
        String s = "Fish";
        System.out.println(s);
        return s;
    }
}

final class Dog extends Mammal {
    private String name;
    private float weight;
    private String nameMaster;

    Dog(String n1, float w, String n2) {
        name = n1;
        weight = w;
        nameMaster = n2;
    }

    String getName() { return name; }
    void setName(String n) { name = n; }

    float getWeight() { return weight; }
    void setWeight(float w) { weight = w; }

    String getNameMaster() { return nameMaster; }
    void setNameMaster(String n) { nameMaster = n; }

    String bark() {
        String s = "Bowbow";
        System.out.println(s);
        return s;
    }

    String getFood() {
        String s = "Apple";
        System.out.println(s);
        return s;
    }
}

final class Crocodile extends Reptile {
    private String name;
    private float weight;

    Crocodile(String n, float w) {
        name = n;
        weight = w;
    }

    String getName() { return name; }
    void setName(String n) { name = n; }

    float getWeight() { return weight; }
    void setWeight(float w) { weight = w; }

    String getFood() {
        String s = "Meat";
        System.out.println(s);
        return s;
    }
}