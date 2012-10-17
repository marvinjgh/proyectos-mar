package funciones;

import java.awt.image.BufferedImage;
import java.awt.image.Raster;
import java.awt.image.WritableRaster;
import java.util.Arrays;

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
     *
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

    private void updateImage() {
        Arrays.fill(hred, (byte) 0);
        Arrays.fill(hgreen, (byte) 0);
        Arrays.fill(hblue, (byte) 0);

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
                    if (this.bpp == 32) {
                        alpha[i][j] = (short) 255;
                    }

                }
            }
            bpp = 8;
            type = BufferedImage.TYPE_BYTE_GRAY;
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
            System.out.println(baba1);
        }
        for (int i = 0; i < 256; i++) {
            System.out.println(echist[0][i]);
        }
        //updateImage();
    }

    public void converTo8bo16b(int b) {

        if (bpp > 8) {
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
        } else {
            javax.swing.JOptionPane.showMessageDialog(null, "debe usar una imagen de 24 o 32 bpp");
        }
    }

    public byte getBpp() {
        return bpp;
    }

    public void operarMatriz(double[][] matriz, int tipo, double divisor) {

        // DESENFOQUE
        // double [][] matriz = {{0.11,0.11,0.11},{0.11,0.11,0.11},{0.11,0.11,0.11}};
        // double [][] matriz = {{(double)1/9,(double)1/9,(double)1/9},{(double)1/9,(double)1/9,(double)1/9},{(double)1/9,(double)1/9,(double)1/9}};
        // NPI  
        //double [][] matriz = {{(double)0,(double)-1,(double)0},{(double)-1,(double)5,(double)-1},{(double)0,(double)-1,(double)0}};
        // SUAVIZADO VERTICAL
        //double [][] matriz = {{(double)0,(double)1/3,(double)0},{(double)0,(double)1/3,(double)0},{(double)0,(double)1/3,(double)0}};
        // SUAVIZADO HORIZONTAL
        //double [][] matriz = {{(double)0,(double)0,(double)0},{(double)1/3,(double)1/3,(double)1/3},{(double)0,(double)0,(double)0}};
        // ??FILTRO MENOS MEDIA
        //double [][] matriz = {{(double)-1/9,(double)-1/9,(double)-1/9},{(double)-1/9,(double)8/9,(double)-1/9},{(double)-1/9,(double)-1/9,(double)-1/9}};
        // FILTRO MEDIA PONDERADA
        //double [][] matriz = {{(double)1/10,(double)1/10,(double)1/10},{(double)1/10,(double)2/10,(double)1/10},{(double)1/10,(double)1/10,(double)1/10}};
        //RESTA PIXEL DE LA IZQUIERDA
        //  double [][] matriz = {{(double)0,(double)0,(double)0},{(double)0,(double)-1,(double)1},{(double)0,(double)0,(double)0}};
        //RESTA PIXEL DE LA DERECHA
        //double [][] matriz = {{(double)0,(double)0,(double)0},{(double)0,(double)1,(double)-1},{(double)0,(double)0,(double)0}};


        double aux1 = 0, aux2 = 0, aux3 = 0;
        int fmin = 0, cmin = 0, fmax = 0, cmax = 0;
        boolean procesar = true;

        short[][] red = new short[width][height], green = null, blue = null;
        long[] hred = new long[256], hgreen = null, hblue = null;

        if (bpp > 8) {
            green = new short[width][height];
            hgreen = new long[256];
            blue = new short[width][height];
            hblue = new long[256];
        }

        if (tipo == 3) {
            fmin = -1;
            fmax = 1;
            cmin = -1;
            cmax = 1;
        }
        if (tipo == 5) {
            fmin = -2;
            fmax = 2;
            cmin = -2;
            cmax = 2;
        }
        if (tipo == 7) {
            fmin = -3;
            fmax = 3;
            cmin = -3;
            cmax = 3;
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if ((i == 0 || j == 0 || i == width - 1 || j == height - 1) && tipo == 3) {
                    aux1 = aux2 = aux3 = 0;
                    procesar = false;
                }
                if ((i <= 1 || j <= 1 || i >= width - 2 || j >= height - 2) && tipo == 5) {
                    aux1 = aux2 = aux3 = 0;
                    procesar = false;
                }
                if ((i <= 2 || j <= 2 || i >= width - 3 || j >= height - 3) && tipo == 7) {
                    aux1 = aux2 = aux3 = 0;
                    procesar = false;
                }
                if (procesar == true) {
                    for (int f = fmin; f <= fmax; f++) {
                        for (int c = cmin; c <= cmax; c++) {
                            aux1 = aux1 + matriz[f + fmax][c + cmax] * this.red[i + c][j + f];
                            if (bpp > 8) {
                                aux2 = aux2 + matriz[f + fmax][c + cmax] * this.green[i + c][j + f];
                                aux3 = aux3 + matriz[f + fmax][c + cmax] * this.blue[i + c][j + f];
                            }
                        }
                    }
                }
                aux1 /= divisor;
                aux2 /= divisor;
                aux3 /= divisor;
                procesar = true;

                aux1 = (aux1 > 255) ? 255 : aux1;
                aux1 = (aux1 < 0) ? 0 : aux1;
                aux2 = (aux2 > 255) ? 255 : aux2;
                aux2 = (aux2 < 0) ? 0 : aux2;
                aux3 = (aux3 > 255) ? 255 : aux3;
                aux3 = (aux3 < 0) ? 0 : aux3;

                red[i][j] = (short) aux1;
                hred[(int) aux1]++;
                if (bpp > 8) {
                    green[i][j] = (short) aux2;
                    blue[i][j] = (short) aux3;
                    hgreen[(int) aux2]++;
                    hblue[(int) aux3]++;
                }
                aux1 = 0;
                aux2 = 0;
                aux3 = 0;
            }
        }

        this.red = red;
        this.hred = hred;
        this.green = green;
        this.hgreen = hgreen;
        this.blue = blue;
        this.hblue = hblue;

    }

    public void prewitt_sobel(double k) {

        short[][] matrizm = new short[width][height];
        long[] hmatrizm = new long[256];


        double aux1 = 0, aux2 = 0, aux3 = 0, auy1 = 0, auy2 = 0, auy3 = 0;
        boolean procesar = true;

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if ((i == 0 || j == 0 || i == width - 1 || j == height - 1)) {
                    aux1 = aux2 = aux3 = 0;
                    auy1 = auy2 = auy3 = 0;
                    procesar = false;
                }
                if (procesar == true) {

                    //derivada en x
                    aux1 = -1.d * (double) red[i - 1][j - 1] + 1.d * (double) red[i - 1][j + 1]
                            + -k * (double) red[i][j - 1] + k * (double) red[i][j + 1]
                            + -1.d * (double) red[i + 1][j - 1] + 1.d * (double) red[i + 1][j + 1];
                    if (bpp > 8) {
                        aux2 = -1.d * (double) green[i - 1][j - 1] + 1.d * (double) green[i - 1][j + 1]
                                + -k * (double) green[i][j - 1] + k * (double) green[i][j + 1]
                                + -1.d * (double) green[i + 1][j - 1] + 1.d * (double) green[i + 1][j + 1];
                        aux3 = -1.d * (double) blue[i - 1][j - 1] + 1.d * (double) blue[i - 1][j + 1]
                                + -k * (double) blue[i][j - 1] + k * (double) blue[i][j + 1]
                                + -1.d * (double) blue[i + 1][j - 1] + 1.d * (double) blue[i + 1][j + 1];

                    }

                    //derivada en y
                    auy1 = -1.d * (double) red[i - 1][j - 1] + 1.d * (double) red[i + 1][j - 1]
                            + -k * (double) red[i - 1][j] + k * (double) red[i + 1][j]
                            + -1.d * (double) red[i - 1][j + 1] + 1.d * (double) red[i + 1][j + 1];
                    if (bpp > 8) {
                        auy2 = -1.d * (double) green[i - 1][j - 1] + 1.d * (double) green[i - 1][j - 1]
                                + -k * (double) green[i - 1][j] + k * (double) green[i][j]
                                + -1.d * (double) green[i - 1][j + 1] + 1.d * (double) green[i + 1][j + 1];

                        auy3 = -1.d * (double) blue[i - 1][j - 1] + 1.d * (double) blue[i + 1][j - 1]
                                + -k * (double) blue[i - 1][j] + k * (double) blue[i + 1][j]
                                + -1.d * (double) blue[i - 1][j + 1] + 1.d * (double) blue[i + 1][j + 1];
                    }
                }

                procesar = true;

                aux1 = Math.max(Math.abs(aux1), Math.abs(auy1));
                if (bpp > 8) {
                    aux2 = Math.max(Math.abs(aux2), Math.abs(auy2));
                    aux3 = Math.max(Math.abs(aux3), Math.abs(auy3));
                }

                if (bpp > 8) {
                    aux1 = Math.max(aux1, Math.max(aux2, aux3));
                }

                aux1 = (aux1 > 255) ? 255 : aux1;

                matrizm[i][j] = (short) aux1;
                hmatrizm[(int) aux1]++;

                aux1 = aux2 = aux3 = 0;
                auy1 = auy2 = auy3 = 0;
            }
        }

        red = matrizm;
        hred = hmatrizm;
        if (bpp > 8) {
            green = matrizm;
            blue = matrizm;
            hgreen = hmatrizm;
            hblue = hmatrizm;
        }
    }

    public void prewitt_sobel_color(double k) {

        if (bpp > 8) {
            short[][] matrizr = new short[width][height];
            short[][] matrizg = new short[width][height];
            short[][] matrizb = new short[width][height];
            long[] hmatrizr = new long[256];
            long[] hmatrizg = new long[256];
            long[] hmatrizb = new long[256];

            double aux1 = 0, aux2 = 0, aux3 = 0, auy1 = 0, auy2 = 0, auy3 = 0;
            boolean procesar = true;

            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    if ((i == 0 || j == 0 || i == width - 1 || j == height - 1)) {
                        aux1 = aux2 = aux3 = 0;
                        auy1 = auy2 = auy3 = 0;
                        procesar = false;
                    }
                    if (procesar == true) {

                        //derivada en x
                        aux1 = -1.d * (double) red[i - 1][j - 1] + 1.d * (double) red[i - 1][j + 1]
                                + -k * (double) red[i][j - 1] + k * (double) red[i][j + 1]
                                + -1.d * (double) red[i + 1][j - 1] + 1.d * (double) red[i + 1][j + 1];
                        if (bpp > 8) {
                            aux2 = -1.d * (double) green[i - 1][j - 1] + 1.d * (double) green[i - 1][j + 1]
                                    + -k * (double) green[i][j - 1] + k * (double) green[i][j + 1]
                                    + -1.d * (double) green[i + 1][j - 1] + 1.d * (double) green[i + 1][j + 1];
                            aux3 = -1.d * (double) blue[i - 1][j - 1] + 1.d * (double) blue[i - 1][j + 1]
                                    + -k * (double) blue[i][j - 1] + k * (double) blue[i][j + 1]
                                    + -1.d * (double) blue[i + 1][j - 1] + 1.d * (double) blue[i + 1][j + 1];

                        }

                        //derivada en y
                        auy1 = -1.d * (double) red[i - 1][j - 1] + 1.d * (double) red[i + 1][j - 1]
                                + -k * (double) red[i - 1][j] + k * (double) red[i + 1][j]
                                + -1.d * (double) red[i - 1][j + 1] + 1.d * (double) red[i + 1][j + 1];
                        if (bpp > 8) {
                            auy2 = -1.d * (double) green[i - 1][j - 1] + 1.d * (double) green[i - 1][j - 1]
                                    + -k * (double) green[i - 1][j] + k * (double) green[i][j]
                                    + -1.d * (double) green[i - 1][j + 1] + 1.d * (double) green[i + 1][j + 1];

                            auy3 = -1.d * (double) blue[i - 1][j - 1] + 1.d * (double) blue[i + 1][j - 1]
                                    + -k * (double) blue[i - 1][j] + k * (double) blue[i + 1][j]
                                    + -1.d * (double) blue[i - 1][j + 1] + 1.d * (double) blue[i + 1][j + 1];
                        }
                    }


                    procesar = true;

                    aux1 = Math.max(Math.abs(aux1), Math.abs(auy1));
                    aux2 = Math.max(Math.abs(aux2), Math.abs(auy2));
                    aux3 = Math.max(Math.abs(aux3), Math.abs(auy3));

                    aux1 = (aux1 > 255) ? 255 : aux1;
                    aux2 = (aux2 > 255) ? 255 : aux2;
                    aux3 = (aux3 > 255) ? 255 : aux3;
                    double max = Math.max(aux1, Math.max(aux2, aux3));

                    if (max == aux1) {
                        matrizr[i][j] = (short) aux1;
                        hmatrizr[(int) aux1]++;
                    } else if (max == aux2) {
                        matrizg[i][j] = (short) aux2;
                        hmatrizg[(int) aux2]++;
                    } else {
                        matrizb[i][j] = (short) aux3;
                        hmatrizb[(int) aux3]++;
                    }
                    aux1 = aux2 = aux3 = 0;
                    auy1 = auy2 = auy3 = 0;
                }
            }
            red = matrizr;
            hred = hmatrizr;
            green = matrizg;
            blue = matrizb;
            hgreen = hmatrizg;
            hblue = hmatrizb;

        }
    }

    public void roberts() {

        short[][] matrizm = new short[width][height];
        long[] hmatrizm = new long[256];


        double aux1 = 0, aux2 = 0, aux3 = 0, auy1 = 0, auy2 = 0, auy3 = 0;
        boolean procesar = true;

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if ((i == 0 || j == 0 || i == width - 1 || j == height - 1)) {
                    aux1 = aux2 = aux3 = 0;
                    auy1 = auy2 = auy3 = 0;
                    procesar = false;
                }
                if (procesar == true) {

                    //derivada en x
                    aux1 = -1.d * (double) red[i - 1][j - 1] + 1.d * (double) red[i][j];
                    if (bpp > 8) {
                        aux2 = -1.d * (double) green[i - 1][j - 1] + 1.d * (double) green[i][j];
                        aux3 = -1.d * (double) blue[i - 1][j - 1] + 1.d * (double) blue[i][j];
                    }
                    //derivada en y
                    auy1 = -1.d * (double) red[i + 1][j - 1] + 1.d * (double) red[i][j];
                    if (bpp > 8) {
                        auy2 = -1.d * (double) green[i + 1][j - 1] + 1.d * (double) green[i][j];
                        auy3 = -1.d * (double) blue[i + 1][j - 1] + 1.d * (double) blue[i][j];
                    }
                }

                procesar = true;

                aux1 = Math.max(Math.abs(aux1), Math.abs(auy1));
                if (bpp > 8) {
                    aux2 = Math.max(Math.abs(aux2), Math.abs(auy2));
                    aux3 = Math.max(Math.abs(aux3), Math.abs(auy3));
                }

                if (bpp > 8) {
                    aux1 = Math.max(aux1, Math.max(aux2, aux3));
                }

                aux1 = (aux1 > 255) ? 255 : aux1;

                matrizm[i][j] = (short) aux1;
                hmatrizm[(int) aux1]++;

                aux1 = aux2 = aux3 = 0;
                auy1 = auy2 = auy3 = 0;
            }
        }

        red = matrizm;
        hred = hmatrizm;
        if (bpp > 8) {
            green = matrizm;
            blue = matrizm;
            hgreen = hmatrizm;
            hblue = hmatrizm;
        }
    }

    public void roberts_color() {

        if (bpp > 8) {
            short[][] matrizr = new short[width][height];
            short[][] matrizg = new short[width][height];
            short[][] matrizb = new short[width][height];
            long[] hmatrizr = new long[256];
            long[] hmatrizg = new long[256];
            long[] hmatrizb = new long[256];

            double aux1 = 0, aux2 = 0, aux3 = 0, auy1 = 0, auy2 = 0, auy3 = 0;
            boolean procesar = true;

            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    if ((i == 0 || j == 0 || i == width - 1 || j == height - 1)) {
                        aux1 = aux2 = aux3 = 0;
                        auy1 = auy2 = auy3 = 0;
                        procesar = false;
                    }
                    if (procesar == true) {

                        //derivada en x
                        aux1 = -1.d * (double) red[i - 1][j - 1] + 1.d * (double) red[i][j];
                        aux2 = -1.d * (double) green[i - 1][j - 1] + 1.d * (double) green[i][j];
                        aux3 = -1.d * (double) blue[i - 1][j - 1] + 1.d * (double) blue[i][j];

                        //derivada en y
                        auy1 = -1.d * (double) red[i + 1][j - 1] + 1.d * (double) red[i][j];
                        auy2 = -1.d * (double) green[i + 1][j - 1] + 1.d * (double) green[i][j];
                        auy3 = -1.d * (double) blue[i + 1][j - 1] + 1.d * (double) blue[i][j];

                    }

                    procesar = true;

                    aux1 = Math.max(Math.abs(aux1), Math.abs(auy1));
                    aux2 = Math.max(Math.abs(aux2), Math.abs(auy2));
                    aux3 = Math.max(Math.abs(aux3), Math.abs(auy3));

                    aux1 = (aux1 > 255) ? 255 : aux1;
                    aux2 = (aux2 > 255) ? 255 : aux2;
                    aux3 = (aux3 > 255) ? 255 : aux3;
                    double max = Math.max(aux1, Math.max(aux2, aux3));

                    if (max == aux1) {
                        matrizr[i][j] = (short) aux1;
                        hmatrizr[(int) aux1]++;
                    } else if (max == aux2) {
                        matrizg[i][j] = (short) aux2;
                        hmatrizg[(int) aux2]++;
                    } else {
                        matrizb[i][j] = (short) aux3;
                        hmatrizb[(int) aux3]++;
                    }
                    aux1 = aux2 = aux3 = 0;
                    auy1 = auy2 = auy3 = 0;
                }
            }
            red = matrizr;
            hred = hmatrizr;
            green = matrizg;
            blue = matrizb;
            hgreen = hmatrizg;
            hblue = hmatrizb;

        }
    }

    //MEDIANA ////////////////////////////////////////////////////////////////
    public void mediana(int tipo) {

        short[][] matrizr = new short[width][height];
        short[][] matrizg = new short[width][height];
        short[][] matrizb = new short[width][height];
        long[] hmatrizr = new long[256];
        long[] hmatrizg = new long[256];
        long[] hmatrizb = new long[256];

        double coloresMediana3r[] = new double[tipo * tipo];
        double coloresMediana3g[] = new double[tipo * tipo];
        double coloresMediana3b[] = new double[tipo * tipo];

        Arrays.fill(coloresMediana3r, (double) 0);
        Arrays.fill(coloresMediana3g, (double) 0);
        Arrays.fill(coloresMediana3b, (double) 0);

        double aux1, aux2 = 0, aux3 = 0;
        int fmin = 0, cmin = 0, fmax = 0, cmax = 0;
        boolean procesar = true;
        int posicion = (tipo * tipo) - 1;

        if (tipo == 3) {
            fmin = -1;
            fmax = 1;
            cmin = -1;
            cmax = 1;
        }
        if (tipo == 5) {
            fmin = -2;
            fmax = 2;
            cmin = -2;
            cmax = 2;
        }
        if (tipo == 7) {
            fmin = -3;
            fmax = 3;
            cmin = -3;
            cmax = 3;
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {

                if ((i == 0 || j == 0 || i == width - 1 || j == height - 1) && tipo == 3) {
                    procesar = false;
                    Arrays.fill(coloresMediana3r, (double) 0);
                    if (bpp > 8) {
                        Arrays.fill(coloresMediana3g, (double) 0);
                        Arrays.fill(coloresMediana3b, (double) 0);
                    }
                }
                if ((i <= 1 || j <= 1 || i >= width - 2 || j >= height - 2) && tipo == 5) {

                    procesar = false;
                    Arrays.fill(coloresMediana3r, (double) 0);
                    if (bpp > 8) {
                        Arrays.fill(coloresMediana3g, (double) 0);
                        Arrays.fill(coloresMediana3b, (double) 0);
                    }
                }
                if ((i <= 2 || j <= 2 || i >= width - 3 || j >= height - 3) && tipo == 7) {

                    procesar = false;
                    Arrays.fill(coloresMediana3r, (double) 0);
                    if (bpp > 8) {
                        Arrays.fill(coloresMediana3g, (double) 0);
                        Arrays.fill(coloresMediana3b, (double) 0);
                    }
                }
                if (procesar == true) {
                    for (int f = fmin; f <= fmax; f++) {
                        for (int c = cmin; c <= cmax; c++) {
                            coloresMediana3r[posicion] = (double) red[i + c][j + f];
                            if (bpp > 8) {
                                coloresMediana3g[posicion] = (double) green[i + c][j + f];
                                coloresMediana3b[posicion] = (double) blue[i + c][j + f];
                            }
                            posicion = posicion - 1;
                        }
                    }
                }

                procesar = true;

                Arrays.sort(coloresMediana3r);
                if (bpp > 8) {
                    Arrays.sort(coloresMediana3g);
                    Arrays.sort(coloresMediana3b);
                }
                aux1 = coloresMediana3r[(int) (tipo * tipo) / 2];
                if (bpp > 8) {
                    aux2 = coloresMediana3g[(int) (tipo * tipo) / 2];
                    aux3 = coloresMediana3b[(int) (tipo * tipo) / 2];
                }
                matrizr[i][j] = (short) aux1;
                hmatrizr[(int) aux1]++;
                if (bpp > 8) {
                    matrizg[i][j] = (short) aux2;
                    hmatrizg[(int) aux2]++;
                    matrizb[i][j] = (short) aux3;
                    hmatrizb[(int) aux3]++;
                }

                posicion = tipo * tipo - 1;
            }

        }

        red = matrizr;
        hred = hmatrizr;
        green = matrizg;
        blue = matrizb;
        hgreen = hmatrizg;
        hblue = hmatrizb;

    }

    //ELEMENTO ESTRUCTURANTE ***************************************************
    //DILATACION****************************************************************
    public void elemEstrucDilat(double[][] matriz, int tipo, boolean guardarCambios) {

        short[][] matrizr = new short[width][height];
        short[][] matrizg = new short[width][height];
        short[][] matrizb = new short[width][height];
        long[] hmatrizr = new long[256];
        long[] hmatrizg = new long[256];
        long[] hmatrizb = new long[256];

        double color, pix;

        double aux1 = 0, aux2 = 0, aux3 = 0;
        int fmin = 0, cmin = 0, fmax = 0, cmax = 0;
        boolean procesar = true;
        int posicion = (tipo * tipo) - 1;

        if (tipo == 3) {
            fmin = -1;
            fmax = 1;
            cmin = -1;
            cmax = 1;
        }
        if (tipo == 5) {
            fmin = -2;
            fmax = 2;
            cmin = -2;
            cmax = 2;
        }
        if (tipo == 7) {
            fmin = -3;
            fmax = 3;
            cmin = -3;
            cmax = 3;
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {

                if ((i == 0 || j == 0 || i == width - 1 || j == height - 1)) {
                    aux1 = aux2 = aux3 = 0;
                    procesar = false;
                }
                if (procesar == true) {
                    for (int f = fmin; f <= fmax; f++) {
                        for (int c = cmin; c <= cmax; c++) {
                            pix = matriz[f + fmax][c + cmax];
                            if (pix != 0) {
                                color = red[i + c][j + f];
                                aux1 = ((short) color | (short) aux1);

                                if (bpp > 8) {
                                    color = green[i + c][j + f];
                                    aux2 = ((short) color | (short) aux2);

                                    color = blue[i + c][j + f];
                                    aux3 = ((short) color | (short) aux3);
                                }
                            }
                        }
                    }
                }

                procesar = true;

                matrizr[i][j] = (short) aux1;
                hmatrizr[(int) aux1]++;
                matrizg[i][j] = (short) aux2;
                hmatrizg[(int) aux2]++;
                matrizb[i][j] = (short) aux3;
                hmatrizb[(int) aux3]++;

                aux1 = aux2 = aux3 = 0;

            }
        }


        if (guardarCambios == true) {
            redDil = matrizr;
            greenDil = matrizg;
            blueDil = matrizb;
        } else {
            red = matrizr;
            green = matrizg;
            blue = matrizb;
            hred = hmatrizr;
            hgreen = hmatrizg;
            hblue = hmatrizb;
        }

    }

    //ELEMENTO ESTRUCTURANTE ***************************************************
    // EROSION  *****************************************************************
    public void elemEstrucEros(double[][] matriz, int tipo, boolean guardarCambios) {

        short[][] matrizr = new short[width][height];
        short[][] matrizg = new short[width][height];
        short[][] matrizb = new short[width][height];
        long[] hmatrizr = new long[256];
        long[] hmatrizg = new long[256];
        long[] hmatrizb = new long[256];

        double color, pix;

        double aux1 = 255, aux2 = 255, aux3 = 255;
        int fmin = 0, cmin = 0, fmax = 0, cmax = 0;
        boolean procesar = true;

        if (tipo == 3) {
            fmin = -1;
            fmax = 1;
            cmin = -1;
            cmax = 1;
        }
        if (tipo == 5) {
            fmin = -2;
            fmax = 2;
            cmin = -2;
            cmax = 2;
        }
        if (tipo == 7) {
            fmin = -3;
            fmax = 3;
            cmin = -3;
            cmax = 3;
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {

                if ((i == 0 || j == 0 || i == width - 1 || j == height - 1)) {
                    aux1 = aux2 = aux3 = 0;
                    procesar = false;
                }
                if (procesar == true) {
                    for (int f = fmin; f <= fmax; f++) {
                        for (int c = cmin; c <= cmax; c++) {
                            pix = matriz[f + fmax][c + cmax];
                            if (pix != 0) {
                                color = red[i + c][j + f];
                                aux1 = ((short) color & (short) aux1);

                                if (bpp > 8) {
                                    color = green[i + c][j + f];
                                    aux2 = ((short) color & (short) aux2);

                                    color = blue[i + c][j + f];
                                    aux3 = ((short) color & (short) aux3);
                                }
                            }
                        }
                    }
                }

                procesar = true;

                matrizr[i][j] = (short) aux1;
                hmatrizr[(int) aux1]++;
                matrizg[i][j] = (short) aux2;
                hmatrizg[(int) aux2]++;
                matrizb[i][j] = (short) aux3;
                hmatrizb[(int) aux3]++;

                aux1 = aux2 = aux3 = 255;

            }
        }

        if (guardarCambios == true) {
            redEros = matrizr;
            greenEros = matrizg;
            blueEros = matrizb;
        } else {
            red = matrizr;
            green = matrizg;
            blue = matrizb;
            hred = hmatrizr;
            hgreen = hmatrizg;
            hblue = hmatrizb;
        }
    }

    //CIERRE ****************************************************************
    public void cierre(double[][] matriz, int tipo) {


        short[][] matrizr = new short[width][height];
        short[][] matrizg = new short[width][height];
        short[][] matrizb = new short[width][height];
        long[] hmatrizr = new long[256];
        long[] hmatrizg = new long[256];
        long[] hmatrizb = new long[256];

        double color, pix;

        double aux1 = 255, aux2 = 255, aux3 = 255;
        int fmin = 0, cmin = 0, fmax = 0, cmax = 0;
        boolean procesar = true;

        if (tipo == 3) {
            fmin = -1;
            fmax = 1;
            cmin = -1;
            cmax = 1;
        }
        if (tipo == 5) {
            fmin = -2;
            fmax = 2;
            cmin = -2;
            cmax = 2;
        }
        if (tipo == 7) {
            fmin = -3;
            fmax = 3;
            cmin = -3;
            cmax = 3;
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if ((i == 0 || j == 0 || i == width - 1 || j == height - 1)) {
                    aux1 = aux2 = aux3 = 0;
                    procesar = false;
                }
                if (procesar == true) {
                    for (int f = fmin; f <= fmax; f++) {
                        for (int c = cmin; c <= cmax; c++) {
                            pix = matriz[f + fmax][c + cmax];
                            if (pix != 0) {
                                color = redDil[i + c][j + f];
                                //acum = matrizr[f + fmax][c + cmax];
                                aux1 = ((short) color & (short) aux1);

                                if (bpp > 8) {
                                    color = greenDil[i + c][j + f];
                                    //acum = matrizr[f + fmax][c + cmax];
                                    aux2 = ((short) color & (short) aux2);

                                    color = blueDil[i + c][j + f];
                                    //acum = matrizr[f + fmax][c + cmax];
                                    aux3 = ((short) color & (short) aux3);
                                }
                            }
                        }
                    }
                }

                procesar = true;

                matrizr[i][j] = (short) aux1;
                hmatrizr[(int) aux1]++;
                matrizg[i][j] = (short) aux2;
                hmatrizg[(int) aux2]++;
                matrizb[i][j] = (short) aux3;
                hmatrizb[(int) aux3]++;

                aux1 = aux2 = aux3 = 255;

            }
        }


        red = matrizr;
        green = matrizg;
        blue = matrizb;
        hred = hmatrizr;
        hgreen = hmatrizg;
        hblue = hmatrizb;

    }

    //APERTURA****************************************************************
    public void apertura(double[][] matriz, int tipo, boolean guardarCambios) {

        short[][] matrizr = new short[width][height];
        short[][] matrizg = new short[width][height];
        short[][] matrizb = new short[width][height];
        long[] hmatrizr = new long[256];
        long[] hmatrizg = new long[256];
        long[] hmatrizb = new long[256];

        double color, pix;

        double aux1 = 0, aux2 = 0, aux3 = 0;
        int fmin = 0, cmin = 0, fmax = 0, cmax = 0;
        boolean procesar = true;
        int posicion = (tipo * tipo) - 1;

        if (tipo == 3) {
            fmin = -1;
            fmax = 1;
            cmin = -1;
            cmax = 1;
        }
        if (tipo == 5) {
            fmin = -2;
            fmax = 2;
            cmin = -2;
            cmax = 2;
        }
        if (tipo == 7) {
            fmin = -3;
            fmax = 3;
            cmin = -3;
            cmax = 3;
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {

                if ((i == 0 || j == 0 || i == width - 1 || j == height - 1)) {
                    aux1 = aux2 = aux3 = 0;
                    procesar = false;
                }
                if (procesar == true) {
                    for (int f = fmin; f <= fmax; f++) {
                        for (int c = cmin; c <= cmax; c++) {
                            pix = matriz[f + fmax][c + cmax];
                            if (pix != 0) {
                                color = redEros[i + c][j + f];
                                aux1 = ((short) color | (short) aux1);

                                if (bpp > 8) {
                                    color = greenEros[i + c][j + f];
                                    aux2 = ((short) color | (short) aux2);

                                    color = blueEros[i + c][j + f];
                                    aux3 = ((short) color | (short) aux3);
                                }
                            }
                        }
                    }
                }

                procesar = true;

                matrizr[i][j] = (short) aux1;
                hmatrizr[(int) aux1]++;
                matrizg[i][j] = (short) aux2;
                hmatrizg[(int) aux2]++;
                matrizb[i][j] = (short) aux3;
                hmatrizb[(int) aux3]++;

                aux1 = aux2 = aux3 = 0;

            }
        }

        
        
        if (guardarCambios == true) {
            redDil = matrizr;
            greenDil = matrizg;
            blueDil = matrizb;
        } else {
            red = matrizr;
            green = matrizg;
            blue = matrizb;
            hred = hmatrizr;
            hgreen = hmatrizg;
            hblue = hmatrizb;
        }
    }

    // TOP HAT  *****************************************************************
    public void topHat() {

        short[][] matrizr = new short[width][height];
        short[][] matrizg = new short[width][height];
        short[][] matrizb = new short[width][height];
        long[] hmatrizr = new long[256];
        long[] hmatrizg = new long[256];
        long[] hmatrizb = new long[256];

        double color, pix;

        double aux1=0, aux2=0, aux3=0;


        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                
                aux1 = red[i][j] - redDil[i][j];
                aux1 = (aux1 < 0) ? 0 : aux1;
                
                if (bpp > 8) {
                aux2 = green[i][j] - greenDil[i][j];
                aux3 = blue[i][j] - blueDil[i][j];
                aux2 = (aux2 < 0) ? 0 : aux2;
                aux3 = (aux1 < 0) ? 0 : aux3;
                }


                //System.out.println(aux3);

                matrizr[i][j] = (short) aux1;
                hmatrizr[(int) aux1]++;
                
                if (bpp > 8) {
                matrizg[i][j] = (short) aux2;
                hmatrizg[(int) aux2]++;
                matrizb[i][j] = (short) aux3;
                hmatrizb[(int) aux3]++;
                }

            }
        }

        red = matrizr;
        hred = hmatrizr;
        
        if (bpp > 8) {
        green = matrizg;
        blue = matrizb;
        hgreen = hmatrizg;
        hblue = hmatrizb;
        }

    }

    public void binarizar() {
        if (bpp > 1) {
            float aux;
            Arrays.fill(hred, (byte) 0);
            Arrays.fill(hgreen, (byte) 0);
            Arrays.fill(hblue, (byte) 0);
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    if (bpp > 8) {
                        aux = (float) (red[i][j] * 0.3 + green[i][j] * 0.59 + blue[i][j] * 0.11);
                        aux = (aux >= 128.0) ? 1.f : 0.f;
                    } else {
                        aux = (red[i][j] >= 128.0) ? 1.f : 0.f;
                    }
                    red[i][j] = (short) aux;
                    hred[(int) aux]++;
                    if (bpp >= 8) {
                        green[i][j] = (short) aux;
                        blue[i][j] = (short) aux;
                        hgreen[(int) aux]++;
                        hblue[(int) aux]++;
                        if (this.bpp == 32) {
                            alpha[i][j] = (short) 255;
                        }
                    }
                }
            }
            bpp = 1;
            type = BufferedImage.TYPE_BYTE_BINARY;
        }

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
    public short redDil[][];
    public short greenDil[][];
    public short blueDil[][];
    public short redEros[][];
    public short greenEros[][];
    public short blueEros[][];
}
