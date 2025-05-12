public class Car extends Vehicle {
    private final int START_PRICE = 15;
    private final int INCREMENT_PRICE = 5;
    private final int MAX_PRICE = 60;

    public Car(String plateNumber) {
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
                if (duration < 60) {
                    current_price = START_PRICE - previousPrice;
                    previousPrice = START_PRICE;
                } else {
                    int min_price = Math.min(START_PRICE + INCREMENT_PRICE * (duration / 60), MAX_PRICE);
                    current_price = min_price - previousPrice;
                    previousPrice = min_price;
                }
                previousTime = duration;
            }
            arriveTime = null;
            isInside = false;
            return current_price;
        }
    }

    @Override
    public String toString() {
        return String.format("%s %s %s", "Car", plateNumber, isInside);
    }
}
