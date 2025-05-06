public class SilverCardMember extends Member {
    private double points;
    private double rate;

    public SilverCardMember(String info) {
        super(info);
        this.rate = 1;
    }

    @Override
    public double consume(int amount) {
        double cost;
        if (amount >= points) {
            totalDiscountPrice += points;//总共优惠 += 本次优惠积分
            cost = amount - points;// 实际消费=原价-积分
            points = 0;//将积分设置成0
        } else {
            totalDiscountPrice += amount;// 优惠积分没用完： 总共优惠+=消费金额
            cost = 0;//本单免费
            points -= amount;//从积分中扣除消费金额
        }
        totalCost += cost;
        points += (amount / 30) * rate;
        if (totalCost > 10000) {
            rate = 1.5;
        }
        return cost;
    }

    @Override
    public String toString() {
        return String.format("SilverCardMember: %s points=%.1f", super.toString(), points);
    }
}
