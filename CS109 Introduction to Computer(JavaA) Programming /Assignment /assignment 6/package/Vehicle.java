public abstract class Vehicle implements Comparable<Vehicle> {
    protected String plateNumber;
    protected boolean isInside;
    protected Time arriveTime;

    protected int previousTime;
    protected int previousPrice;

    public Vehicle(String plateNumber) {
        this.plateNumber = plateNumber;
    }

    public Time getArriveTime() {
        return arriveTime;
    }

    public void setArriveTime(Time arriveTime) {
        this.arriveTime = arriveTime;
        this.isInside = true;
    }

    @Override
    public int compareTo(Vehicle v) {
        return this.plateNumber.compareTo(v.plateNumber);
    }

    public abstract int calculateMoney(Time departTime);

}
