public class Factorial {
    public String calculationHistory = "";
    public int result = 1;
    public void calculate(int max) {
        result *= max;
        calculationHistory = Integer.toString(max) + "*" + calculationHistory;
        if (max > 1) {
            calculate(max - 1);
        }
    }
}