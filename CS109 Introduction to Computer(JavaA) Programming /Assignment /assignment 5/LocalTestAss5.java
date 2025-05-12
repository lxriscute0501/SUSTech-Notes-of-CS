import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import java.lang.reflect.Field;
import java.util.ArrayList;
import static org.junit.jupiter.api.Assertions.*;



public class spring25A5LocalTest {
    private StringBuilder sb;
    private int wrongCount;

    @BeforeEach
    public void prepareData() {
        sb = new StringBuilder();
        wrongCount = 0;
    }

    @Test
    public void test01Reflect() {
        Class<?> clzVehicle = Vehicle.class;
        String[] expectFieldsVehicle = {
                "protected java.lang.String Vehicle.plateNumber",
                "protected boolean Vehicle.isInside",
                "protected Time Vehicle.arriveTime"
        };
        ArrayList<String> fieldsVehicle = new ArrayList<>();
        for(Field field: clzVehicle.getDeclaredFields()) {
            fieldsVehicle.add(field.toString());
        }
        for (String s : expectFieldsVehicle) {
            if (!fieldsVehicle.contains(s)) {
                wrongCount++;
                sb.append(System.lineSeparator()).append(s).append(" is missing!");
            }
        }
        assertEquals(0, wrongCount, sb.toString());
    }

    @Test
    public void test02Time() {
        Time t1 = new Time(1, 20);
        t1.addMinutes(50);
        assertEquals("02:10", t1.toString());
        Time t2 = new Time(11, 20);
        t2.addMinutes(150);
        assertEquals("13:50", t2.toString());
    }

    @Test
    public void test03Bus() {
        Vehicle bus = new Bus("B12345");

        bus.setArriveTime(new Time(7,20));
        assertEquals("Bus B12345 true", bus.toString());
        assertEquals(0, bus.calculateMoney(new Time(7,40)));
        assertEquals("Bus B12345 false", bus.toString());

        bus.setArriveTime(new Time(8,0));
        assertEquals(15, bus.calculateMoney(new Time(8,40)));

        bus.setArriveTime(new Time(9,0));
        assertEquals(15, bus.calculateMoney(new Time(9,30)));

        bus.setArriveTime(new Time(10,0));
        assertEquals(15, bus.calculateMoney(new Time(10,50)));

        bus.setArriveTime(new Time(11,0));
        assertEquals(0, bus.calculateMoney(new Time(11,50)));

        bus.setArriveTime(new Time(12,0));
        assertEquals(55, bus.calculateMoney(new Time(17,50)));

        bus.setArriveTime(new Time(18,0));
        assertEquals(0, bus.calculateMoney(new Time(19,50)));
    }

    @Test
    public void test04Bus() {
        Vehicle bus = new Bus("B12346");

        bus.setArriveTime(new Time(7,20));
        assertEquals(30, bus.calculateMoney(new Time(8,20)));

        bus.setArriveTime(new Time(8,30));
        assertEquals(15, bus.calculateMoney(new Time(9,30)));

        bus.setArriveTime(new Time(9,50));
        assertEquals(0, bus.calculateMoney(new Time(10,15)));

        bus.setArriveTime(new Time(10,20));
        assertEquals(0, bus.calculateMoney(new Time(10,55)));
    }

    @Test
    public void test05Bus() {
        Vehicle bus = new Bus("B12347");

        bus.setArriveTime(new Time(7,0));
        assertEquals(15, bus.calculateMoney(new Time(7,59)));

        bus.setArriveTime(new Time(9,00));
        assertEquals(15, bus.calculateMoney(new Time(9,59)));

        bus.setArriveTime(new Time(11,00));
        assertEquals(0, bus.calculateMoney(new Time(11,29)));

        bus.setArriveTime(new Time(12,00));
        assertEquals(0, bus.calculateMoney(new Time(12,02)));
    }

