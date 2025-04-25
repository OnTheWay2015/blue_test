import test_package.tpackage;
import test_servlet.stest_ser_main;

public class helloworld {

    public static void main(String[] args){
        test_class a = new test_class(1,2);
        a.Run(args);

        /*  todo err
        stest_ser_main s = new stest_ser_main();
         try {
             s.run(args);
         }catch (Exception e){

         }
        */
        tpackage b = new tpackage();
        b.run();
    }


}
