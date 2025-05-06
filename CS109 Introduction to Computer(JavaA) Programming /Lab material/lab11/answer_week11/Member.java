public abstract class Member {
    private String memberId;
    private char gender;
    private int age;

    protected double totalCost = 0;//实际消费
    protected double totalDiscountPrice = 0;

    public Member(String info) {
        this.memberId = info.split(" ")[0];
        this.gender = info.split(" ")[1].charAt(0);
        this.age = Integer.parseInt(info.split(" ")[2]);
    }

    public abstract double consume(int amount);

    public double getTotalCost() {
        return totalCost;
    }

    public double getTotalDiscountPrice() {
        return totalDiscountPrice;
    }

    @Override
    public String toString() {
        return String.format("%s %c %d", this.memberId, this.gender, this.age);
    }

    public String getMemberId() {
        return memberId;
    }

    public char getGender() {
        return gender;
    }

    public int getAge() {
        return age;
    }

    /**
     *
     * @return 一个String类型包含： memberId 性别 年龄 总共实际支付（保留1位小数）。 中间以空格间隔
     */
    public String getGenderAgeCost() {
        return String.format("%s %c %d %.1f ", memberId, gender, age, totalCost);
    }
}
