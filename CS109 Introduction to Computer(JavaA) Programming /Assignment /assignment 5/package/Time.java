public class Time  {
    private int hour;
    private int minute;

    public Time(int hour, int minute) {
        this.hour = hour;
        this.minute = minute;
    }

    public int changeToMinutes() {
        return this.hour * 60 + minute;
    }

    public void addMinutes(int minutes) {
        int totalMinutes = changeToMinutes() + minutes;
        this.hour = totalMinutes / 60;
        this.minute = totalMinutes % 60;
    }
    @Override
    public String toString() {
        return String.format("%02d:%02d", hour, minute);
    }

}