    @Test
    public void test06Bus() {
        Vehicle bus = new Bus("B12348");

        bus.setArriveTime(new Time(7,0));
        assertEquals(0, bus.calculateMoney(new Time(7,29)));

        bus.setArriveTime(new Time(9,00));
        assertEquals(15, bus.calculateMoney(new Time(9,59)));

        bus.setArriveTime(new Time(11,00));
        assertEquals(45, bus.calculateMoney(new Time(13,01)));

        bus.setArriveTime(new Time(14,00));
        assertEquals(0, bus.calculateMoney(new Time(14,31)));

        bus.setArriveTime(new Time(15,00));
        assertEquals(15, bus.calculateMoney(new Time(15,31)));

        bus.setArriveTime(new Time(16,00));
        assertEquals(15, bus.calculateMoney(new Time(17,01)));

        bus.setArriveTime(new Time(17,30));
        assertEquals(10, bus.calculateMoney(new Time(18,31)));

        bus.setArriveTime(new Time(19,30));
        assertEquals(0, bus.calculateMoney(new Time(23,31)));
    }

    @Test
    public void test07Car() {
        Vehicle car = new Car("A12345");
        car.setArriveTime(new Time(7,20));
        assertEquals("Car A12345 true", car.toString());
        assertEquals(0, car.calculateMoney(new Time(7,40)));
        assertEquals("Car A12345 false", car.toString());

        car.setArriveTime(new Time(8,0));
        assertEquals(15, car.calculateMoney(new Time(8,50)));

        car.setArriveTime(new Time(9,30));
        assertEquals(5, car.calculateMoney(new Time(10,30)));

        car.setArriveTime(new Time(11,0));
        assertEquals(0, car.calculateMoney(new Time(11,20)));

        car.setArriveTime(new Time(11,30));
        assertEquals(20, car.calculateMoney(new Time(14,50)));
        assertEquals(0, car.calculateMoney(new Time(14,50)));

        car.setArriveTime(new Time(15,0));
        assertEquals(20, car.calculateMoney(new Time(20,0)));

        car.setArriveTime(new Time(21,0));
        assertEquals(0, car.calculateMoney(new Time(22,0)));
    }

    @Test
    public void test08Car() {
        Vehicle car = new Car("A12346");

        car.setArriveTime(new Time(8,0));
        assertEquals(0, car.calculateMoney(new Time(8,29)));

        car.setArriveTime(new Time(9,30));
        assertEquals(15, car.calculateMoney(new Time(10,01)));

        car.setArriveTime(new Time(11,0));
        assertEquals(0, car.calculateMoney(new Time(11,20)));

        car.setArriveTime(new Time(11,30));
        assertEquals(5, car.calculateMoney(new Time(12,01)));
    }

    @Test
    public void test09Car() {
        Vehicle car = new Car("A12347");

        car.setArriveTime(new Time(8,0));
        assertEquals(15, car.calculateMoney(new Time(8,31)));

        car.setArriveTime(new Time(9,0));
        assertEquals(10, car.calculateMoney(new Time(10,31)));

        car.setArriveTime(new Time(11,0));
        assertEquals(0, car.calculateMoney(new Time(11,31)));

        car.setArriveTime(new Time(12,00));
        assertEquals(10, car.calculateMoney(new Time(13, 29)));
    }
    @Test
    public void test10Car() {
        Vehicle car = new Car("A12348");

        car.setArriveTime(new Time(8,0));
        assertEquals(15, car.calculateMoney(new Time(8,59)));

        car.setArriveTime(new Time(9,10));
        assertEquals(5, car.calculateMoney(new Time(10,9)));

        car.setArriveTime(new Time(11,0));
        assertEquals(5, car.calculateMoney(new Time(11,31)));

        car.setArriveTime(new Time(12,0));
        assertEquals(5, car.calculateMoney(new Time(12, 59)));

        car.setArriveTime(new Time(13,20));
        assertEquals(0, car.calculateMoney(new Time(13, 51)));

        car.setArriveTime(new Time(14,0));
        assertEquals(5, car.calculateMoney(new Time(14, 31)));

        car.setArriveTime(new Time(15,0));
        assertEquals(5, car.calculateMoney(new Time(16, 1)));

        car.setArriveTime(new Time(16,30));
        assertEquals(0, car.calculateMoney(new Time(16, 58)));

        car.setArriveTime(new Time(18,0));
        assertEquals(20, car.calculateMoney(new Time(22, 2)));


    }

}
