import java.util.ArrayList;

public class AnimalTest {
    public static void main(String[] args) {
        Animal[] animals = new Animal[6];
        for (int i = 0; i < animals.length; i++) {
            if (i % 2 == 0) {
                animals[i] = new Monkey();
            } else {
                animals[i] = new Human();
            }
        }
        for (Animal a : animals) {
            a.speak();
        }
    }
}
