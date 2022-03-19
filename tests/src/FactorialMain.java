public class FactorialMain {
    public static void main(String[] args) {
        Factorial f = new Factorial();
        f.calculate(10);
        System.out.println(f.calculationHistory + "=" + Integer.toString(f.result));
    }
}