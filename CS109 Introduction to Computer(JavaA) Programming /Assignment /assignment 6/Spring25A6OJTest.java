import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;
public class Spring25A6OJTest {
    private ParkingLot parkingLot;
    private StringBuilder sb;
    private int wrongCount;

    @BeforeEach
    public void prepareData() {
        parkingLot = new ConcreteParkingLot(5, 3);
        sb = new StringBuilder();
        wrongCount = 0;
    }

    @Test
    public void test01Reflect() {
        Class<?> clz = ConcreteParkingLot.class;
        Class<?> clzVehicle = Vehicle.class;
        String[] expectFieldsParkingLot = {"private java.util.List ConcreteParkingLot.vehicles",
                "private Time ConcreteParkingLot.currentTime",
                "private final int ConcreteParkingLot.CAR_CAPACITY",
                "private final int ConcreteParkingLot.BUS_CAPACITY"};
        String[] expectFieldsVehicle = {
                "protected java.lang.String Vehicle.plateNumber",
                "protected boolean Vehicle.isInside",
                "protected Time Vehicle.arriveTime"
        };
        ArrayList<String> fields = Arrays.stream(clz.getDeclaredFields()).map(Field::toString).collect(Collectors.toCollection(ArrayList::new));
        for (String s : expectFieldsParkingLot) {
            if (!fields.contains(s)) {
                wrongCount++;
                sb.append(System.lineSeparator()).append(s).append(" is missing!");
            }
        }
        assertEquals(0, wrongCount, sb.toString());
        sb.setLength(0);
        wrongCount = 0;
        ArrayList<String> fieldsVehicle = Arrays.stream(clzVehicle.getDeclaredFields()).map(Field::toString).collect(Collectors.toCollection(ArrayList::new));
        for (String s : expectFieldsVehicle) {
            if (!fieldsVehicle.contains(s)) {
                wrongCount++;
                sb.append(System.lineSeparator()).append(s).append(" is missing!");
            }
        }
        assertEquals(0, wrongCount, sb.toString());
    }

    @Test
    @Timeout(value = 500, unit = TimeUnit.MILLISECONDS)
    public void test02Time() {
        parkingLot.minutesPassed(60);
        assertEquals("01:00", parkingLot.getTime().toString());
        parkingLot.minutesPassed(65);
        assertEquals("02:05", parkingLot.getTime().toString());
    }

