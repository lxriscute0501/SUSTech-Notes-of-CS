import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class ConcreteParkingLot implements ParkingLot {
    private List<Vehicle> vehicles;
    private List<ParkingRecord> parkingRecords;
    private Time currentTime;
    private final int CAR_CAPACITY;
    private final int BUS_CAPACITY;

    private int carCount;
    private int busCount;
    private int totalIncome;

    public ConcreteParkingLot(int carCapacity, int busCapacity) {
        this.vehicles = new ArrayList<>();
        this.parkingRecords = new ArrayList<>();
        this.CAR_CAPACITY = carCapacity;
        this.BUS_CAPACITY = busCapacity;
        this.carCount = 0;
        this.busCount = 0;
        this.totalIncome = 0;
        this.currentTime = new Time(0, 0);
    }

    @Override
    public Time getTime() {
        return this.currentTime;
    }

    @Override
    public void minutesPassed(int minutes) {
        currentTime.addMinutes(minutes);
    }

    @Override
    public String parkingLotStatus() {
        return String.format("car:%d/%d bus:%d/%d", carCount, CAR_CAPACITY, busCount, BUS_CAPACITY);
    }

    @Override
    public void driveInto(int type, String... plateNumbers) {
        int validLength = 0;
        if (type == 0) {
            if (CAR_CAPACITY > carCount) {
                validLength = Math.min(plateNumbers.length, CAR_CAPACITY - carCount);
                carCount += validLength;
            }
        } else {
            if (BUS_CAPACITY > busCount) {
                validLength = Math.min(plateNumbers.length, BUS_CAPACITY - busCount);
                busCount += validLength;
            }
        }

        for (int i = 0; i < validLength; i++) {
            Vehicle vehicle = null;
            for (Vehicle v : vehicles) {
                if (v.plateNumber.equals(plateNumbers[i])) {
                    vehicle = v;
                }
            }
            if (vehicle == null) {
                if (type == 0) {
                    vehicle = new Car(plateNumbers[i]);

                } else {
                    vehicle = new Bus(plateNumbers[i]);
                }
                this.vehicles.add(vehicle);
            }
            if(i>0) {
                this.currentTime.addMinutes(1);
            }
            vehicle.setArriveTime(new Time(this.currentTime.getHour(), this.currentTime.getMinute()));
            this.parkingRecords.add(new ParkingRecord(new Time(this.currentTime.getHour(), this.currentTime.getMinute()), type, plateNumbers[i]));
        }

    }

    @Override
    public void driveOut(String... plateNumbers) {
        for (String number : plateNumbers) {
            for (Vehicle v : vehicles) {
                if (number.equals(v.plateNumber)) {
                    this.totalIncome += v.calculateMoney(currentTime);
                    if (v instanceof Car) {
                        carCount--;
                    } else {
                        busCount--;
                    }
                }
            }
            for (ParkingRecord record : parkingRecords) {
                if (record.plateNumber.equals(number)) {
                    record.setLeaveTime(new Time(currentTime.getHour(),currentTime.getMinute()));
                }
            }
        }
    }

    @Override
    public Vehicle getVehicleByPlateNumber(String plateNumber) {
        for (Vehicle v : vehicles) {
            if (v.plateNumber.equals(plateNumber)) {
                return v;
            }
        }
        return null;
    }

    @Override
    public List<Vehicle> getVehiclesByNumber() {
        Collections.sort(vehicles);
        return this.vehicles;
    }

    @Override
    public List<String> getParkingRecordByArriveTime(Time start, Time end) {
        List<String> results = new ArrayList<>();
        for (ParkingRecord record : parkingRecords) {
            if (record.getArriveTime().changeToMinutes() >= start.changeToMinutes() &&
                    record.getArriveTime().changeToMinutes() <= end.changeToMinutes()){
                results.add(record.toString());
            }
        }
        return results;
    }


    @Override
    public int currentIncome() {
        return totalIncome;
    }
}

class ParkingRecord implements Comparable<ParkingRecord> {
    Time arriveTime;
    Time leaveTime;
    int type;
    String plateNumber;

    public ParkingRecord(Time arriveTime, int type, String plateNumber) {
        this.arriveTime = arriveTime;
        this.type = type;
        this.plateNumber = plateNumber;
        this.leaveTime = null;
    }

    public Time getArriveTime() {
        return arriveTime;
    }

    public void setArriveTime(Time arriveTime) {
        this.arriveTime = arriveTime;
    }

    public Time getLeaveTime() {
        return leaveTime;
    }

    public String toString() {
        return String.format("%s %s %s %s", type == 0 ? "Car" : "Bus", plateNumber, arriveTime, leaveTime);
    }

    public void setLeaveTime(Time time) {
        this.leaveTime = time;
    }

    @Override
    public int compareTo(ParkingRecord o) {
        return arriveTime.changeToMinutes() - o.arriveTime.changeToMinutes();
    }
}