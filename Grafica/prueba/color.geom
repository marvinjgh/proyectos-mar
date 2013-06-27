#version 330

layout(lines) in;
layout(line_strip) out;

uniform float sin60;
uniform int iter;

void recur(int i, vec2 a, vec2 b){
    //float dx=(gl_in[1].gl_Position.x-gl_in[0].gl_Position.x)/3.f;
    //float dy=(gl_in[1].gl_Position.y-gl_in[0].gl_Position.y)/3.f;
	
	vec2 d = (b-a) /3.f;
    //float xx=gl_in[0].gl_Position.x+3*dx/2.-dy*sin60;
	//float yy=gl_in[0].gl_Position.y+3*dy/2.+dx*sin60;

	vec2 c = vec2(a.x+3*d.x/2.-d.y*sin60,a.y+3*d.y/2.-d.y*sin60);
    //gl_Position = vec4(gl_in[0].gl_Position.x+dx,gl_in[0].gl_Position.y+dy,gl_in[1].gl_Position.zw);
    //EmitVertex();
    
    if(i<=0){
        gl_Position = vec4(a,gl_in[0].gl_Position.zw);
        EmitVertex();
        gl_Position = vec4(b,gl_in[0].gl_Position.zw);
        EmitVertex();
    }else{
        recur(i-1,a,a+d);
        recur(i-1,a+d,c);
        recur(i-1,c,b-d);
        recur(i-1,b-d,b);
    }
}

void main() {
    //gl_Position = gl_in[0].gl_Position;
    //EmitVertex();
    //gl_Position = gl_in[1].gl_Position;
    //EmitVertex();
    
	recur(iter, gl_in[0].gl_Position.xy, gl_in[1].gl_Position.xy);
    
  EndPrimitive();
}







