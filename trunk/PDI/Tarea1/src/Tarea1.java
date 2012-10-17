
import interfaz.Interfaz;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import javax.swing.UnsupportedLookAndFeelException;


/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
/**
 *
 * @author Marvin
 */
public class Tarea1 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws IOException {
        
        try {
            javax.swing.UIManager.setLookAndFeel("com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel");
        } catch (ClassNotFoundException | InstantiationException | IllegalAccessException | UnsupportedLookAndFeelException ex) {
            try {
                JOptionPane.showMessageDialog(null, "Tema principal no soportado pasando al tema Metal");
                javax.swing.UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
            } catch (    ClassNotFoundException | InstantiationException | IllegalAccessException | UnsupportedLookAndFeelException ex1) {
                Logger.getLogger(Tarea1.class.getName()).log(Level.SEVERE, null, ex1);
            }
        }
        Interfaz I= new Interfaz();
        I.setVisible(true);
        I.setLocationRelativeTo(null);
    }

}
