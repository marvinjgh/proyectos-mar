/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Marvin
 */
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.WindowConstants;

/**
 * Ejemplo para guardar una imagen en un fichero desde java. Código original de
 * webo_cs
 */
public class Guardar {

	/**
	 * Main de ejemplo. Crea un BufferedImage, hace un dibujito y lo guarda en
	 * un fichero jpg.
	 *
	 * @param args
	 */
	public static void main(String[] args) {
		JFrame unaVentana = new JFrame("la ventana");
		unaVentana.getContentPane().setLayout(new FlowLayout());
		unaVentana.getContentPane().add(new JLabel("etiqueta"));
		unaVentana.getContentPane().add(new JButton("boton"));
		unaVentana.getContentPane().add(new JTextField(20));
		unaVentana.pack();
		unaVentana.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
		unaVentana.setVisible(true);

		File fichero = new File("foto.jpg");
		String formato = "jpg";

		// Creamos la imagen para dibujar en ella.
		BufferedImage imagen = new BufferedImage(unaVentana.getContentPane().getWidth(),
				unaVentana.getContentPane().getHeight(), BufferedImage.TYPE_INT_RGB);

		// Hacemos el dibujo
		Graphics g = imagen.getGraphics();
		unaVentana.getContentPane().paint(g);

		// Escribimos la imagen en el archivo.
		try {
			ImageIO.write(imagen, formato, fichero);
		} catch (IOException e) {
			System.out.println("Error de escritura");
		}
	}

}