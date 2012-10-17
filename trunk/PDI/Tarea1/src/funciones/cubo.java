/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package funciones;

import java.util.ArrayList;

/**
 *
 * @author Marvin
 */
public class cubo {

    private box[][][] boxs8;
    private box[][][] boxs16;
    private boolean bit; // true 8b false 16b

    public cubo() {
        boxs8 = null;
        boxs16 = null;
        bit = false;
    }

    public void cubode(int bits) {
        if (bits == 8) {
            boxs8 = new box[8][8][4];
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    for (int k = 0; k < 4; k++) {
                        boxs8[i][j][k] = new box(i * 32, j * 32, k * 64, (i + 1) * 32 - 1, (j + 1) * 32 - 1, (k + 1) * 64 - 1);
                    }

                }
            }
            bit = true;
        } else {
            boxs16 = new box[64][32][32];
            for (int i = 0; i < 64; i++) {
                for (int j = 0; j < 32; j++) {
                    for (int k = 0; k < 32; k++) {
                        boxs16[i][j][k] = new box(i * 4, j * 8, k * 8, (i + 1) * 4 - 1, (j + 1) * 8 - 1, (k + 1) * 8 - 1);
                    }

                }
            }
            bit = false;
        }
    }

    public void ubicarpixel(short r, short g, short b) {
        if (bit) {
            boxs8[r / 32][g / 32][b / 64].contiene(r, g, b);
        } else {
            boxs16[r / 4][g / 8][b / 8].contiene(r, g, b);
        }
    }

    public int[] getColor(short r, short g, short b) {
        if (bit) {
            return boxs8[r / 32][g / 32][b / 64].color;
        } else {
            return boxs16[r / 4][g / 8][b / 8].color;
        }
    }

    public void fijarcentros() {
        if (bit) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    for (int k = 0; k < 4; k++) {
                        boxs8[i][j][k].centros();
                    }

                }
            }
        } else {
            for (int i = 0; i < 64; i++) {
                for (int j = 0; j < 32; j++) {
                    for (int k = 0; k < 32; k++) {
                        boxs16[i][j][k].centros();
                    }

                }
            }
        }
    }

    private class box {

        public box(int p0, int p1, int p2, int d0, int d1, int d2) {
            this.p0 = p0;
            this.p1 = p1;
            this.p2 = p2;
            this.d0 = d0;
            this.d1 = d1;
            this.d2 = d2;
            pixeles = new ArrayList<>();
            color = new int[]{0, 0, 0};
            cont = 0;
        }

        public void contiene(short r, short g, short b) {
            reg re;
            boolean encontrado = false;
            if (pixeles.isEmpty()) {
                pixeles.add(new reg(r, g, b));
                cont++;
            } else {
                for (int i = 0; i < pixeles.size(); i++) {
                    re = pixeles.get(i);
                    if (r == re.pixel[0] && g == re.pixel[1] && b == re.pixel[2]) {
                        re.incrementar();
                        cont++;
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) {
                    pixeles.add(new reg(r, g, b));
                    cont++;
                }
            }

        }

        public void centros() {
            reg re;
            float aux;
            float r = 0, b = 0, g = 0;
            if (pixeles.isEmpty()) {
                color[0] = (d0 + p0) / 2;
                color[1] = (d1 + p1) / 2;
                color[2] = (d2 + p2) / 2;
            } else {
                for (int i = 0; i < pixeles.size(); i++) {
                    re = pixeles.get(i);
                    aux = (float) re.count / (float) cont;
                    r += (float) re.pixel[0] * aux;
                    g += (float) re.pixel[1] * aux;
                    b += (float) re.pixel[2] * aux;

                }
                color[0] = (int) r;
                color[1] = (int) g;
                color[2] = (int) b;
            }
        }

        private class reg {

            int[] pixel;
            int count;

            public reg(short r, short g, short b) {
                pixel = new int[]{r, g, b};
                count = 1;
            }

            public void incrementar() {
                count++;
            }
        }
        int p0, p1, p2, d0, d1, d2, cont;
        int[] color;
        ArrayList<reg> pixeles;
    }

}