    @Test
    @Timeout(value = 1000, unit = TimeUnit.MILLISECONDS)
    public void test03DriveIntoBasic() {
        parkingLot.minutesPassed(40);
        parkingLot.driveInto(0, "X0001", "X0002", "X0003", "X0004");
        parkingLot.minutesPassed(45);
        parkingLot.driveInto(1, "Y0001");
        assertEquals("car:4/5 bus:1/3", parkingLot.parkingLotStatus());
        String[] resultExcept = {"X0001 00:40",
                "X0002 00:41",
                "X0003 00:42",
                "X0004 00:43",
                "Y0001 01:28"};
        try {
            Field vehiclesField = ConcreteParkingLot.class.getDeclaredField("vehicles");
            vehiclesField.setAccessible(true);
            List<Vehicle> field = (List<Vehicle>) vehiclesField.get(parkingLot);
            assertEquals(5, field.size());
            List<String> mapResult = field.stream().map(f -> f.toString() + " " + f.getArriveTime()).toList();
            for (String result : resultExcept) {
                if (!mapResult.contains(result)) {
                    wrongCount++;
                    sb.append(System.lineSeparator()).append(result).append(" is missing!");
                }
            }

            vehiclesField.setAccessible(false);

        } catch (NoSuchFieldException | IllegalAccessException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Drive in for only one time.
     */
    @Test
    @Timeout(value = 1000, unit = TimeUnit.MILLISECONDS)
    public void test04DriveOutBasic1() {
        parkingLot.minutesPassed(8 * 60);
        parkingLot.driveInto(0, "C0001", "C0002", "C0003", "C0004");
        parkingLot.minutesPassed(45);
        parkingLot.driveInto(1, "B00001", "B00002", "B00003");
        parkingLot.minutesPassed(76);
        parkingLot.driveOut("C0001", "C0002", "C0003");
        assertEquals(75, parkingLot.currentIncome());
        assertEquals("car:1/5 bus:3/3", parkingLot.parkingLotStatus());
        assertEquals("10:06", parkingLot.getTime().toString());
        String[] resultExcept = { "Car C0001 false null",
                "Car C0002 false null",
                "Car C0003 false null",
                "Car C0004 true 08:02",
                "Bus B0001 true 08:03",
                "Bus B0002 true 08:04"};
        try {
            Field vehiclesField = ConcreteParkingLot.class.getDeclaredField("vehicles");
            vehiclesField.setAccessible(true);
            List<Vehicle> field = (List<Vehicle>) vehiclesField.get(parkingLot);
            assertEquals(7, field.size());
            List<String> mapResult = field.stream().map(f -> f.toString() + " " + f.getArriveTime()).toList();
            for (String result : resultExcept) {
                if (!mapResult.contains(result)) {
                    wrongCount++;
                    sb.append(System.lineSeparator()).append(result).append(" is missing!");
                }
            }
            vehiclesField.setAccessible(false);

        } catch (NoSuchFieldException | IllegalAccessException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Drive in for only more than one time.
     */
    @Test
    @Timeout(value = 1500, unit = TimeUnit.MILLISECONDS)
    public void test05DriveOutBasic2() {
        parkingLot.minutesPassed(55);
        parkingLot.driveInto(0, "A00001", "A00002", "A00003", "A00004");
        parkingLot.minutesPassed(28);
        parkingLot.driveOut("A00002", "A00003", "A00004");
        assertEquals(15, parkingLot.currentIncome());
        assertEquals("car:1/5 bus:0/3", parkingLot.parkingLotStatus());
        assertEquals("01:26", parkingLot.getTime().toString());

        parkingLot.minutesPassed(130);
        parkingLot.driveInto(0, "A00002", "A00004", "A00005");
        parkingLot.minutesPassed(10);
        parkingLot.driveInto(1, "C12345", "D12345", "E12345");
        parkingLot.minutesPassed(300);
        parkingLot.driveOut("A00002", "C12345");

        assertEquals(130, parkingLot.currentIncome());
        assertEquals("car:3/5 bus:2/3", parkingLot.parkingLotStatus());
        assertEquals("08:50", parkingLot.getTime().toString());

        String[] resultExcept = {"Car A00001 true 00:55",
                "Car A00002 false null",
                "Car A00003 false null",
                "Car A00004 true 03:37",
                "Car A00005 true 03:38",
                "Bus C12345 false null",
                "Bus D12345 true 03:49"};
        try {
            Field vehiclesField = ConcreteParkingLot.class.getDeclaredField("vehicles");
            vehiclesField.setAccessible(true);
            List<Vehicle> field = (List<Vehicle>) vehiclesField.get(parkingLot);
            assertEquals(8, field.size());
            List<String> mapResult = field.stream().map(f -> f.toString() + " " + f.getArriveTime()).toList();
            for (String result : resultExcept) {
                if (!mapResult.contains(result)) {
                    wrongCount++;
                    sb.append(System.lineSeparator()).append(result).append(" is missing!");
                }
            }
            vehiclesField.setAccessible(false);
        } catch (NoSuchFieldException | IllegalAccessException e) {
            throw new RuntimeException(e);
        }
    }

    @Test
    @Timeout(value = 1500, unit = TimeUnit.MILLISECONDS)
    public void test06getVehicleByNumber() {
        parkingLot.minutesPassed(30);
        parkingLot.driveInto(0, "E00001", "F00002", "A00003", "B00004");
        parkingLot.minutesPassed(150);
        parkingLot.driveInto(1, "H12345", "H54321");
        parkingLot.minutesPassed(280);
        parkingLot.driveOut("E00001", "H54321");
        assertEquals("Bus H12345 true", parkingLot.getVehicleByPlateNumber("H12345").toString());
        assertEquals("Car F00002 true", parkingLot.getVehicleByPlateNumber("F00002").toString());
        assertEquals("Car E00001 false", parkingLot.getVehicleByPlateNumber("E00001").toString());
        assertEquals("Car A00003 true", parkingLot.getVehiclesByNumber().get(0).toString());
        assertEquals("Car B00004 true", parkingLot.getVehiclesByNumber().get(1).toString());
        assertEquals("Car E00001 false", parkingLot.getVehiclesByNumber().get(2).toString());
        assertEquals("Car F00002 true", parkingLot.getVehiclesByNumber().get(3).toString());
        assertEquals("Bus H12345 true", parkingLot.getVehiclesByNumber().get(4).toString());
        assertEquals("Bus H54321 false", parkingLot.getVehiclesByNumber().get(5).toString());
    }

    @Test
    @Timeout(value = 1500, unit = TimeUnit.MILLISECONDS)
    public void test07parkingRecord1() {
        parkingLot.minutesPassed(30);
        parkingLot.driveInto(0, "E00001", "F00002", "A00003", "B00004");
        parkingLot.minutesPassed(150);
        parkingLot.driveInto(1, "H12345", "H54321");
        parkingLot.minutesPassed(280);
        parkingLot.driveOut("E00001", "H54321", "F00002");
        parkingLot.minutesPassed(300);
        parkingLot.driveInto(0, "F00002", "C12345");
        parkingLot.minutesPassed(10);
        parkingLot.driveInto(1, "H54321", "A00001", "A00002");
        parkingLot.minutesPassed(20);
        parkingLot.driveOut("H54321", "A00003", "F00002");
        assertEquals("car:2/5 bus:2/3", parkingLot.parkingLotStatus());
        assertEquals(235, parkingLot.currentIncome());
        assertEquals("13:16", parkingLot.getTime().toString());

        List<String> result1 = parkingLot.getParkingRecordByArriveTime(new Time(8, 0), new Time(14, 0));
        List<String> result2 = parkingLot.getParkingRecordByArriveTime(new Time(8, 0), new Time(14, 0));
        assertEquals("Car F00002 12:44 13:16",result1.get(0));
        assertEquals("Car C12345 12:45 null",result1.get(1));
        assertEquals("Bus H54321 12:55 13:16",result1.get(2));
        assertEquals("Car F00002 12:44 13:16",result2.get(0));
        assertEquals("Car C12345 12:45 null",result2.get(1));
    }
}