import java.util.Scanner;

public class output {

public static void main(String[] args) { 
 Integer counter = 1; 
 System.out.print("Ingrese un numero");
 System.out.print(System.lineSeparator());
 Integer num = 0; 
 Scanner _v_num_s_ = new Scanner(System.in); String _v_num_s_aux_ = _v_num_s_.next(); try { num = Integer.parseInt(_v_num_s_aux_); } catch (Exception e) { System.out.println("Compilation Ended"); return; }
 if ( num < 10 ){ 
 System.out.print("Es menor que diez");
 } 
 if ( 10 < 5 || num == 8
 ){ 
 System.out.print("Funciona");
 } 
 while ( num < counter ) { 
 System.out.print(counter);
 System.out.print(" ");
 counter = (counter + 1);
 }
 }

}

