import java.util.List;

public interface ParkingLot {
    public Time getTime();

    public void minutesPassed(int minutes);

    public String parkingLotStatus();

    public void driveInto(int type, String... plateNumbers);

    public void driveOut(String ... plateNumbers);

    public Vehicle getVehicleByPlateNumber(String plateNumber);

    public List<Vehicle> getVehiclesByNumber();

    public List<String> getParkingRecordByArriveTime(Time start, Time end);

    public int currentIncome();
}
