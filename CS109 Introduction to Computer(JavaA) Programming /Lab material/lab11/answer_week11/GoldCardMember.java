import java.util.InputMismatchException;

public class GoldCardMember extends Member {
    public GoldCardMember(String info) {
        super(info);
    }

    @Override
    public double consume(int amount) {
        double cost = 0;
        if (amount > 20000) {
            cost = (amount - 20000) * 0.8;
            cost += 17850;
            totalDiscountPrice += ((amount - 20000) * 0.2 + 2150);
        } else if (amount > 10000) {
            cost = (amount - 10000) * 0.85;
            cost += 9350;
            totalDiscountPrice += ((amount - 10000) * 0.15 + 650);
        } else if (amount > 5000) {
            cost = (amount - 5000) * 0.9;
            cost += 4850;
            totalDiscountPrice += ((amount - 5000) * 0.1 + 150);
        } else if (amount > 2000) {
            cost = (amount - 2000) * 0.95;
            cost += 2000;
            totalDiscountPrice += (amount - 2000) * 0.05;
        } else if (amount > 0) {
            cost = amount;
        } else {
            //这里不要测试，给出的数据都要大于0
            throw new InputMismatchException("amount should > 0:" + amount);
        }
        totalCost += cost;
        return cost;
    }

    @Override
    public String toString() {
        return String.format("GoldCardMember: %s",super.toString());
    }
}
