import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;

import java.util.concurrent.TimeUnit;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class Spring25A6OverFlowTest {
    private ConcreteParkingLot parkingLot;

    @BeforeEach
    void setUp() {
        parkingLot = new ConcreteParkingLot(2, 1);
    }

    @Test
    @Timeout(value = 1000, unit = TimeUnit.MILLISECONDS)
    void testCarOverflow() {
        parkingLot.driveInto(0, "A11111", "A11112");
        parkingLot.driveInto(0, "A11113");
        assertNotNull(parkingLot.getVehicleByPlateNumber("A11111"));
        assertNotNull(parkingLot.getVehicleByPlateNumber("A11112"));
        assertNull(parkingLot.getVehicleByPlateNumber("A11113"), "A11113 should not be parked");
        assertEquals("car:2/2 bus:0/1", parkingLot.parkingLotStatus());
    }

    @Test
    @Timeout(value = 1000, unit = TimeUnit.MILLISECONDS)
    void testBusOverflow() {
        parkingLot.driveInto(1, "B11111");
        parkingLot.driveInto(1, "B11112");
        assertNotNull(parkingLot.getVehicleByPlateNumber("B11111"));
        assertNull(parkingLot.getVehicleByPlateNumber("B11112"), "B11112 should not be parked");
        assertEquals("car:0/2 bus:1/1", parkingLot.parkingLotStatus());
    }

    @Test
    @Timeout(value = 1000, unit = TimeUnit.MILLISECONDS)
    void testCarOverflowBatch() {
        ConcreteParkingLot lot = new ConcreteParkingLot(3, 2);
        lot.driveInto(0, "A001", "A002", "A003", "A004", "A005");

        assertNotNull(lot.getVehicleByPlateNumber("A001"));
        assertNotNull(lot.getVehicleByPlateNumber("A002"));
        assertNotNull(lot.getVehicleByPlateNumber("A003"));
        assertNull(lot.getVehicleByPlateNumber("A004"));
        assertNull(lot.getVehicleByPlateNumber("A005"));

        assertEquals("car:3/3 bus:0/2", lot.parkingLotStatus());
    }

    @Test
    @Timeout(value = 1000, unit = TimeUnit.MILLISECONDS)
    void testRepeatedCarEntry() {
        ConcreteParkingLot lot = new ConcreteParkingLot(2, 1);
        lot.driveInto(0, "R001");
        lot.minutesPassed(30);
        lot.driveOut("R001");
        lot.minutesPassed(10);
        lot.driveInto(0, "R001");
        assertNotNull(lot.getVehicleByPlateNumber("R001"));
        assertEquals("car:1/2 bus:0/1", lot.parkingLotStatus());
    }

    @Test
    @Timeout(value = 1000, unit = TimeUnit.MILLISECONDS)
    void testTimeAdvanceOnOverflowAttempt() {
        ConcreteParkingLot lot = new ConcreteParkingLot(1, 1);
        lot.minutesPassed(10);
        lot.driveInto(0, "C001", "C002", "C003");
        assertEquals("00:10", lot.getVehicleByPlateNumber("C001").getArriveTime().toString());
        assertNull(lot.getVehicleByPlateNumber("C002"));
        assertEquals("00:10", lot.getTime().toString());
    }
}
