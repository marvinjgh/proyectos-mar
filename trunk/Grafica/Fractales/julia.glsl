#version 330

uniform sampler1D tex;
uniform vec2 c;
uniform int iter;

out vec4 outputColor;

void main() {
	vec2 z;
	z.x = 3.0 * (gl_TexCoord[0].x - 0.5);
	z.y = 3.0 * (gl_TexCoord[0].y - 0.5);

	int i;
	for(i=0; i<iter; i++) {
		float x = (z.x * z.x - z.y * z.y) + -0.7;
		float y = (z.y * z.x + z.x * z.y) + 0.27015;

		if((x * x + y * y) > 4.0) break;
		z.x = x;
		z.y = y;
	}
	
	float d = float(i)/float(iter);
	gl_FragColor = (i== iter ? vec4(1,0,0,1) : texture1D(tex, float(i) / float(iter)));
}
