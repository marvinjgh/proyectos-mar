#version 330

uniform sampler1D tex;
uniform vec2 c;
uniform float scale;
uniform int iter;

out vec4 outputColor;

void main() {
	vec2 z, c2;

	c2.x = 1.3333 * (gl_TexCoord[0].x - 0.5) * scale - c.x;
	c2.y = (gl_TexCoord[0].y - 0.5) * scale - c.y;

	int i;
	z = c2;
	float x, y;
	for(i=0; i<iter; i++) {
		x = (z.x * z.x - z.y * z.y) + c2.x;
		y = (z.y * z.x + z.x * z.y) + c2.y;

		if((x * x + y * y) > 4.0) break;
		z.x = x;
		z.y = y;
	}

	outputColor = (i== iter ? vec4(0,0,0,1) : texture1D(tex, float(i) / float(iter)));
}
