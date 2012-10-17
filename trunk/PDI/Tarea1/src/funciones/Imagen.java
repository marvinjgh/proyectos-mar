package funciones;

import java.awt.image.BufferedImage;
import java.awt.image.Raster;
import java.awt.image.WritableRaster;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;

/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
/**
 *
 * @author Marvin
 */
public class Imagen {
    
    /**
     * Constructor de Imagen apartir de un BufferedImage
     * @param original Imagen original dentro un BufferedImage
     */
    public Imagen(BufferedImage original) {
        Raster rasterImagenOriginal = original.getData();
        width = original.getWidth();
        height = original.getHeight();
        type = (byte) original.getType();

        hred = new long[256];
        hgreen = new long[256];
        hblue = new long[256];
        bpp = (byte) original.getColorModel().getPixelSize();
        red = new short[width][height];
        green = new short[width][height];
        blue = new short[width][height];
        alpha = new short[width][height];
        echist = new short[3][256];

        Arrays.fill(hred, 0);
        Arrays.fill(hgreen, 0);
        Arrays.fill(hblue, 0);

        int aux;

        if (true) {// if para cuando sera gris, rgb y argb 
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {

                    aux = rasterImagenOriginal.getSample(i, j, 0);
                    red[i][j] = (short) aux;
                    hred[aux]++;
                    if (this.bpp > 8) {
                        aux = rasterImagenOriginal.getSample(i, j, 1);
                        green[i][j] = (short) aux;
                        hgreen[aux]++;

                        aux = rasterImagenOriginal.getSample(i, j, 2);
                        blue[i][j] = (short) aux;
                        hblue[aux]++;

                        if (this.bpp == 32) {
                            aux = rasterImagenOriginal.getSample(i, j, 3);
                            alpha[i][j] = (short) aux;
                        }
                    }

                }
            }

        }
        cube = new cubo();
    }
   
    private void updateImage(){
        Arrays.fill(hred, (byte) 0);
        Arrays.fill(hgreen, (byte) 0);
        Arrays.fill(hblue, (byte) 0);

        int aux;
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                red[i][j] = echist[0][red[i][j]];
                hred[red[i][j]]++;

                if (this.bpp > 8) {
                    green[i][j] = echist[0][green[i][j]];
                    hgreen[green[i][j]]++;

                    blue[i][j] = echist[0][blue[i][j]];
                    hblue[blue[i][j]]++;
                }
            }
        }
    }
    
    public BufferedImage imagenfinal() {

        BufferedImage salida = new BufferedImage(width, height, type);
        WritableRaster rasterSalida = salida.getRaster();

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                rasterSalida.setSample(i, j, 0, red[i][j]);
                if (this.bpp > 8) {
                    rasterSalida.setSample(i, j, 1, green[i][j]);
                    rasterSalida.setSample(i, j, 2, blue[i][j]);
                    if (this.bpp == 32) {
                        rasterSalida.setSample(i, j, 3, alpha[i][j]);
                    }
                }
            }
        }
        return salida;
    }

    public void ConverToGray() {
        if (bpp > 8) {
            float aux;
            Arrays.fill(hred, (byte) 0);
            Arrays.fill(hgreen, (byte) 0);
            Arrays.fill(hblue, (byte) 0);
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    aux = (float) (red[i][j] * 0.3 + green[i][j] * 0.59 + blue[i][j] * 0.11);
                    red[i][j] = (short) aux;
                    green[i][j] = (short) aux;
                    blue[i][j] = (short) aux;
                    hred[(int) aux]++;
                    hgreen[(int) aux]++;
                    hblue[(int) aux]++;
                }
            }
        }

    }

    public int getWidthImagen() {
        return this.width;
    }

    public int getHeightImagen() {
        return this.height;
    }

    public int getTypeImagen() {
        return this.type;
    }
    
    public void ecualizar() {
        
        double scale_factor = (float) (255.0 / (width * height));
        long sumr = 0, sumg = 0, sumb = 0;
        int baba1;
        for (int i = 0; i < 256; i++) {
            sumr += hred[i];
            baba1 = (int) (sumr * scale_factor);
            echist[0][i] = (baba1 > 255) ? 255 : (short) baba1;

            sumg += hgreen[i];
            baba1 = (int) (sumg * scale_factor);
            echist[1][i] = (baba1 > 255) ? 255 : (short) baba1;

            sumb += hblue[i];
            baba1 = (int) (sumb * scale_factor);
            echist[2][i] = (baba1 > 255) ? 255 : (short) baba1;
        }
        updateImage();
    }
    
    
    //PENDIENTEEEEEEEE
    //NO PUEDO GUARDAR ESTA IMAGEN COMO IMAGEN ORIGINAL.
    public void escalarMenos() {
        
        double scale_factor = (float) (255.0 / (width * height));
        long sumr = 0, sumg = 0, sumb = 0;
        int baba1 = 0;
        for (int i = 0; i < 256; i++) {
            sumr += hred[i];
            baba1 = (int) (sumr * scale_factor );
            echist[0][i] = (baba1 > 255) ? 255 : (short)baba1;
            
            sumg += hgreen[i];
            baba1 = (int) (sumg * scale_factor );
            echist[1][i] = (baba1 > 255) ? 255 : (short)baba1;

            sumb += hblue[i] ;
            baba1 = (int) (sumb * scale_factor );
            echist[2][i] = (baba1 > 255) ? 255 : (short)baba1;
            System.out.println(baba1);
        }
        for (int i = 0; i < 256; i++) {
            System.out.println(echist[0][i]);
        }
        //updateImage();
    }
    
    public void converTo8bo16b(int b) {
        
        if (bpp > 8){
        cube.cubode(b);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                cube.ubicarpixel(red[i][j], green[i][j], blue[i][j]);
            }
        }
        int c[];
        cube.fijarcentros();
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                c = cube.getColor(red[i][j], green[i][j], blue[i][j]);
                red[i][j] = (short) c[0];
                green[i][j] = (short) c[1];
                blue[i][j] = (short) c[2];
            }
        }
        }else
            javax.swing.JOptionPane.showMessageDialog(null, "debe usar una imagen de 24 o 32 bpp");
    }

    public byte getBpp() {
        return bpp;
    }
    
    
    private byte bpp;
    private byte type;
    public short red[][];
    public short green[][];
    public short blue[][];
    public short alpha[][];
    public short echist[][];
    public long hred[];
    public long hgreen[];
    public long hblue[];
    private int width;
    private int height;
    private cubo cube;
}
