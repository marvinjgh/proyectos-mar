/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Marvin
 */
/*
 * PruebaJScrollPane.java
 *
 * Created on 15 de abril de 2005, 17:26
 */

import java.util.Locale;
import javax.swing.*;
/**
 * @author  Chuidiang
 *
 * Ejemplo de uso del JScrollPane. Se mete dentro un JLabel con una imagen.
 */
public class Prueba {

    /** Creates a new instance of PruebaJScrollPane */
    public Prueba() {

        // La ventana
        JFrame ventana = new JFrame("Imagen");

        // El panel de scroll
        JScrollPane scroll = new JScrollPane();

        // La etiqueta.
        JLabel etiqueta = new JLabel();

        // Se carga la imagen, con path absoluto para evitar problemas y debe
        // ser un gif.
        Icon imagen = new ImageIcon (
            "checkboard.png");

        // Se mete la imagen en el label
        etiqueta.setIcon (imagen);

        // Se mete el scroll en la ventana
        
        scroll.setSize(100, 100);
        ventana.getContentPane().add(scroll);
        // Se mete el label en el scroll
        scroll.setViewportView(etiqueta);

        // Y se visualiza todo.
        //ventana.pack();
        ventana.setSize(400, 400);
        ventana.setVisible(true);
    }

    /**
     * Programa principal. Instancia un PruebaJScrollPane
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        new Prueba();
    }

}