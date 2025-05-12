public class Bus extends Vehicle {
    private final int INCREMENT_PRICE = 15;
    private final int MAX_PRICE = 100;

    public Bus(String plateNumber) {
        super(plateNumber);
    }

    @Override
    public int calculateMoney(Time leaveTime) {
        if (arriveTime == null) {
            return 0;
        } else {
            int duration = leaveTime.changeToMinutes() - arriveTime.changeToMinutes();
            int current_price;
            if (duration < 30) {
                current_price = 0;
            } else {
                duration += previousTime;
                int min_price = Math.min(INCREMENT_PRICE * (duration / 60 + 1), MAX_PRICE);
                current_price = min_price - previousPrice;
                previousPrice = min_price;
                previousTime = duration;
            }
            arriveTime = null;
            isInside = false;
            return current_price;
        }
    }

    @Override
    public String toString() {
        return String.format("%s %s %s", "Bus", plateNumber, isInside);
    }
}
