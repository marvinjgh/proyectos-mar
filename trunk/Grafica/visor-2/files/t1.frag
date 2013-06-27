#version 330

uniform sampler2D mysampler1;
uniform sampler2D mysampler2;
uniform sampler2D mysampler3;
uniform sampler2D mysampler4;
uniform sampler2D mysampler5;

in float select;
in vec2 UV;
in vec4 color;
out vec4 outputColor;

void main()
{
	
	if (color.rgba != vec4(-1.0f,-1.0f,-1.0f,-1.0f)){
		outputColor = color;
	}else{
		if (select==0.0f){
			outputColor = texture(mysampler1,UV);
		}else
		if (select==1.0f){
		outputColor = texture(mysampler2,UV);
		}else
		if (select==2.0f){
			outputColor = texture(mysampler3,UV);
		}else
		if (select==3.0f){
			outputColor = texture(mysampler4,UV);
		}else
		if (select==4.0f){
			outputColor = texture(mysampler5,UV);
		}
	}
	//outputColor *= vec4(0,1,1,1);
}