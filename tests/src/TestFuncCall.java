public class TestFuncCall {
    public static void print(int a) {
        System.out.println(Integer.toString(a));
    }

    public static void main(String[] args) {
        print(Add.add(7, 8));
    }
}