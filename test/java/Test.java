public class Test {
    private final int a = 1;

    public int calc(int b) {
        return b + a;
    }

    public static void main(String[] args) {
        Test test = new Test();
        int  b = 3;
        long c = 5;
        long d = 7;
        System.out.println(test.calc(b));
        System.out.println(c + d);
    }
}
