import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;

public class spring25A6LocalTest {
    private ParkingLot parkingLot;
    private StringBuilder sb;
    private int wrongCount;

    @BeforeEach
    public void prepareData() {
        parkingLot = new ConcreteParkingLot(4, 2);
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
    public void test02Time() {
        parkingLot.minutesPassed(30);
        assertEquals("00:30", parkingLot.getTime().toString());
        parkingLot.minutesPassed(45);
        assertEquals("01:15", parkingLot.getTime().toString());
    }

    @Test
    public void test03DriveIntoBasic() {
        parkingLot.minutesPassed(55);
        parkingLot.driveInto(0, "A00001", "A00002", "A00003", "A00004");
        parkingLot.minutesPassed(30);
        parkingLot.driveInto(1, "B00001", "B00002", "B00003");
        assertEquals("car:4/4 bus:2/2", parkingLot.parkingLotStatus());
        String[] resultExcept = {"A00001 00:55",
                "A00002 00:56",
                "A00003 00:57",
                "A00004 00:58",
                "B00001 01:28",
                "B00002 01:29"};
        try {
            Field vehiclesField = ConcreteParkingLot.class.getDeclaredField("vehicles");
            vehiclesField.setAccessible(true);
            List<Vehicle> field = (List<Vehicle>) vehiclesField.get(parkingLot);
            assertEquals(6, field.size());
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
    public void test04DriveOutBasic1() {
        parkingLot.minutesPassed(55);
        parkingLot.driveInto(0, "A00001", "A00002", "A00003", "A00004");
        parkingLot.minutesPassed(30);
        parkingLot.driveInto(1, "B00001", "B00002", "B00003");
        parkingLot.minutesPassed(29);
        parkingLot.driveOut("B00001", "B00002", "A00003");
        assertEquals(35, parkingLot.currentIncome());
        assertEquals("car:3/4 bus:0/2", parkingLot.parkingLotStatus());
        assertEquals("01:58", parkingLot.getTime().toString());
        String[] resultExcept = {"Car A00001 true 00:55",
                "Car A00002 true 00:56",
                "Car A00003 false null",
                "Car A00004 true 00:58",
                "Bus B00001 false null",
                "Bus B00002 false null"};
        try {
            Field vehiclesField = ConcreteParkingLot.class.getDeclaredField("vehicles");
            vehiclesField.setAccessible(true);
            List<Vehicle> field = (List<Vehicle>) vehiclesField.get(parkingLot);
            assertEquals(6, field.size());
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
    public void test05DriveOutBasic2() {
        parkingLot.minutesPassed(55);
        parkingLot.driveInto(0, "A00001", "A00002", "A00003", "A00004");
        parkingLot.minutesPassed(28);
        parkingLot.driveOut("A00002", "A00003", "A00004");
        assertEquals(15, parkingLot.currentIncome());
        assertEquals("car:1/4 bus:0/2", parkingLot.parkingLotStatus());
        assertEquals("01:26", parkingLot.getTime().toString());

        parkingLot.minutesPassed(130);
        parkingLot.driveInto(0, "A00002", "A00004", "A00005");
        parkingLot.minutesPassed(10);
        parkingLot.driveInto(1, "C12345", "D12345", "E12345");
        parkingLot.minutesPassed(300);
        parkingLot.driveOut("A00002", "C12345");

        assertEquals(130, parkingLot.currentIncome());
        assertEquals("car:3/4 bus:1/2", parkingLot.parkingLotStatus());
        assertEquals("08:49", parkingLot.getTime().toString());

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

    @Test
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
        assertEquals("car:2/4 bus:1/2", parkingLot.parkingLotStatus());
        assertEquals(235, parkingLot.currentIncome());
        assertEquals("13:15", parkingLot.getTime().toString());

        List<String> result1 = parkingLot.getParkingRecordByArriveTime(new Time(8, 0), new Time(14, 0));
        List<String> result2 = parkingLot.getParkingRecordByArriveTime(new Time(8, 0), new Time(14, 0));
        assertEquals("Car F00002 12:44 13:15",result1.get(0));
        assertEquals("Car C12345 12:45 null",result1.get(1));
        assertEquals("Bus H54321 12:55 13:15",result1.get(2));
        assertEquals("Car F00002 12:44 13:15",result2.get(0));
        assertEquals("Car C12345 12:45 null",result2.get(1));
    }
}
