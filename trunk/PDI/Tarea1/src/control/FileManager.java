package control;

import interfaz.Interfaz;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;
import javax.swing.JFileChooser;
import javax.swing.filechooser.FileNameExtensionFilter;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.filechooser.FileNameExtensionFilter;

/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
/**
 *
 * @author Marvin
 */
public class FileManager {

    /**
     *
     * @return Un elemento de tipo File que contiene la imagen o un null
     */
    public File cargarImagen(Interfaz I) {

        JFileChooser jFileChooser = new JFileChooser();
        FileNameExtensionFilter filter = new FileNameExtensionFilter("Imagenes JPEG, PNG, BMP", "jpg", "png", "jpeg", "bmp");
        File file = null;

        jFileChooser.setDialogType(JFileChooser.OPEN_DIALOG);
        jFileChooser.setAcceptAllFileFilterUsed(false);
        jFileChooser.setFileFilter(filter);

        int seleccion = jFileChooser.showDialog(I, null);

        if (seleccion == JFileChooser.APPROVE_OPTION) {
            file = jFileChooser.getSelectedFile();
        } else if (seleccion == JFileChooser.ERROR_OPTION) {
            JOptionPane.showMessageDialog(I, "Error abriendo el archivo");
        }
        return file;
    }

    public void guardarImagen(Interfaz I, BufferedImage imagen) {

        JFileChooser jFileChooser = new JFileChooser();

        FileNameExtensionFilter filterjpg = new FileNameExtensionFilter("Imagenes JPEG", "jpg", "jpeg");
        FileNameExtensionFilter filterpng = new FileNameExtensionFilter("Imagenes PNG", "png");
        FileNameExtensionFilter filterbmp = new FileNameExtensionFilter("Imagenes BMP", "bmp");

        File file = null;
        jFileChooser.setDialogType(JFileChooser.SAVE_DIALOG);
        jFileChooser.setAcceptAllFileFilterUsed(false);

        jFileChooser.addChoosableFileFilter(filterjpg);
        jFileChooser.addChoosableFileFilter(filterpng);
        jFileChooser.addChoosableFileFilter(filterbmp);

        int seleccion = jFileChooser.showDialog(I, null);
        String fil = jFileChooser.getFileFilter().getDescription();
        String tipo = null;
        if (seleccion == JFileChooser.APPROVE_OPTION) {
            if (fil.equals("Imagenes JPEG")) {
                file = new File(jFileChooser.getSelectedFile().getPath() + ".jpg");
                tipo = "JPEG";
            }
            if (fil.equals("Imagenes PNG")) {
                file = new File(jFileChooser.getSelectedFile().getPath() + ".png");
                tipo = "PNG";
            }
            if (fil.equals("Imagenes BMP")) {
                file = new File(jFileChooser.getSelectedFile().getPath() + ".bmp");
                tipo = "BMP";
            }
            try {
                ImageIO.write(imagen, tipo, file);

            } catch (IOException ex) {
                JOptionPane.showMessageDialog(I, "Error guardando el archivo");

            }
        } else if (seleccion == JFileChooser.ERROR_OPTION) {
            JOptionPane.showMessageDialog(I, "Error abriendo el archivo");
        }

        // 

    }
}
