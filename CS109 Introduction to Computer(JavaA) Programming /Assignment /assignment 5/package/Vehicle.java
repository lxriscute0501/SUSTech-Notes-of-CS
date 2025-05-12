public abstract class Vehicle {
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

    public abstract int calculateMoney(Time leaveTime);

}
